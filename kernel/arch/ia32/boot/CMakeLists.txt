project(boot)

list(APPEND boot_src init/boot.asm init/main.c)

add_library(${PROJECT_NAME} OBJECT ${boot_src})