set(BOOT_ASM boot_asm)
set(BOOT_C boot_c)

add_library(${BOOT_ASM} OBJECT init/boot.S)
add_library(${BOOT_C} OBJECT init/main.c)

target_include_directories(${BOOT_ASM} PRIVATE include)
target_include_directories(${BOOT_C} PRIVATE include)

target_compile_options(${BOOT_ASM} PRIVATE
    -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Os -nostdinc -c
)

target_compile_options(${BOOT_C} PRIVATE
    -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Os -nostdinc -c
)

set(BOOT_ASM ${BOOT_ASM} PARENT_SCOPE)
set(BOOT_C ${BOOT_C} PARENT_SCOPE)