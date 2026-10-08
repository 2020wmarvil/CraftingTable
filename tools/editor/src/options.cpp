#include "editor/options.h"

#include <format>

namespace ct::editor
{

namespace
{

Expected<log::Format> parseLogFormat(std::string_view value)
{
    if (value == "text") {
        return log::Format::Text;
    }
    if (value == "json") {
        return log::Format::Json;
    }
    return makeError(ErrorCode::InvalidArgument,
                     std::format("Invalid --log-format '{}' (expected text or json)", value));
}

Expected<log::Level> parseLogLevel(std::string_view value)
{
    for (const log::Level level : {log::Level::Trace, log::Level::Debug, log::Level::Info, log::Level::Warn,
                                   log::Level::Error, log::Level::Fatal}) {
        if (value == log::levelName(level)) {
            return level;
        }
    }
    return makeError(
        ErrorCode::InvalidArgument,
        std::format("Invalid --log-level '{}' (expected trace, debug, info, warn, error, or fatal)", value));
}

} // namespace

Expected<Options> parseOptions(std::span<const std::string_view> args)
{
    constexpr std::string_view kLogFormatPrefix = "--log-format=";
    constexpr std::string_view kLogLevelPrefix = "--log-level=";

    Options options;
    for (const std::string_view arg : args) {
        if (arg == "--headless") {
            options.isHeadless = true;
        } else if (arg == "--version") {
            options.shouldPrintVersion = true;
        } else if (arg == "--help" || arg == "-h") {
            options.shouldPrintHelp = true;
        } else if (arg.starts_with(kLogFormatPrefix)) {
            auto format = parseLogFormat(arg.substr(kLogFormatPrefix.size()));
            if (!format) {
                return std::unexpected(std::move(format.error()));
            }
            options.logFormat = *format;
        } else if (arg.starts_with(kLogLevelPrefix)) {
            auto level = parseLogLevel(arg.substr(kLogLevelPrefix.size()));
            if (!level) {
                return std::unexpected(std::move(level.error()));
            }
            options.logLevel = *level;
        } else {
            return makeError(ErrorCode::InvalidArgument, std::format("Unknown argument '{}'", arg));
        }
    }
    return options;
}

std::string_view usage()
{
    return "Usage: ct_editor [options]\n"
           "\n"
           "Options:\n"
           "  --headless            Run without a window (required until the windowed editor lands in M2)\n"
           "  --log-format=FORMAT   text (default) or json; logs go to stderr\n"
           "  --log-level=LEVEL     trace, debug, info (default), warn, error, fatal\n"
           "  --version             Print version info as JSON to stdout and exit\n"
           "  -h, --help            Print this help and exit\n";
}

} // namespace ct::editor
