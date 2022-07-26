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
uint8_t port_inb(uint32_t _port);

/**
 * @brief  读一个字
 * @param  _port           要读的端口
 * @return uint16_t        读取到的数据
 * @warning 不处理执行失败的问题
 */
uint16_t port_inw(uint32_t _port);

/**
 * @brief  读一个双字
 * @param  _port           要读的端口
 * @return uint32_t        读取到的数据
 * @warning 不处理执行失败的问题
 */
uint32_t port_ind(uint32_t _port);

/**
 * @brief  写一个字节
 * @param  _port           要写的端口
 * @param  _data           要写的数据
 * @warning 不处理执行失败的问题
 */
void port_outb(uint32_t _port, uint8_t _data);

/**
 * @brief  写一个字
 * @param  _port           要写的端口
 * @param  _data           要写的数据
 * @warning 不处理执行失败的问题
 */
void port_outw(uint32_t _port, uint16_t _data);

/**
 * @brief  写一个双字
 * @param  _port           要写的端口
 * @param  _data           要写的数据
 * @warning 不处理执行失败的问题
 */
void port_outd(uint32_t _port, uint32_t _data);

#endif /* _PORT_H_ */
