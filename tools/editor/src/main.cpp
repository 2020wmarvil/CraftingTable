// ct_editor: the CraftingTable editor. Runs headless for agents and (from M2) with an ImGui window for humans.
// Conventions: stdout carries command output (data), stderr carries logs.

#include "core/log.h"
#include "core/version.h"
#include "editor/options.h"

#include <cstdio>
#include <memory>
#include <print>
#include <string_view>
#include <vector>

namespace
{

constexpr int kExitSuccess = 0;
constexpr int kExitUsageError = 2;

} // namespace

int main(int argc, char** argv)
{
    const std::vector<std::string_view> args(argv + 1, argv + argc);
    const ct::Expected<ct::editor::Options> options = ct::editor::parseOptions(args);
    if (!options) {
        std::print(stderr, "error: {}\n\n{}", options.error().message, ct::editor::usage());
        return kExitUsageError;
    }

    if (options->shouldPrintHelp) {
        std::print(stdout, "{}", ct::editor::usage());
        return kExitSuccess;
    }
    if (options->shouldPrintVersion) {
        std::println(stdout, R"({{"name":"ct_editor","version":"{}"}})", ct::engineVersion());
        return kExitSuccess;
    }

    ct::log::setMinLevel(options->logLevel);
    ct::log::addSink(std::make_unique<ct::log::StreamSink>(stderr, options->logFormat));

    if (!options->isHeadless) {
        CT_LOG_ERROR("editor", "Windowed mode is not implemented yet (M2). Run with --headless.");
        return kExitUsageError;
    }

    CT_LOG_INFO("editor", "Starting ct_editor {} (headless)", ct::engineVersion());
    CT_LOG_INFO("editor", "Nothing to do yet; exiting");
    return kExitSuccess;
}
