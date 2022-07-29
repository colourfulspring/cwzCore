.code16                                             # 声明16位模式

# 代码段：准备
.section .text                                      # 声明代码段
.global start

ZeroTheSegmentRegister:
  xor %ax, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %ss

// 栈空间位于0x7c00及往前的空间，栈顶在0x7c00
SetupTheStackPointer:
  mov $0, %bp
  mov $0x7c00, %sp

start:
// 查看是否支持拓展int 13h
CheckInt13:
  mov $0x41, %ah
  mov $0x55aa, %bx 
  mov $0x80, %dl
  int $0x13
  cmp $0xaa55, %bx
  jnz BootLoaderEnd

// 寻找MBR分区表中的活动分区，看分区项第一个字节是否为0x80，最多4个分区
SeekTheActivePartition:
  // 分区表位于0x7c00+446=0x7c00+0x1be=0x7dbe的位置，使用di作为基地址
  mov $0x7dbe, %di

ActivePartitionFound:
  // ebx保存活动分区的起始地址(StartLBA)
  mov 8(%di), %ebx
  movl %ebx, BlockLow # 写4字节到BlockLow位置

  // 目标内存起始地址
  movw $0x7e00, BufferOffset
  movb $1, BlockCount

  // 读取第一个扇区
  call ReadDisk

GetFirstFat:
  mov $0x7e00, %di

  // ebx目前为保留扇区数
  xor %ebx, %ebx
  mov 0x0e(%di), %bx    # 0x7e00+14 BPB_ResvdSecCnt

  // FirstFat起始扇区号=隐藏扇区+保留扇区
  mov 0x1c(%di), %eax   # 0x7e00+28 BPB_HiddSec
  add %eax, %ebx

  // 0x7e00+13 BPB_SecPerClus
  xor %eax, %eax

  movb 0xd(%di), %al
  push %ax              # 保存每个簇中的块个数
  
  // 0x7e00+17 BPB_RootEntCnt
  movw 0x11(%di), %ax
  push %ax              # 保存根目录最多项数

// 获取数据区起始区扇区号
GetDataAreaBase:
  // mov 0x24(%di), %eax
  mov 0x16(%di), %ax    # 0x7e00+22 BPB_FATSz16

  xor %cx, %cx
  mov 0x10(%di), %cl    # 0x7e00+16 BPB_NumFATs

  // ebx+=BPB_NumFATs*BPB_FATSz16
  AddFatSize:
    add %eax, %ebx
    loop AddFatSize

  push %ebx             # 保存根目录块号

// 读取数据区8个扇区/1个簇
ReadRootDirectory:
  mov %ebx, BlockLow
  movw $0x8000, BufferOffset 
  mov $0x8000, %di 
  movb $8, BlockCount # 读取根目录8个扇区
  call ReadDisk

.set STR_LOAD, 0x44414f4c
.set STR_ER__, 0x20205245
.set STR_ELF_, 0x20464c45
.set STR_BIN_, 0x204e4942

SeekTheInitialBin:
  cmpl $STR_LOAD, (%di)
  jne nextFile
  cmpl $STR_ER__, 4(%di)
  jne nextFile
  cmpl $STR_BIN_, 8(%di)
  jne nextFile

  jmp InitialBinFound
  
  nextFile:
    cmp $0x9000, %di
    ja BootLoaderEnd
    add $32, %di
    jmp SeekTheInitialBin

InitialBinFound:
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

  NoRemainder:
    mov %ax, BlockCount

    pop %ebx          // 读取根目录块号

    xor %eax, %eax
    pop %ax           // 读取根目录最多项数

    mov %ax, %cx
    and $0x0F, %cx
    cmp $0, %cx
    je NoRemainder2
    inc %ebx          // 若项数除以16有余数

    NoRemainder2:
      shr $4, %ax     // 项数除以16
      add %eax, %ebx

    // 文件起始簇号，也是转为扇区号，乘以8即可
    mov 0x1a(%di), %ax
    sub $2, %ax

    // mov $8, %cx
    pop %cx           // 读取每个簇中的块数
    mul %cx

    // 现在文件起始扇区号存在dx:ax，直接保存到ebx，这个起始是相对于DataBase 0x32,72
    // 所以待会计算真正的起始扇区号还需要加上DataBase
    and $0x0000ffff, %eax
    add %eax, %ebx
    mov %dx, %ax
    shl $16, %eax
    add %eax, %ebx

    mov %ebx, BlockLow

    movw $0x9000, BufferOffset
    mov $0x9000, %di
    call ReadDisk

    // 跳转到Initial.bin继续执行
    jmp *%di

// BIOS INT 13H 中断服务：磁盘服务
// AH=02H 读取扇区
// AL=读入的扇区数（非0）
// CH=磁道号（柱面号）的低8位
// CL=扇区号1~63（bit 0~5），磁道号（柱面号）的高2位（bit 6~7，只对硬盘有效）
// DH=磁头号
// DL=驱动器号（硬盘bit7置1）
// ES:BX=>数据缓冲区

// AH=42H 扩展读
// DL=80H 读硬盘
// SI 参数结构体

ReadDisk:
  mov $0x42, %ah
  mov $0x80, %dl
  mov $DiskAddressPacket, %si
  int $0x13
  test %ah, %ah										# AH==0 说明成功
  jnz BootLoaderEnd
  ret

BootLoaderEnd:
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

// 结束为止
// times 446-($-$$) db 0