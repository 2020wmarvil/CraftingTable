add_library(ct_core STATIC
    src/assert.cpp
    src/error.cpp
    src/log.cpp
    src/version.cpp
)
add_library(ct::core ALIAS ct_core)

target_include_directories(ct_core PUBLIC include)
target_compile_definitions(ct_core PRIVATE
    CT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    CT_VERSION_STRING="${PROJECT_VERSION}"
)
ct_target_options(ct_core)
