add_subdirectory(boot)

target_sources(${KERNEL_TARGET} PRIVATE init.c start.S)