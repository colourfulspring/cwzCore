
/**
 * @file io.h
 * @brief IO 接口
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

#ifndef _IO_H_
#define _IO_H_

#include "stdint.h"
#include "color.h"

#include "tui.h"

/**
 * @brief IO 接口抽象
 */

/// 缓冲区大小
#define IO_BUF_SIZE 512

// io 对象
void IO_init();

/**
 * @brief 端口读字节
 * @param  _port           要读的端口
 * @return uint8_t         读到的数据
 */
uint8_t IO_inb(uint32_t _port);

/**
 * @brief 端口读字
 * @param  _port           要读的端口
 * @return uint16_t        读到的数据
 */
uint16_t IO_inw(uint32_t _port);

/**
 * @brief 端口读双字
 * @param  _port           要读的端口
 * @return uint32_t        读到的数据
 */
uint32_t IO_ind(uint32_t _port);

/**
 * @brief 端口写字节
 * @param  _port           要写的端口
 * @param  _data           要写的数据
 */
void IO_outb(uint32_t _port, uint8_t _data);

/**
 * @brief 端口写字
 * @param  _port           要写的端口
 * @param  _data           要写的数据
 */
void IO_outw(uint32_t _port, uint16_t _data);

/**
 * @brief 端口写双字
 * @param  _port           要写的端口
 * @param  _data           要写的数据
 */
void IO_outd(uint32_t _port, uint32_t _data);

/**
 * @brief MMIO 读字节
 * @param  _addr           要读的地址
 * @return uint8_t         读到的数据
 */
uint8_t IO_read8(void *_addr);

/**
 * @brief MMIO 写字节
 * @param  _addr           要写的地址
 * @param  _val            要写的值
 */
void IO_write8(void *_addr, uint8_t _val);

/**
 * @brief MMIO 读字
 * @param  _addr           要读的地址
 * @return uint16_t        读到的数据
 */
uint16_t IO_read16(void *_addr);

/**
 * @brief MMIO 写字
 * @param  _addr           要写的地址
 * @param  _val            要写的值
 */
void IO_write16(void *_addr, uint16_t _val);

/**
 * @brief MMIO 读双字
 * @param  _addr           要读的地址
 * @return uint32_t        读到的数据
 */
uint32_t IO_read32(void *_addr);

/**
 * @brief MMIO 写双字
 * @param  _addr           要写的地址
 * @param  _val            要写的数据
 */
void IO_write32(void *_addr, uint32_t _val);

/**
 * @brief MMIO 读四字
 * @param  _addr           要读的地址
 * @return uint64_t        读到的数据
 */
uint64_t IO_read64(void *_addr);

/**
 * @brief MMIO 写四字
 * @param  _addr           要写的地址
 * @param  _val            要写的数据
 */
void IO_write64(void *_addr, uint64_t _val);

/**
 * @brief 获取当前颜色
 * @return COLOR::color_t  当前使用的颜色
 */
color_t IO_get_color(void);

/**
 * @brief 设置当前颜色
 * @param  _color          要设置的颜色
 */
void IO_set_color(const color_t _color);

/**
 * @brief 输出字符
 * @param  _c              要输出的字符
 */
void IO_put_char(const char _c);

/**
 * @brief 输入字符
 * @return char            读到的字符
 * @todo
 */
char IO_get_char(void);

/**
 * @brief 输出字符串
 * @param  _s              要输出的字符串
 * @return int32_t         输出的长度
 * @todo 返回值
 */
int32_t IO_write_string(const char *_s);

#endif /* _IO_H_ */
