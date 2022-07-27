file(GLOB_RECURSE libc_src "include/*.h" "src/*.c")

# 添加源文件到主目标
target_sources(${KERNEL_TARGET} PRIVATE ${libc_src})

# 添加头文件搜索路径
target_include_directories(${KERNEL_TARGET} PUBLIC include)