# Compiler settings shared by every CraftingTable target. Third-party targets do not use this.
function(ct_target_options target)
    set_target_properties(${target} PROPERTIES COMPILE_WARNING_AS_ERROR ON)

    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /permissive-
            /utf-8
            /Zc:preprocessor
            /Zc:__cplusplus
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
        )
    endif()
endfunction()
