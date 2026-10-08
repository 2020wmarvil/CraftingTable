# Each dependency is a git submodule pinned to an exact commit. Only add what the current milestone needs.

if(CT_BUILD_TESTS)
    set(DOCTEST_WITH_TESTS OFF CACHE BOOL "" FORCE)
    set(DOCTEST_NO_INSTALL ON CACHE BOOL "" FORCE)
    set(DOCTEST_USE_STD_HEADERS ON CACHE BOOL "" FORCE)
    add_subdirectory(doctest)
endif()
