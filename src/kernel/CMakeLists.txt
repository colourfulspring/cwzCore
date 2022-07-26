set(kernel_src io.c main.c)

# 添加源文件到主目标
target_sources(${KERNEL_TARGET} PRIVATE ${kernel_src})