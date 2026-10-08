#pragma once

#include <format>
#include <source_location>
#include <string>
#include <string_view>
#include <utility>

namespace ct::detail
{

/// Logs a fatal record describing the failed check, then aborts.
[[noreturn]] void assertFailed(std::string_view expression, std::string_view message, std::source_location location);

inline std::string assertMessage()
{
    return {};
}

template <typename... Args>
std::string assertMessage(std::format_string<Args...> format, Args&&... args)
{
    return std::format(format, std::forward<Args>(args)...);
}

} // namespace ct::detail

/// Always-on check for programmer errors. Optional message: CT_VERIFY(x > 0, "x was {}", x).
#define CT_VERIFY(condition, ...)                                                                                      \
    do {                                                                                                               \
        if (!(condition)) [[unlikely]] {                                                                               \
            ::ct::detail::assertFailed(#condition, ::ct::detail::assertMessage(__VA_ARGS__),                           \
                                       std::source_location::current());                                               \
        }                                                                                                              \
    } while (false)

/// Debug-only check for programmer errors. The condition is not evaluated in release builds.
#ifdef NDEBUG
#define CT_ASSERT(condition, ...)                                                                                      \
    do {                                                                                                               \
        (void)sizeof(!(condition));                                                                                    \
    } while (false)
#else
#define CT_ASSERT(condition, ...) CT_VERIFY(condition, __VA_ARGS__)
#endif
