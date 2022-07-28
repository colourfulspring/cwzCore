project(kernel C ASM)

# ----------------------------------------------------------------------------------------------------
# 函数和宏定义

# 添加最小编译宏定义与编译参数
function(set_minimal_compile_options _target _scope)
    set(_compile_options)
    set(_asm_compile_options)
    set(_c_compile_options -nostdinc -ffreestanding -fPIC -Wall -fno-stack-protector -fno-builtin
                        -Werror-implicit-function-declaration)

    foreach(_option ${_compile_options})
        target_compile_options(${_target} ${_scope} ${_option})
    endforeach()

    foreach(_option ${_asm_compile_options})
        target_compile_options(${_target} ${_scope} $<$<COMPILE_LANGUAGE:ASM>:${_option}>)  
    endforeach()

    foreach(_option ${_c_compile_options})
        target_compile_options(${_target} ${_scope} $<$<COMPILE_LANGUAGE:C>:${_option}>)  
    endforeach()
endfunction()

# ----------------------------------------------------------------------------------------------------
# 其他目标

# 添加子目录
add_subdirectory(tools)


# ----------------------------------------------------------------------------------------------------
# kernel 目标

# 添加目标
add_executable(${KERNEL_TARGET})
set_target_properties(${KERNEL_TARGET} PROPERTIES OUTPUT_NAME "${KERNEL_TARGET}.elf")

# 添加子目录
add_subdirectory(arch)
add_subdirectory(drivers)
add_subdirectory(fs)
add_subdirectory(libc)
add_subdirectory(kernel)

# 添加头文件搜索路径
target_include_directories(${KERNEL_TARGET} PRIVATE
    include
)

# 添加链接选项
target_link_options(${KERNEL_TARGET} PRIVATE
    -no-pie -nostdlib -nostartfiles
)

# 添加最小编译选项
set_minimal_compile_options(${KERNEL_TARGET} PRIVATE)

# ----------------------------------------------------------------------------------------------------
# 最终目标

get_target_property(BOOT_BLOCK_FILE ${BOOT_BLOCK_TARGET} OUTPUT_FILE)
get_target_property(LOADER_FILE ${LOADER_TARGET} OUTPUT_FILE)
set(FINAL_OUTPUT ${CMAKE_BINARY_DIR}/scene.img)
add_custom_command(OUTPUT ${FINAL_OUTPUT}
    COMMAND ${CMAKE_COMMAND}
            -D check_size=true
            -D bootblock=${BOOT_BLOCK_FILE}
            -D loader=${LOADER_FILE}
            -D kernel=$<TARGET_FILE:${KERNEL_TARGET}>
            -D mkfat32=$<TARGET_FILE:mkfat32>
            -D dd=${CMAKE_DD}
            -D output=${FINAL_OUTPUT}
            -P ${PROJECT_SCRIPT_PATH}/BuildRootfs.cmake
    DEPENDS ${TOOLS_TARGET} ${BOOT_BLOCK_TARGET} ${LOADER_TARGET} ${KERNEL_TARGET}
)
add_custom_target(${FINAL_TARGET} ALL DEPENDS ${FINAL_OUTPUT})
set_target_properties(${FINAL_TARGET} PROPERTIES OUTPUT_FILE ${FINAL_OUTPUT})