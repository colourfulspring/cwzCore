
/**
 * @file port.h
 * @brief IA32 端口读写
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

#ifndef _PORT_H_
#define _PORT_H_

#include "stdint.h"

/**
 * @brief 端口读写封装
 */

/**
 * @brief  读一个字节
 * @param  _port           要读的端口
 * @return uint8_t         读取到的数据
 * @warning 不处理执行失败的问题
 */
uint8_t Port_inb(uint32_t _port);

/**
 * @brief  读一个字
 * @param  _port           要读的端口
 * @return uint16_t        读取到的数据
 * @warning 不处理执行失败的问题
 */
uint16_t Port_inw(uint32_t _port);

/**
 * @brief  读一个双字
 * @param  _port           要读的端口
 * @return uint32_t        读取到的数据
 * @warning 不处理执行失败的问题
 */
uint32_t Port_ind(uint32_t _port);

/**
 * @brief  写一个字节
 * @param  _port           要写的端口
 * @param  _data           要写的数据
 * @warning 不处理执行失败的问题
 */
void Port_outb(uint32_t _port, uint8_t _data);

/**
 * @brief  写一个字
 * @param  _port           要写的端口
 * @param  _data           要写的数据
 * @warning 不处理执行失败的问题
 */
void Port_outw(uint32_t _port, uint16_t _data);

/**
 * @brief  写一个双字
 * @param  _port           要写的端口
 * @param  _data           要写的数据
 * @warning 不处理执行失败的问题
 */
void Port_outd(uint32_t _port, uint32_t _data);

#endif /* _PORT_H_ */
