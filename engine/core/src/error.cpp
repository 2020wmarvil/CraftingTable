#include "core/error.h"

namespace ct
{

std::string_view errorCodeName(ErrorCode code)
{
    switch (code) {
    case ErrorCode::Unknown:
        return "unknown";
    case ErrorCode::InvalidArgument:
        return "invalid_argument";
    case ErrorCode::NotFound:
        return "not_found";
    case ErrorCode::Io:
        return "io";
    case ErrorCode::Unsupported:
        return "unsupported";
    }
    return "unknown";
}

} // namespace ct
