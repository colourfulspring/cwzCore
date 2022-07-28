#include <asm.h>

.set ShitHappensCodePos, ShitHappens + 0x06

.code16                                             # 声明16位模式

# 代码段：准备
.section .text                                      # 声明代码段
.global start

start:
  cli                                               # 关中断

// 清空段寄存器
ZeroTheSegmentRegister:
  xor %ax, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %ss

// 栈空间位于0x7c00及往前的空间，栈顶在0x7c00
SetupTheStackPointer:
  mov $0, %bp
  mov $start, %sp

  // 打印信息
  mov $BootLoaderStart, %si
  call PrintString

// 查看是否支持拓展int 13h
CheckInt13:
  mov $0x41, %ah
  mov $0x55aa, %bx 
  mov $0x80, %dl
  int $0x13
  cmp $0xaa55, %bx
  movb $0x31, ShitHappensCodePos
  jnz BootLoaderEnd

LoadLoader:
	// 块号 1
  movl $1, BlockLow
  // 目标地址 0x7e00
  movw $LOADER_START, BufferOffset
	// 块数 20（10KB）
  movb $LOADER_SECTORS, BlockCount

  // 读取
  call ReadDisk

  // 打印信息
  mov $GotoLoader, %si
  call PrintString

	// 跳转
  mov $LOADER_START, %di
  jmp *%di

// AH=42H 扩展读
// DL=80H 读硬盘
// SI 参数结构体

ReadDisk:
  mov $0x42, %ah
  mov $0x80, %dl
  mov $DiskAddressPacket, %si
  int $0x13
  test %ah, %ah										# AH==0 说明成功
  movb $0x33, ShitHappensCodePos
  jnz BootLoaderEnd
  ret

// 打印以0x0a结尾的字符串
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

BootLoaderEnd:
  mov $ShitHappens, %si 
  call PrintString
  hlt

// 数据段
.section .data

// 使用拓展int 13h读取硬盘的结构体DAP
DiskAddressPacket:
  // 包大小，目前恒等于16/0x10，0x00
PackSize:
  .byte 0x10
  // 保留字节，恒等于0，0x01
Reserved:
  .byte 0
  // 要读取的数据块个数，0x02
BlockCount:
  .word 0
  // 目标内存地址的偏移，0x04
BufferOffset:
  .word 0
  // 目标内存地址的段，让它等于0，0x06
BufferSegment:
  .word 0
  // 磁盘起始绝对地址，扇区为单位，这是低字节部分，0x08
BlockLow:
  .int 0
  // 这是高字节部分，0x0c
BlockHigh:
  .int 0

ImportantTips:

BootLoaderStart: 
  .ascii "Start Booting!"
  .word 0x0d, 0x0a

GotoLoader:
  .ascii "Go to Loader!"
  .word 0x0d, 0x0a

ShitHappens:
  .ascii "Error 0, Shit happens!"
  .word 0x0d, 0x0a