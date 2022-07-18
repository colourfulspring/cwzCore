set(drivers_src tui/port.c tui/tui.c io.c)

target_sources(${KERNEL_TARGET} PRIVATE ${drivers_src})

target_include_directories(${KERNEL_TARGET} PUBLIC "tui/include")