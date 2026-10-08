#pragma once

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <format>
#include <memory>
#include <source_location>
#include <string>
#include <string_view>

namespace ct::log
{

/// Severity of a log record, in increasing order.
enum class Level : uint8_t
{
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal,
};

/// Lowercase name of a level, as used in JSON output ("info", "warn", ...).
std::string_view levelName(Level level);

/// One log entry. Views are only valid for the duration of Sink::write.
struct Record
{
    Level level = Level::Info;
    std::string_view category;
    std::string_view message;
    std::chrono::system_clock::time_point time;
    std::source_location location;
};

/// Receives every record at or above the global minimum level. Called under the logger lock.
class Sink
{
public:
    virtual ~Sink() = default;
    virtual void write(const Record& record) = 0;
};

enum class Format : uint8_t
{
    Text, ///< Human-readable single line.
    Json, ///< One JSON object per line (JSON Lines).
};

/// Writes records to a C stream (typically stderr) in the given format, flushing after each record.
class StreamSink final : public Sink
{
public:
    StreamSink(std::FILE* stream, Format format);
    void write(const Record& record) override;

private:
    std::FILE* m_stream;
    Format m_format;
};

/// Registers a sink. Thread-safe.
void addSink(std::unique_ptr<Sink> sink);

/// Removes all sinks. Thread-safe.
void clearSinks();

/// Records below this level are discarded before formatting. Default is Info.
void setMinLevel(Level level);
Level minLevel();
bool isEnabled(Level level);

/// Dispatches a record to all sinks. Prefer the CT_LOG_* macros, which skip formatting for disabled levels.
void write(Level level, std::string_view category, std::string_view message,
           std::source_location location = std::source_location::current());

/// Formats a record as a single JSON object without a trailing newline.
/// Fields: time (UTC ISO 8601), level, category, message, file (repo-relative when possible), line.
std::string formatJson(const Record& record);

/// Formats a record as a single human-readable line without a trailing newline.
std::string formatText(const Record& record);

/// Strips the repository root from a source path and normalizes separators to '/'.
/// Paths outside the repository are returned normalized but otherwise unchanged.
std::string relativeSourcePath(std::string_view path);

} // namespace ct::log

#define CT_LOG(level, category, ...)                                                                                   \
    do {                                                                                                               \
        if (::ct::log::isEnabled(level)) {                                                                             \
            ::ct::log::write((level), (category), std::format(__VA_ARGS__));                                           \
        }                                                                                                              \
    } while (false)

#define CT_LOG_TRACE(category, ...) CT_LOG(::ct::log::Level::Trace, category, __VA_ARGS__)
#define CT_LOG_DEBUG(category, ...) CT_LOG(::ct::log::Level::Debug, category, __VA_ARGS__)
#define CT_LOG_INFO(category, ...) CT_LOG(::ct::log::Level::Info, category, __VA_ARGS__)
#define CT_LOG_WARN(category, ...) CT_LOG(::ct::log::Level::Warn, category, __VA_ARGS__)
#define CT_LOG_ERROR(category, ...) CT_LOG(::ct::log::Level::Error, category, __VA_ARGS__)
#define CT_LOG_FATAL(category, ...) CT_LOG(::ct::log::Level::Fatal, category, __VA_ARGS__)
