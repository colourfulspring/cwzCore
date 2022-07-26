#include <asm.h>

.set SMAP,                  0x534d4150              # SMAP字符串

.code32

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


    # 探测内存
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


.code64                                     # 声明64位模式
_start64:
    mov $stack_top, %rsp
    and $0xFFFFFFFFFFFFF000, %rsp
    mov $0, %rbp
    call main
    hlt
    ret

.section .bss
stack:
    .skip 16384

stack_top:
