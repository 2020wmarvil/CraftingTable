#include "core/log.h"

#include <algorithm>
#include <atomic>
#include <cctype>
#include <mutex>
#include <print>
#include <vector>

namespace ct::log
{

namespace
{

struct State
{
    std::mutex mutex;
    std::vector<std::unique_ptr<Sink>> sinks;
    std::atomic<Level> minLevel = Level::Info;
};

State& state()
{
    static State s_state;
    return s_state;
}

void appendJsonString(std::string& out, std::string_view text)
{
    out += '"';
    for (const char c : text) {
        switch (c) {
        case '"':
            out += "\\\"";
            break;
        case '\\':
            out += "\\\\";
            break;
        case '\n':
            out += "\\n";
            break;
        case '\r':
            out += "\\r";
            break;
        case '\t':
            out += "\\t";
            break;
        default:
            if (static_cast<unsigned char>(c) < 0x20) {
                out += std::format("\\u{:04x}", static_cast<unsigned char>(c));
            } else {
                out += c;
            }
        }
    }
    out += '"';
}

std::string normalizePath(std::string_view path)
{
    std::string result(path);
    std::ranges::replace(result, '\\', '/');
    return result;
}

bool startsWithIgnoringCase(std::string_view text, std::string_view prefix)
{
    if (text.size() < prefix.size()) {
        return false;
    }
    return std::ranges::equal(text.substr(0, prefix.size()), prefix, [](char a, char b) {
        return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
    });
}

} // namespace

std::string_view levelName(Level level)
{
    switch (level) {
    case Level::Trace:
        return "trace";
    case Level::Debug:
        return "debug";
    case Level::Info:
        return "info";
    case Level::Warn:
        return "warn";
    case Level::Error:
        return "error";
    case Level::Fatal:
        return "fatal";
    }
    return "unknown";
}

StreamSink::StreamSink(std::FILE* stream, Format format) : m_stream(stream), m_format(format) {}

void StreamSink::write(const Record& record)
{
    const std::string line = m_format == Format::Json ? formatJson(record) : formatText(record);
    std::println(m_stream, "{}", line);
    std::fflush(m_stream);
}

void addSink(std::unique_ptr<Sink> sink)
{
    const std::scoped_lock lock(state().mutex);
    state().sinks.push_back(std::move(sink));
}

void clearSinks()
{
    const std::scoped_lock lock(state().mutex);
    state().sinks.clear();
}

void setMinLevel(Level level)
{
    state().minLevel.store(level, std::memory_order_relaxed);
}

Level minLevel()
{
    return state().minLevel.load(std::memory_order_relaxed);
}

bool isEnabled(Level level)
{
    return level >= minLevel();
}

void write(Level level, std::string_view category, std::string_view message, std::source_location location)
{
    if (!isEnabled(level)) {
        return;
    }
    const Record record{
        .level = level,
        .category = category,
        .message = message,
        .time = std::chrono::system_clock::now(),
        .location = location,
    };
    const std::scoped_lock lock(state().mutex);
    for (const auto& sink : state().sinks) {
        sink->write(record);
    }
}

std::string formatJson(const Record& record)
{
    std::string out;
    out.reserve(128 + record.message.size());
    out += std::format(R"({{"time":"{:%FT%TZ}","level":")", std::chrono::floor<std::chrono::milliseconds>(record.time));
    out += levelName(record.level);
    out += R"(","category":)";
    appendJsonString(out, record.category);
    out += R"(,"message":)";
    appendJsonString(out, record.message);
    out += R"(,"file":)";
    appendJsonString(out, relativeSourcePath(record.location.file_name()));
    out += std::format(R"(,"line":{}}})", record.location.line());
    return out;
}

std::string formatText(const Record& record)
{
    std::string level(levelName(record.level));
    std::ranges::transform(level, level.begin(), [](char c) { return static_cast<char>(std::toupper(c)); });
    return std::format("{:%T} {:<5} [{}] {} ({}:{})", std::chrono::floor<std::chrono::milliseconds>(record.time), level,
                       record.category, record.message, relativeSourcePath(record.location.file_name()),
                       record.location.line());
}

std::string relativeSourcePath(std::string_view path)
{
    std::string normalized = normalizePath(path);
    const std::string root = normalizePath(CT_SOURCE_DIR) + '/';
    if (startsWithIgnoringCase(normalized, root)) {
        normalized.erase(0, root.size());
    }
    return normalized;
}

} // namespace ct::log
