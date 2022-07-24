add_subdirectory(boot)

target_sources(${KERNEL_TARGET} PRIVATE init.c start.S)

target_link_options(${KERNEL_TARGET} PRIVATE -T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)
target_link_options(${KERNEL_TARGET} PRIVATE -Wl,-melf_x86_64 -Wl,--no-relax)