
/**
 * @file common.h
 * @brief 通用定义头文件
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2021-09-18
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleKernel
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2021-09-18<td>digmouse233<td>迁移到 doxygen
 * </table>
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include "stdint.h"
#include "stddef.h"

// 引用链接脚本中的变量
/// @see http://wiki.osdev.org/Using_Linker_Script_Values
/// 内核开始
void *kernel_start[];
/// 内核代码段开始
void *kernel_text_start[];
/// 内核代码段结束
void *kernel_text_end[];
/// 内核数据段开始
void *kernel_data_start[];
/// 内核数据段结束
void *kernel_data_end[];
/// 内核结束
void *kernel_end[];

/// 内核开始
#define KERNEL_START_ADDR (uintptr_t)(kernel_start)
/// 内核代码段开始
#define KERNEL_TEXT_START_ADDR (uintptr_t)(kernel_text_start)
/// 内核代码段结束
#define KERNEL_TEXT_END_ADDR (uintptr_t)(kernel_text_end)
/// 内核数据段开始
#define KERNEL_DATA_START_ADDR (uintptr_t)(kernel_data_start)
/// 内核数据段结束
#define KERNEL_DATA_END_ADDR (uintptr_t)(kernel_data_end)
/// 内核结束
#define KERNEL_END_ADDR (uintptr_t)(kernel_end)

// 规定数据大小，方便用
/// 一个字节，8 bits
#define BYTE 0x1
/// 1KB
#define KB 0x400
/// 1MB
#define MB 0x100000
/// 页大小 4KB
#define PAGE_SIZE 4 * KB

/**
 * @brief 对齐
 * @tparam T
 * @param  _addr           要对齐的地址
 * @param  _align          要对齐的目标
 * @return T               对齐过的地址
 * @note 向上取整
 * @note 针对指针
 */

#define ALIGN(T, _addr, _align) (T)((ptrdiff_t)((uint8_t *)(_addr) + (_align) - 1) & (~((_align) - 1)))

#endif /* _COMMON_H_ */
