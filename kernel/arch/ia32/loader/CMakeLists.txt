# 添加目标
add_executable(${LOADER_TARGET} loader.S main.c)

set_target_properties(${LOADER_TARGET} PROPERTIES OUTPUT_NAME "${LOADER_TARGET}.elf")

# target_compile_options(${LOADER_TARGET} PRIVATE "-m32")