#include "core/assert.h"

#include "core/log.h"

#include <cstdlib>

namespace ct::detail
{

void assertFailed(std::string_view expression, std::string_view message, std::source_location location)
{
    const std::string text = message.empty() ? std::format("Check failed: {}", expression)
                                             : std::format("Check failed: {} ({})", expression, message);
    log::write(log::Level::Fatal, "assert", text, location);
    std::abort();
}

} // namespace ct::detail
