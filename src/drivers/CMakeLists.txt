set(drivers_src tui/tui.c)

# 添加源文件到主目标
target_sources(${KERNEL_TARGET} PRIVATE ${drivers_src})

# 添加头文件搜索路径
target_include_directories(${KERNEL_TARGET} PUBLIC "tui/include")