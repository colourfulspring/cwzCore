# 添加目标
add_executable(${LOADER_TARGET} loader.S main.c)

set_target_properties(${LOADER_TARGET} PROPERTIES OUTPUT_NAME "${LOADER_TARGET}.elf")

# 不链接标准库，不检查栈
target_link_options(${LOADER_TARGET} PUBLIC -no-pie -nostdlib -nostartfiles)