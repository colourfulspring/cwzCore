file(GLOB_RECURSE libc_src "include/*.h" "src/*.c")

target_sources(${KERNEL_TARGET} PRIVATE ${libc_src})

target_include_directories(${KERNEL_TARGET} PUBLIC include)