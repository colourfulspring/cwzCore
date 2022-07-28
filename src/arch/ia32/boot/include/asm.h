#ifndef __BOOT_ASM_H__
#define __BOOT_ASM_H__

/* Assembler macros to create x86 segments */

/* Normal segment */
#define SEG_NULLASM \
    .word 0, 0;     \
    .byte 0, 0, 0, 0

#define SEG_ASM(type, base, lim)                     \
    .word(((lim) >> 12) & 0xffff), ((base) &0xffff); \
    .byte(((base) >> 16) & 0xff), (0x90 | (type)), (0xC0 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)

/* Application segment type bits */
#define STA_X 0x8 // Executable segment
#define STA_E 0x4 // Expand down (non-executable segments)
#define STA_C 0x4 // Conforming code segment (executable only)
#define STA_W 0x2 // Writeable (non-executable segments)
#define STA_R 0x2 // Readable (executable segments)
#define STA_A 0x1 // Accessed

/* Assembler macros to create x64 segments */

#define SEG_NULLASM64 SEG_NULLASM

#define SEG_ASM64(type, base, lim)                   \
    .word(((lim) >> 12) & 0xffff), ((base) &0xffff); \
    .byte(((base) >> 16) & 0xff), (0x90 | (type)), (0x20 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)

#define SEG_ASM64_DATA(type, base, lim)              \
    .word(((lim) >> 12) & 0xffff), ((base) &0xffff); \
    .byte(((base) >> 16) & 0xff), (0x90 | (type)), (0x00 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)

#define BOOT_START 0x7C00
#define LOADER_START 0x7E00

#define LOADER_SECTORS 20

#endif /* !__BOOT_ASM_H__ */
