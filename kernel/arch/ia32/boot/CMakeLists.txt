include_directories(include)

add_compile_options(-march=i686 -fno-builtin -fno-PIC -Wall
                    -nostdinc -fno-stack-protector -Os -c)

add_library(${BOOT_OBJECTS} OBJECT init/boot.S init/main.c)