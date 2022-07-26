#include <asm.h>

.set CR0_PE_ON,             0x1                     # 保护模式标识



.code16                                             # 声明16位模式

# 代码段：准备
.section .text                                      # 声明代码段
.global start
start:                                              # 起始地址0x7c00
    cli                                             # 关中断

    # 段寄存器清零
    xorw %ax, %ax                                   # Segment number zero
    movw %ax, %ds                                   # -> Data Segment
    movw %ax, %es                                   # -> Extra Segment
    movw %ax, %ss                                   # -> Stack Segment

    # 为了兼容早期设备，需要打开A20
    push %ax
    mov $0x2401, %ax
    int $0x15 
    pop %ax



# 数据段：临时描述符表
.section .data
label_gdt:
    SEG_NULLASM
label_desc_code32:
    SEG_ASM(STA_X | STA_R, 0x0, 0xffffffff) # 可执行/可读，基地址0x0，段长度0xFFFFF
label_desc_data32:
    SEG_ASM(STA_W, 0x0, 0xffffffff)         # 可写，基地址0x0，段长度0xFFFFF

.set gdt_len, 0x18
.set selector_code32, label_desc_code32 - label_gdt # 32位代码段选择子
.set selector_data32, label_desc_data32 - label_gdt # 32位数据段选择子

gdtdesc:
    .word gdt_len - 1;  # 全局描述符表边界（数值上等于表长-1）
    .int  label_gdt     # 全局描述符表线性基地址

# 代码段：跳转到32位
.section .text          # 代码段
    .byte 0x66          # why does OSbook have this line but ucore doesn't have? Experiment shows no different
    lgdt gdtdesc
    movl %cr0, %eax
    orl $CR0_PE_ON, %eax
    movl %eax, %cr0     # 从实模式进入保护模式

    # 跳转到32位模式
    ljmp $selector_code32, $protcseg



.code32                                             # 声明32位模式
protcseg:
    # Set up the protected-mode data segment registers
    movw $selector_data32, %ax                      # Our data segment selector
    movw %ax, %ds                                   # -> DS: Data Segment
    movw %ax, %es                                   # -> ES: Extra Segment
    movw %ax, %fs                                   # -> FS
    movw %ax, %gs                                   # -> GS
    movw %ax, %ss                                   # -> SS: Stack Segment

    movl $0x0, %ebp                         # 帧指针
    movl $start, %esp                       # 栈指针（栈空间0~0x7c00）
    call bootmain

    # If bootmain returns (it shouldn't), loop.
spin:
    jmp spin