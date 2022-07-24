// gdt_table
.section .data
.globl gdt_table

gdt_table:
    .quad 0x0000000000000000    // 0 NULL descriptor            00
    .quad 0x0020980000000000    // 1 KERNEL Code 64-bit Segment 08
    .quad 0x0000920000000000    // 2 KERNEL Data 64-bit Segment 10
    .quad 0x0020f80000000000    // 3 USER Code 64-bit Segment   18
    .quad 0x0000f20000000000    // 4 USER Data 64-bit Segment   20
    .quad 0x00cf9a000000ffff    // 5 KERNEL Code 32-bit Segment 28
    .quad 0x00cf92000000ffff    // 6 KERNEL Data 32-bit Segment 30
    .fill 10, 8, 0              // 8~9 TSS (jump one segment<7>) in long-mode 128-bit
gdt_end:

gdt_pointer:
gdt_limit:
    .word gdt_end - gdt_table - 1
gdt_base:
    .quad gdt_table

.section .text
.globl _start
_start:
    // ...
    call init_c