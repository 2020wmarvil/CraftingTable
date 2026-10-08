#pragma once

#include "core/error.h"
#include "core/log.h"

#include <span>
#include <string_view>

namespace ct::editor
{

/// Parsed command line for ct_editor.
struct Options
{
    bool isHeadless = false;
    bool shouldPrintVersion = false;
    bool shouldPrintHelp = false;
    log::Format logFormat = log::Format::Text;
    log::Level logLevel = log::Level::Info;
};

/// Parses arguments, excluding the program name. Returns InvalidArgument for unknown flags or bad values.
[[nodiscard]] Expected<Options> parseOptions(std::span<const std::string_view> args);

/// Usage text for --help.
std::string_view usage();

} // namespace ct::editor
