#include <asm.h>

.set CR0_PE_ON,             0x1                     # 保护模式标识

.code16                                             # 声明16位模式

# 代码段：准备
.section .text                                      # 声明代码段

.global start
start:
	call ClearScreen

    mov $LoaderStart, %si
    call PrintString

loop1:
    jmp loop1

    # 关中断
    cli

    # 段寄存器清零
    xorw %ax, %ax                                   # Segment number zero

    # 为了兼容早期设备，需要打开A20
    push %ax
    mov $0x2401, %ax
    int $0x15 
    pop %ax


// # 数据段：临时描述符表
// .section .data
// label_gdt:
//     SEG_NULLASM
// label_desc_code32:
//     SEG_ASM(STA_X | STA_R, 0x0, 0xffffffff) # 可执行/可读，基地址0x0，段长度0xFFFFF
// label_desc_data32:
//     SEG_ASM(STA_W, 0x0, 0xffffffff)         # 可写，基地址0x0，段长度0xFFFFF

// .set gdt_len, 0x18
// .set selector_code32, label_desc_code32 - label_gdt # 32位代码段选择子
// .set selector_data32, label_desc_data32 - label_gdt # 32位数据段选择子

// gdtdesc:
//     .word gdt_len - 1;  # 全局描述符表边界（数值上等于表长-1）
//     .int  label_gdt     # 全局描述符表线性基地址

// # 代码段：跳转到32位
// .section .text          # 代码段
//     .byte 0x66          # why does OSbook have this line but ucore doesn't have? Experiment shows no different
//     lgdt gdtdesc
//     movl %cr0, %eax
//     orl $CR0_PE_ON, %eax
//     movl %eax, %cr0     # 从实模式进入保护模式

//     # 跳转到32位模式
//     ljmp $selector_code32, $protcseg



// .code32                                             # 声明32位模式
// .extern main
// protcseg:
//     # Set up the protected-mode data segment registers
//     movw $selector_data32, %ax                      # Our data segment selector
//     movw %ax, %ds                                   # -> DS: Data Segment
//     movw %ax, %es                                   # -> ES: Extra Segment
//     movw %ax, %fs                                   # -> FS
//     movw %ax, %gs                                   # -> GS
//     movw %ax, %ss                                   # -> SS: Stack Segment

//     // movl $0x0, %ebp                          # 帧指针
//     // movl $0x7c00, %esp                       # 栈指针（栈空间0~0x7c00）
//     // call main

//     # If bootmain returns (it shouldn't), loop.
// spin:
//     jmp spin

// 开启A20地址线后，暂时打开保护模式，让FS寄存器具有4GB寻址功能 代码书P57

// 使用封装的Func_ReadOneSector功能把kernel.bin存入内存0x7E00
// int $0x13 用法：
// %ax: 待读取的磁盘起始LBA扇区号：1  除了前面有一个引导扇区（扇区号为0）外
// %cl: 读入的扇区数量：？  ？与kernel.bin的大小有关，前提是kernel.bin不在文件系统里。
// %es:%bx：目标缓冲区起始地址：代码书P59，宏书P56 
// 
// 再把内核从0x7E00移动到0x10000。代码P60
// 
















// 打印以0x0a结尾的字符串
.global PrintString
PrintString:
  push %ax
  push %cx
  push %si
  mov $512, %cx

  PrintChar:
    mov (%si), %al
    mov $0x0e, %ah
    int $0x10
    cmpb $0x0a,(%si)
    je Return
    inc %si
    loop PrintChar
	
  Return:
    pop %si
    pop %cx
    pop %ax
    ret

.global ClearScreen
ClearScreen:
  // 清屏
  mov $0x600, %ax
  mov $0x700, %bx
  mov $0x0, %cx
  mov $0x184F, %dx
  int $0x10

  // 指针复位
  mov $0x200, %ax
  mov $0x0, %bx
  mov $0x0, %dx
  int $0x10
  ret

.global TestPrint
TestPrint:
  mov $TextMessage, %si
  call PrintString
  ret

LoaderStart: 
  .ascii "Start Loader!"
  .word 0x0d, 0x0a

TextMessage: 
  .ascii "Test Message!"
  .word 0x0d, 0x0a