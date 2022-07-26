#include <asm.h>

# 数据段：64位临时描述符表
.section .data
label_gdt64:
    SEG_NULLASM64
label_desc_code64:
    SEG_ASM64(STA_X | STA_R, 0x0, 0x0)
label_desc_data64:
    SEG_ASM64_DATA(STA_W, 0x0, 0x0)

.set gdt_len64, 0x18
.set selector_code64, label_desc_code64 - label_gdt64 # 64位代码段选择子
.set selector_data64, label_desc_data64 - label_gdt64 # 64位数据段选择子

gdtdesc64:
    .word gdt_len64 - 1;
    .int  label_gdt64

# 代码段：
.section .text

.global _start
_start:
    // 临时页表地址 0x90000
    movl $0x91007, 0x90000 // PML4表项
    movl $0x91007, 0x90800 // 2048 ?
    
    movl $0x92007, 0x91000 // PDPT表项

    movl $0x000083, 0x92000 // PDT表项（2MB页）
    movl $0x200083, 0x92008
    movl $0x400083, 0x92010
    movl $0x600083, 0x92018
    movl $0x800083, 0x92020
    movl $0xa00083, 0x92028

    // 修改全局描述符表寄存器
    .byte 0x66
    lgdt gdtdesc64
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %ds
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    // 启用 PAE
    movl %cr4, %eax
    btsl $5, %eax     // bts eax, 5 ?
    movl %eax, %cr4

    // 修改页表基地址寄存器
    movl $0x90000, %eax
    movl %eax, %cr3

    // 打开长模式
    movl $0xc0000080, %ecx      // IA32_EFER
    rdmsr

    btsl $8, %eax
    wrmsr

    // 启用分页
    movl %cr0, %eax
    btsl $0, %eax
    btsl $31, %eax
    movl %eax, %cr0

    # 跳转到64位模式
    ljmp $selector_code64, $_start64


.code64                                     # 声明64位模式
_start64:
    // 设置栈地址
    mov $stack_top, %rsp
    // 栈地址按照 4096 字节对齐
    and $0xFFFFFFFFFFFFF000, %rsp
    // 帧指针修改为 0
    mov $0, %rbp
    # 初始化 C++
    call loader_main
    hlt
    ret

.section .bss
stack:
    .skip 16384

stack_top:
