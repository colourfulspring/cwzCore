project(boot C ASM)

set(CUR_OBJECTS ${PROJECT_NAME}_objs)
set(CUR_NAME ${PROJECT_NAME})

# 编译启动块的各个目标文件
file(GLOB cur_src "src/*.S")
add_library(${CUR_OBJECTS} OBJECT ${cur_src})

# 添加最小编译选项
set_minimal_compile_options(${CUR_OBJECTS} PRIVATE)

# 设置优化选项和32位编译
target_compile_options(${CUR_OBJECTS} PRIVATE -Os -m32)

# 设置输出文件名
set(LINK_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CUR_NAME}.o)        # 链接输出
set(OBJCOPY_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CUR_NAME}.bin)   # 提纯输出

# 处理各的目标文件
add_custom_command(OUTPUT ${LINK_OUTPUT} ${OBJCOPY_OUTPUT}
    # 手动链接
    COMMAND ${CMAKE_LINKER}
            -m elf_i386 -nostdlib -N
            -T ${CMAKE_CURRENT_SOURCE_DIR}/${CUR_NAME}.ld
            $<TARGET_OBJECTS:${CUR_OBJECTS}>
            -o ${LINK_OUTPUT}
    
    # 使用objcopy将目标变成纯二进制
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${LINK_OUTPUT} ${OBJCOPY_OUTPUT}

    DEPENDS ${CUR_OBJECTS}
    COMMAND_EXPAND_LISTS
)

# 设置目标依赖刚才扩充完的文件
add_custom_target(${BOOT_BLOCK_TARGET} ALL DEPENDS ${OBJCOPY_OUTPUT})
set_target_properties(${BOOT_BLOCK_TARGET} PROPERTIES OUTPUT_FILE ${OBJCOPY_OUTPUT})