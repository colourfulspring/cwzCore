# 添加loader目标
set(LOADER_OBJECTS loader_objects)

# file(GLOB loader_src src/main.c src/loader2.S)
# set(loader_src src/loader2.S src/main.c)
set(loader_src src/loader2.S)

add_library(${LOADER_OBJECTS} OBJECT ${loader_src})

# 添加最小编译选项
set_minimal_compile_options(${LOADER_OBJECTS} PRIVATE)

# 设置优化选项
target_compile_options(${LOADER_OBJECTS} PRIVATE -Os)

# 设置输出文件名
set(LINK_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/loader.o)        # 链接输出
set(OBJCOPY_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/loader.bin)   # 提纯输出

# 处理各的目标文件
add_custom_command(OUTPUT ${LINK_OUTPUT} ${OBJCOPY_OUTPUT}
    # 手动链接
    COMMAND ${CMAKE_LINKER}
            -m elf_x86_64 -nostdlib -N -e start -Ttext 0x9000
            $<TARGET_OBJECTS:${LOADER_OBJECTS}>
            -o ${LINK_OUTPUT}
    
    # 使用objcopy将目标变成纯二进制
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${LINK_OUTPUT} ${OBJCOPY_OUTPUT}

    DEPENDS ${LOADER_OBJECTS}
    COMMAND_EXPAND_LISTS
)

# 设置loader目标依赖刚才扩充完的文件
add_custom_target(${LOADER_TARGET} ALL DEPENDS ${OBJCOPY_OUTPUT})
set_target_properties(${LOADER_TARGET} PROPERTIES OUTPUT_FILE ${OBJCOPY_OUTPUT})