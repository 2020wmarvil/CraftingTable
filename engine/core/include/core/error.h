#pragma once

#include <cstdint>
#include <expected>
#include <string>
#include <string_view>
#include <utility>

namespace ct
{

/// Broad category of a recoverable failure. The message carries the specifics.
enum class ErrorCode : uint8_t
{
    Unknown,
    InvalidArgument,
    NotFound,
    Io,
    Unsupported,
};

/// Lowercase snake_case name of a code, as used in JSON output ("invalid_argument", ...).
std::string_view errorCodeName(ErrorCode code);

/// A recoverable failure. Returned through Expected, never thrown.
struct Error
{
    ErrorCode code = ErrorCode::Unknown;
    std::string message;
};

/// Result of a fallible operation. Mark functions returning it [[nodiscard]].
template <typename T>
using Expected = std::expected<T, Error>;

/// Convenience for `return makeError(ErrorCode::NotFound, "...");` from a function returning Expected.
inline std::unexpected<Error> makeError(ErrorCode code, std::string message)
{
    return std::unexpected(Error{.code = code, .message = std::move(message)});
}

} // namespace ct
