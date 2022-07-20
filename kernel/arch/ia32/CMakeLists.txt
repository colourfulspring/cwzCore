include(KernelTools)

project(boot C ASM)

# 编译启动块的各个目标文件
set(BOOT_OBJECTS boot_objects)
add_subdirectory(boot)

# 手动链接
set(LINK_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bootblock.o)
add_custom_command(OUTPUT ${LINK_OUTPUT}
    COMMAND ${CMAKE_LINKER}
            -m elf_i386 -nostdlib -N -T ${CMAKE_CURRENT_SOURCE_DIR}/boot.ld
            $<TARGET_OBJECTS:${BOOT_OBJECTS}>
            -o ${LINK_OUTPUT}
    DEPENDS ${BOOT_OBJECTS}
    COMMAND_EXPAND_LISTS
)
add_custom_target(link_target DEPENDS ${LINK_OUTPUT})

# 使用objcopy将目标变成纯二进制
set(OBJCOPY_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bootblock.out)
add_custom_command(OUTPUT ${OBJCOPY_OUTPUT}
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${LINK_OUTPUT} ${OBJCOPY_OUTPUT}
    DEPENDS link_target
)
add_custom_target(objcopy_target DEPENDS ${OBJCOPY_OUTPUT})

# 使用sign工具扩充到512字节
set(SIGN_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bootblock.bin)
add_custom_command(OUTPUT ${SIGN_OUTPUT}
    COMMAND $<TARGET_FILE:sign> ${OBJCOPY_OUTPUT} ${SIGN_OUTPUT}
    DEPENDS objcopy_target
)

# 设置boot目标依赖刚才扩充完的文件
add_custom_target(${PROJECT_NAME} DEPENDS ${SIGN_OUTPUT})

# 设置boot目标依赖tools
add_dependencies(${PROJECT_NAME} tools)

# 设置kernel目标依赖boot目标
add_dependencies(${KERNEL_TARGET} ${PROJECT_NAME})