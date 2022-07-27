.code16                                             # 声明16位模式

# 代码段：准备
.section .text                                      # 声明代码段
.global start

.set ShitHappensCodePos, ShitHappens + 0x06


ZeroTheSegmentRegister:
  xor %ax, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %ss

// 栈空间位于0x7c00及往前的空间，栈顶在0x7c00
SetupTheStackPointer:
  mov $0x7c00, %sp

start:
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

// 寻找MBR分区表中的活动分区，看分区项第一个字节是否为0x80，最多4个分区
SeekTheActivePartition:
  // 分区表位于0x7c00+446=0x7c00+0x1be=0x7dbe的位置，使用di作为基地址
  mov $0x7dbe, %di
//  mov $4, %cx
//   isActivePartition:
//     mov [%di], %bl
//     cmp $0x80, %bl 
//     // 如果是则说明找到了，使用jmp if equel指令跳转继续
//     je ActivePartitionFound
//     // 如果没找到，则继续寻找下一个分区项，si+16
//     add $16, %di
//     loop isActivePartition
//   ActivePartitionNotFound:
//     movb $0x32, ShitHappensCodePos
//     jmp BootLoaderEnd
// 找到活动分区后，di目前就是活动分区项的首地址
ActivePartitionFound:
  mov $PartitionFound, %si
  call PrintString
  // ebx保存活动分区的起始地址
  mov 8(%di), %ebx
  movl %ebx, BlockLow
  // 目标内存起始地址
  movw $0x7e00, BufferOffset
  movb $1, BlockCount
  // 读取第一个扇区
  call ReadDisk
GetFirstFat:
  mov $0x7e00, %di
  // ebx目前为保留扇区数
  xor %ebx, %ebx
  mov 0x0e(%di), %bx
  // FirstFat起始扇区号=隐藏扇区+保留扇区
  mov %eax, 0x1c(%di)
  add %eax, %ebx
// 获取数据区起始区扇区号
GetDataAreaBase:
  mov 0x24(%di), %eax
  xor %cx, %cx
  mov 0x10(%di), %cl 
  AddFatSize:
    add %eax, %ebx
    loop AddFatSize
// 读取数据区8个扇区/1个簇
ReadRootDirectory:
  mov %ebx, BlockLow
  movw $0x8000, BufferOffset 
  mov $0x8000, %di 
  movb $8, BlockCount 
  call ReadDisk
  movb $0x34, ShitHappensCodePos

str1:
.set STR_LOAD, 0x4c4f4144
str2:
.set STR_ER__, 0x45522020
str3:
.set set_ELF_, 0x454c4620

SeekTheInitialBin:
  cmpl $STR_LOAD, (%di)
  jne nextFile
  cmpl $STR_ER__, 4(%di)
  jne nextFile
  cmpl $set_ELF_, 8(%di)
  jne nextFile
  jmp InitialBinFound
  nextFile:
    cmp $0x9000, %di
    ja BootLoaderEnd
    add $32, %di
    jmp SeekTheInitialBin

InitialBinFound:
  mov InitialFound, %si 
  call PrintString
  // 获取文件长度
  mov 0x1c(%di), %ax
  mov 0x1e(%di), %dx
  // 文件长度是字节为单位的，需要先除以512得到扇区数
  mov $512, %cx
  div %cx
  // 如果余数不为0，则需要多读一个扇区
  cmp $0, %dx
  je NoRemainder
  // ax是要读取的扇区数
  inc %ax
  mov %ax, BlockCount 
  NoRemainder:
    // 文件起始簇号，也是转为扇区号，乘以8即可
    mov 0x1a(%di), %ax
    sub $2, %ax 
    mov $8, %cx 
    mul %cx
    // 现在文件起始扇区号存在dx:ax，直接保存到ebx，这个起始是相对于DataBase 0x32,72
    // 所以待会计算真正的起始扇区号还需要加上DataBase
    and $0x0000ffff, %eax
    add %eax, %ebx
    mov %dx, %ax
    shl $16, %eax
    add %eax, %ebx
    mov %ebx, (BlockLow) 
    movw $0x9000, (BufferOffset)
    mov $0x9000, %di 
    call ReadDisk
    // 跳转到Initial.bin继续执行
    mov $GotoInitial, %si
    call PrintString
    jmp *%di

ReadDisk:
  mov $0x42, %ah 
  mov $0x80, %dl 
  mov $DiskAddressPacket, %si
  int $0x13
  test %ah, %ah
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
PartitionFound:
  .ascii "Get Partition!"
  .word 0x0d, 0x0a
InitialFound:
  .ascii "Get Initial!"
  .word 0x0d, 0x0a
GotoInitial:
  .ascii "Go to Initial!"
  .word 0x0d, 0x0a
ShitHappens:
  .ascii "Error 0, Shit happens, check your code!"
  .word 0x0d, 0x0a

// 结束为止
//   times 446-($-$$) db 0