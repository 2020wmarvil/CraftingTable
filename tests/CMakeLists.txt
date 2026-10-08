include("${PROJECT_SOURCE_DIR}/third_party/doctest/scripts/cmake/doctest.cmake")

add_executable(ct_unit_tests
    unit/error_tests.cpp
    unit/log_tests.cpp
    unit/editor_options_tests.cpp
)
target_link_libraries(ct_unit_tests PRIVATE ct::core ct::editor_lib doctest::doctest_with_main)
ct_target_options(ct_unit_tests)
doctest_discover_tests(ct_unit_tests)

# Smoke tests: run the editor executable end to end.
add_test(NAME smoke.editor_headless COMMAND ct_editor --headless --log-format=json)
add_test(NAME smoke.editor_version COMMAND ct_editor --version)
set_tests_properties(smoke.editor_version PROPERTIES PASS_REGULAR_EXPRESSION [[^\{"name":"ct_editor","version":"[0-9.]+"\}]])
add_test(NAME smoke.editor_requires_headless COMMAND ct_editor)
set_tests_properties(smoke.editor_requires_headless PROPERTIES WILL_FAIL TRUE)
add_test(NAME smoke.editor_rejects_unknown_flag COMMAND ct_editor --headless --bogus)
set_tests_properties(smoke.editor_rejects_unknown_flag PROPERTIES WILL_FAIL TRUE)
