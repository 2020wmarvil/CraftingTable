#include "core/log.h"

#include <doctest/doctest.h>

#include <string>
#include <vector>

namespace
{

struct CapturedRecord
{
    ct::log::Level level;
    std::string category;
    std::string message;
};

class CaptureSink final : public ct::log::Sink
{
public:
    explicit CaptureSink(std::vector<CapturedRecord>& records) : m_records(records) {}

    void write(const ct::log::Record& record) override
    {
        m_records.push_back({
            .level = record.level,
            .category = std::string(record.category),
            .message = std::string(record.message),
        });
    }

private:
    std::vector<CapturedRecord>& m_records;
};

/// Installs a capture sink for the duration of a test and restores defaults afterwards.
class ScopedCapture
{
public:
    ScopedCapture()
    {
        ct::log::clearSinks();
        ct::log::addSink(std::make_unique<CaptureSink>(m_records));
    }
    ~ScopedCapture()
    {
        ct::log::clearSinks();
        ct::log::setMinLevel(ct::log::Level::Info);
    }
    ScopedCapture(const ScopedCapture&) = delete;
    ScopedCapture& operator=(const ScopedCapture&) = delete;

    const std::vector<CapturedRecord>& records() const { return m_records; }

private:
    std::vector<CapturedRecord> m_records;
};

ct::log::Record makeRecord(std::string_view message)
{
    return {
        .level = ct::log::Level::Warn,
        .category = "test",
        .message = message,
        .time = {},
        .location = std::source_location::current(),
    };
}

} // namespace

TEST_CASE("Macros format messages and dispatch to sinks")
{
    const ScopedCapture capture;
    CT_LOG_INFO("render", "frame {} took {}ms", 7, 16);

    REQUIRE(capture.records().size() == 1);
    CHECK(capture.records()[0].level == ct::log::Level::Info);
    CHECK(capture.records()[0].category == "render");
    CHECK(capture.records()[0].message == "frame 7 took 16ms");
}

TEST_CASE("Records below the minimum level are discarded")
{
    const ScopedCapture capture;
    ct::log::setMinLevel(ct::log::Level::Warn);
    CT_LOG_INFO("test", "dropped");
    CT_LOG_WARN("test", "kept");

    REQUIRE(capture.records().size() == 1);
    CHECK(capture.records()[0].message == "kept");
}

TEST_CASE("JSON output has the documented fields")
{
    const std::string json = ct::log::formatJson(makeRecord("hello"));
    CHECK(json.starts_with(R"({"time":"1970-01-01T00:00:00.000Z","level":"warn","category":"test","message":"hello")"));
    CHECK(json.find(R"("file":"tests/unit/log_tests.cpp")") != std::string::npos);
    CHECK(json.find(R"("line":)") != std::string::npos);
    CHECK(json.ends_with("}"));
}

TEST_CASE("JSON output escapes special characters")
{
    const std::string json = ct::log::formatJson(makeRecord("quote\" backslash\\ newline\n tab\t bell\a"));
    CHECK(json.find(R"("message":"quote\" backslash\\ newline\n tab\t bell\u0007")") != std::string::npos);
}

TEST_CASE("Source paths are made repo-relative with forward slashes")
{
    const std::string relative = ct::log::relativeSourcePath(std::source_location::current().file_name());
    CHECK(relative == "tests/unit/log_tests.cpp");
    CHECK(ct::log::relativeSourcePath("C:\\elsewhere\\file.cpp") == "C:/elsewhere/file.cpp");
}
