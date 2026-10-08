#pragma once

#include <string_view>

namespace ct
{

/// Engine version from the top-level CMake project, e.g. "0.1.0".
std::string_view engineVersion();

} // namespace ct
