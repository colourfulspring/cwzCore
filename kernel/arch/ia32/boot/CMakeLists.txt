project(boot C ASM)

# 编译启动块的各个目标文件
set(BOOT_OBJECTS boot_objects)

add_library(${BOOT_OBJECTS} OBJECT init/boot.S init/main.c)
target_include_directories(${BOOT_OBJECTS} PRIVATE include)
target_compile_options(${BOOT_OBJECTS} PRIVATE
                    -fno-builtin -fno-PIC -Wall
                    -nostdinc -fno-stack-protector -Os -c)

# 设置输出文件名
set(LINK_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bootblock.o)        # 链接输出
set(OBJCOPY_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bootblock.out)   # 提纯输出
set(SIGN_OUTPUT ${CMAKE_BINARY_DIR}/bootblock.bin)              # 扩充输出

# 处理各的目标文件
add_custom_command(OUTPUT ${LINK_OUTPUT} ${OBJCOPY_OUTPUT} ${SIGN_OUTPUT}
    # 手动链接
    COMMAND ${CMAKE_LINKER}
            -m elf_i386 -nostdlib -N -T ${CMAKE_CURRENT_SOURCE_DIR}/boot.ld
            $<TARGET_OBJECTS:${BOOT_OBJECTS}>
            -o ${LINK_OUTPUT}
    
    # 使用objcopy将目标变成纯二进制
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${LINK_OUTPUT} ${OBJCOPY_OUTPUT}
    
    # 使用sign工具扩充到512字节
    COMMAND $<TARGET_FILE:sign> ${OBJCOPY_OUTPUT} ${SIGN_OUTPUT}

    DEPENDS ${BOOT_OBJECTS}
    COMMAND_EXPAND_LISTS
)

# 设置boot目标依赖刚才扩充完的文件
add_custom_target(${BOOT_BLOCK_TARGET} ALL DEPENDS ${SIGN_OUTPUT})
set_target_properties(${BOOT_BLOCK_TARGET} PROPERTIES OUTPUT_FILE ${SIGN_OUTPUT})

# 设置boot目标依赖tools
add_dependencies(${BOOT_BLOCK_TARGET} ${TOOLS_TARGET})