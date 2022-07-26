// #include <asm.h>

# Start the CPU: switch to 32-bit protected mode, jump into C.
# The BIOS loads this code from the first sector of the hard disk into
# memory at physical address 0x7c00 and starts executing in real mode
# with %cs=0 %ip=7c00.

.set PROT_MODE_CSEG,        0x8                     # kernel code segment selector
.set PROT_MODE_DSEG,        0x10                    # kernel data segment selector
.set CR0_PE_ON,             0x1                     # protected mode enable flag
.set SMAP,                  0x534d4150

# start address should be 0:7c00, in real mode, the beginning address of the running bootloader
.globl start
start:
.code16                                             # Assemble for 16-bit mode
    # cli                                             # Disable interrupts
    # cld                                             # String operations increment

    # Set up the important data segment registers (DS, ES, SS).
    xorw %ax, %ax                                   # Segment number zero
    movw %ax, %ds                                   # -> Data Segment
    movw %ax, %es                                   # -> Extra Segment
    movw %ax, %ss                                   # -> Stack Segment

    # Enable A20:
    #  For backwards compatibility with the earliest PCs, physical
    #  address line 20 is tied low, so that addresses higher than
    #  1MB wrap around to zero by default. This code undoes this.

    push %ax
    mov $0x2401, %ax
    int $0x15 
    pop %ax

probe_memory:
    movl $0, 0x8000            # 0x8000 处开始存放 struct e820map, 前 4 字节是系统内存映射地址描述符（ARD）个数
    xorl %ebx, %ebx            # EBX 清零
    movw $0x8004, %di          # 0x8004 开始存放 struct e820map 的 map 字段, 即以 20 字节为单位的 ARD 数组, BIOS 通过 ES:DI 获得该位置
start_probe:
    movl $0xE820, %eax         # 调用 int 0x15 0xe820 请求 BIOS 进行内存探测
    movl $20, %ecx             # 保存 ARD 的内存大小, 20 字节
    movl $SMAP, %edx           # "SWAP" 字符串
    int $0x15                  # 调用 BIOS 0x15 号中断
    jnc continue_probe         # 如果 int 0x15 执行成功, CF 标志位是 0, 否则是 1
    movw $12345, 0x8000        # CF == 1, 探测失败
    jmp finish_probe
continue_probe:                # CF == 0, 继续探测
    addw $20, %di              # 指出下一个 ARD 应该存放的位置
    incl 0x8000                # 增加 struct e820map 的 nr_map 字段值（增加 ARD 个数）
    cmpl $0, %ebx              # EBX == 0 表示探测已结束
    jnz start_probe            # EBX != 0 则继续探测
finish_probe:


    # Switch from real to protected mode, using a bootstrap GDT
    # and segment translation that makes virtual addresses
    # identical to physical addresses, so that the
    # effective memory map does not change during the switch.

.section .gdt
label_gdt:
    .int 0x0, 0x0
label_desc_code32:
    .int 0x0000ffff, 0x00cf9a00
label_desc_data32:
    .int 0x0000ffff, 0x00cf9200

    .equ gdt_len, 0x17

gdtptr:
    .word gdt_len - 1;
    .int  label_gdt

    .equ selector_code32, label_desc_code32 - label_gdt
    .equ selector_data32, label_desc_data32 - label_gdt

    cli                 # Turn off interrupts
    .byte 0x66          # why does OSbook have this line but ucore doesn't have? Experiment shows no different
    lgdt gdtptr
    movl %cr0, %eax
    orl $CR0_PE_ON, %eax
    movl %eax, %cr0

    # Jump to next instruction, but in 32-bit code segment.
    # Switches processor into 32-bit mode.
    // ljmp $PROT_MODE_CSEG, $protcseg
    ljmp $selector_code32, $protcseg

.code32                                             # Assemble for 32-bit mode
protcseg:
    # Set up the protected-mode data segment registers
    movw $PROT_MODE_DSEG, %ax                       # Our data segment selector
    movw %ax, %ds                                   # -> DS: Data Segment
    movw %ax, %es                                   # -> ES: Extra Segment
    movw %ax, %fs                                   # -> FS
    movw %ax, %gs                                   # -> GS
    movw %ax, %ss                                   # -> SS: Stack Segment


    # Switch from protected mode to IA-32e mode, 
    # ?? using a bootstrap GDT
    # ?? and segment translation that makes virtual addresses
    # ?? identical to physical addresses, so that the
    # ?? effective memory map does not change during the switch.

.section .gdt64
label_gdt64:
    .quad 0x0
label_desc_code64:
    .quad 0x0020980000000000
label_desc_data64:
    .quad 0x0000920000000000

    .equ gdt_len64, 0x17
gdtptr64:
    .word gdt_len64 - 1;
    .int  label_gdt64

    .equ selector_code64, label_desc_code64 - label_gdt64
    .equ selector_data64, label_desc_data64 - label_gdt64

// template page table at address 0x90000
    movl $0x91007, 0x90000
    movl $0x91007, 0x90800

    movl $0x92007, 0x91000

    movl $0x000083, 0x92000
    movl $0x200083, 0x92008
    movl $0x400083, 0x92010
    movl $0x600083, 0x92018
    movl $0x800083, 0x92020
    movl $0xa00083, 0x92028

// load GDTR
    .byte 0x66
    lgdt gdtptr64
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %ds
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

// open PAE
    movl %cr4, %eax
    btsl $5, %eax
    movl %eax, %cr4

// load cr3
    movl $0x90000, %eax
    movl %eax, %cr3

// enable long-mode
    movl $0xc0000080, %ecx      // IA32_EFER
    rdmsr

    btsl $8, %eax
    wrmsr

// open PE and paging
    movl %cr0, %eax
    btsl $0, %eax
    btsl $31, %eax
    movl %eax, %cr0

// jump from loader to kernel
    //ljmp $selector_code64, $bootmain // ??

    # Set up the stack pointer and call into C. The stack region is from 0--start(0x7c00)
    movl $0x0, %ebp
    movl $start, %esp
    call bootmain

    # If bootmain returns (it shouldn't), loop.
spin:
    jmp spin

// 开启A20地址线后，暂时打开保护模式，让FS寄存器具有4GB寻址功能 代码书P57

// 使用封装的Func_ReadOneSector功能把kernel.bin存入内存0x7E00
// int $0x13 用法：
// %ax: 待读取的磁盘起始LBA扇区号：1  除了前面有一个引导扇区（扇区号为0）外
// %cl: 读入的扇区数量：？  ？与kernel.bin的大小有关，前提是kernel.bin不在文件系统里。
// %es:%bx：目标缓冲区起始地址：代码书P59，宏书P56 
// 
// 再把内核从0x7E00移动到0x10000。代码P60
// 
