#include "core/error.h"

#include <doctest/doctest.h>

namespace
{

ct::Expected<int> parsePositive(int value)
{
    if (value <= 0) {
        return ct::makeError(ct::ErrorCode::InvalidArgument, "must be positive");
    }
    return value;
}

} // namespace

TEST_CASE("Expected carries a value on success")
{
    const auto result = parsePositive(3);
    REQUIRE(result.has_value());
    CHECK(*result == 3);
}

TEST_CASE("Expected carries an Error on failure")
{
    const auto result = parsePositive(-1);
    REQUIRE_FALSE(result.has_value());
    CHECK(result.error().code == ct::ErrorCode::InvalidArgument);
    CHECK(result.error().message == "must be positive");
}

TEST_CASE("ErrorCode names are snake_case")
{
    CHECK(ct::errorCodeName(ct::ErrorCode::InvalidArgument) == "invalid_argument");
    CHECK(ct::errorCodeName(ct::ErrorCode::NotFound) == "not_found");
}
