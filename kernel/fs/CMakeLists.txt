file(GLOB_RECURSE fs_src "include/*.h" "./*.c")

target_sources(${KERNEL_TARGET} PRIVATE ${fs_src})

target_include_directories(${KERNEL_TARGET} PUBLIC include)