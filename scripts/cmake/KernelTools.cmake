# Output an object list.
macro(os_target_sources_out_objects _target _rel_dir _objects)

    if(NOT CMAKE_CURRENT_BINARY_DIR MATCHES "^${CMAKE_BINARY_DIR}/")
        message(
            FATAL_ERROR
                "CMAKE_CURRENT_BINARY_DIR (${CMAKE_CURRENT_BINARY_DIR}) must be in CMAKE_BINARY_DIR (${CMAKE_BINARY_DIR})."
        )
    endif()

    string(REGEX REPLACE "^${CMAKE_BINARY_DIR}/${_rel_dir}/" "" _curr_bin_rel_path
                         ${CMAKE_CURRENT_BINARY_DIR})

    foreach(_src ${ARGN})
        if(_src MATCHES "\.(c|C)$")
            set(_obj_extension ${CMAKE_C_OUTPUT_EXTENSION})
        elseif(_src MATCHES "\.(s|S)$")
            set(_obj_extension ${CMAKE_ASM_OUTPUT_EXTENSION})
        elseif(_src MATCHES "\.(asm)$")
            set(_obj_extension ${CMAKE_ASM_NASM_OUTPUT_EXTENSION})
        elseif(_src MATCHES "\.(cpp|CPP|cxx|CXX|cc|CC)$")
            set(_obj_extension ${CMAKE_CXX_OUTPUT_EXTENSION})
        else()
            message(FATAL_ERROR "Unsupported file type: ${_src}")
        endif()
        list(
            APPEND
            ${_objects}
            ${_rel_dir}/CMakeFiles/${_target}.dir/${_curr_bin_rel_path}/${_src}${_obj_extension}
        )
    endforeach()

    unset(_obj_extension)
    unset(_curr_bin_rel_path)
endmacro()

# Add target to convert ELF kernel to binary image.
function(os_objcopy_binary _kernel_target _binary_name)
    add_custom_target(
        ${_binary_name} ALL
        COMMAND ${CMAKE_OBJCOPY} -O binary -S $<TARGET_FILE:${_kernel_target}>
                ${_binary_name}
        DEPENDS ${_kernel_target})
endfunction()
