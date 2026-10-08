# Editor logic lives in a library so tests can link it; the executable is a thin main().
add_library(ct_editor_lib STATIC
    src/options.cpp
)
add_library(ct::editor_lib ALIAS ct_editor_lib)
target_include_directories(ct_editor_lib PUBLIC include)
target_link_libraries(ct_editor_lib PUBLIC ct::core)
ct_target_options(ct_editor_lib)

add_executable(ct_editor src/main.cpp)
target_link_libraries(ct_editor PRIVATE ct::editor_lib)
ct_target_options(ct_editor)
