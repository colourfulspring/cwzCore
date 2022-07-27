#ifndef _TUI_H_
#define _TUI_H_

#include "color.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

/**
 * @brief 位置信息
 */
typedef struct pos_t {
    // 列
    uint8_t col;
    // 行
    uint8_t row;
} pos_t;

/**
 * @brief 颜色信息
 */
typedef struct col_t {
    // 前景色
    uint8_t fore : 4;
    // 背景色
    uint8_t back : 4;
} col_t;

/**
 * @brief 内存中的字符对象
 */
typedef struct char_t {
    // 字符
    uint8_t c;
    // 颜色
    col_t color;
} char_t;

/**
 * @brief TUI 接口
 */


/// CRT 控制寄存器-地址
#define TUI_ADDR 0x3D4
/// CRT 控制寄存器-数据
#define TUI_DATA 0x3D5
/// 光标高位
#define TUI_CURSOR_H 0xE
/// 光标低位
#define TUI_CURSOR_L 0xF
/// TUI 缓存基址
/// @todo 从 grub 获取
#define TUI_MEM_BASE 0xB8000
/// TUI 缓存大小
/// @todo 从 grub 获取
#define TUI_MEM_SIZE 0x8000
/// 规定显示行数
/// @todo 从 grub 获取
#define TUI_WIDTH 80
/// 规定显示列数
/// @todo 从 grub 获取
#define TUI_HEIGHT 25

void TUI_init(void);

/**
 * @brief 在指定位置输出
 * @param  _c              字符
 * @param  _color          颜色
 * @param  _x              行
 * @param  _y              列
 */
void TUI_putEntryAt(char _c, col_t _color, size_t _x, size_t _y);

/**
 * @brief 转义字符处理
 * @param  _c              要处理的字符
 * @return true            _c 是一个转义字符
 * @return false           _c 不是一个转义字符
 */
bool TUI_escapeConv(char _c);

/**
 * @brief 滚动显示
 */
void TUI_scroll(void);


/**
 * @brief 设置颜色
 * @param  _color           要设置的颜色
 * @todo 支持分别字体与背景色
 */
void TUI_setColor(color_t _color);

/**
 * @brief 获取颜色
 * @return COLOR::color_t  正在使用的颜色
 */
color_t TUI_getColor(void);

/**
 * @brief 设置光标位置
 * @param  _pos            要设置的位置
 */
void TUI_setPos(pos_t _pos);

/**
 * @brief 设置行
 * @param  _row            要设置的行
 */
void TUI_setRow(size_t _row);

/**
 * @brief 设置列
 * @param  _col            要设置的列
 */
void TUI_setColumn(size_t _col);

/**
 * @brief 获取光标位置
 * @return pos_t           光标的位置
 */
pos_t TUI_getPos(void);

/**
 * @brief 写 TUI 缓存
 * @param  _idx            要写的位置
 * @param  _data           要写的数据
 */
void TUI_write(size_t _idx, char_t _data);

/**
 * @brief 读 TUI 缓存
 * @param  _idx            要读的位置
 * @return char_t          该位置处的 char_t 对象
 */
char_t TUI_read(size_t _idx);

/**
 * @brief 写字符
 * @param  _c              要写的字符
 */
void TUI_putChar(char _c);

/**
 * @brief 读字符
 * @return uint8_t         读到的字符
 * @note tui 没有读字符的操作，这里只是保持接口一致
 */
uint8_t TUI_getChar(void);

/**
 * @brief 写字符串
 * @param  _s              要写的字符串
 */
void TUI_writeString(const char *_s);

/**
 * @brief 写指定长度的字符串
 * @param  _s              要写的字符串
 * @param  _len            要写的长度
 */
void TUI_writeBuf(const char *_s, const size_t _len);

/**
 * @brief 清屏，清空 TUI 缓存
 */
void TUI_clear(void);


#endif /* _TUI_H_ */
