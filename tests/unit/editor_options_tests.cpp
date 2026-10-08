#include "editor/options.h"

#include <doctest/doctest.h>

#include <array>

TEST_CASE("No arguments gives defaults")
{
    const auto options = ct::editor::parseOptions({});
    REQUIRE(options.has_value());
    CHECK_FALSE(options->isHeadless);
    CHECK(options->logFormat == ct::log::Format::Text);
    CHECK(options->logLevel == ct::log::Level::Info);
}

TEST_CASE("Flags are parsed")
{
    constexpr std::array<std::string_view, 3> kArgs = {"--headless", "--log-format=json", "--log-level=debug"};
    const auto options = ct::editor::parseOptions(kArgs);
    REQUIRE(options.has_value());
    CHECK(options->isHeadless);
    CHECK(options->logFormat == ct::log::Format::Json);
    CHECK(options->logLevel == ct::log::Level::Debug);
}

TEST_CASE("Unknown arguments are rejected")
{
    constexpr std::array<std::string_view, 1> kArgs = {"--bogus"};
    const auto options = ct::editor::parseOptions(kArgs);
    REQUIRE_FALSE(options.has_value());
    CHECK(options.error().code == ct::ErrorCode::InvalidArgument);
}

TEST_CASE("Invalid values are rejected")
{
    constexpr std::array<std::string_view, 1> kArgs = {"--log-format=xml"};
    const auto options = ct::editor::parseOptions(kArgs);
    REQUIRE_FALSE(options.has_value());
    CHECK(options.error().message.find("xml") != std::string::npos);
}
