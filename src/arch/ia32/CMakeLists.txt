file(GLOB port_src "port/*.h" "port/*.c")

target_sources(${KERNEL_TARGET} PRIVATE init.c start.S ${port_src})

target_include_directories(${KERNEL_TARGET} PRIVATE port)

# 添加链接脚本
target_link_options(${KERNEL_TARGET} PRIVATE -T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)

# 添加 ld 参数
target_link_options(${KERNEL_TARGET} PRIVATE -Wl,-melf_x86_64 -Wl,--no-relax)

# ----------------------------------------------------------------------------------------------------
# 两个独立目标

add_subdirectory(boot)