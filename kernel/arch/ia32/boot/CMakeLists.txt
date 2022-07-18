set(BOOT_ASM boot_asm)
set(BOOT_C boot_c)

include_directories(include)
add_compile_options(-fno-builtin -fno-PIC -Wall 
                    -nostdinc -fno-stack-protector -Os -c)

add_library(${BOOT_ASM} OBJECT init/boot.S)
add_library(${BOOT_C} OBJECT init/main.c)

set(BOOT_ASM ${BOOT_ASM} PARENT_SCOPE)
set(BOOT_C ${BOOT_C} PARENT_SCOPE)