
/**
 * @file tui.cpp
 * @brief TUI 接口实现
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

#include "stddef.h"
#include "stdint.h"
#include "string.h"
#include "tui.h"
#include "port.h"

/// 记录当前位置
pos_t TUI_pos;
/// 记录当前命令行颜色
col_t TUI_color;

// TUI 缓存
char_t *const TUI_buffer = (char_t *) TUI_MEM_BASE;

col_t make_col(uint8_t _fore, uint8_t _back) {
    col_t res;
    res.fore = _fore;
    res.back = _back;
    return res;
}

pos_t make_pos(uint8_t _col, uint8_t _row) {
    pos_t res;
    res.col = _col;
    res.row = _row;
    return res;
}

char_t make_char(uint8_t _c, col_t _col) {
    char_t res;
    res.c = _c;
    res.color = _col;
    return res;
}

void TUI_init(void) {
    // 当前位置
    TUI_pos.row = 0;
    TUI_pos.col = 0;

    // 当前命令行颜色
    TUI_color.fore = COLOR_WHITE;
    TUI_color.back = COLOR_BLACK;

    TUI_clear();

    TUI_writeString("TUI init.\n");
}

void TUI_putEntryAt(const char _c, col_t _color, const size_t _x,
                    const size_t _y) {
    // 计算索引
    const size_t index = _y * TUI_WIDTH + _x;
    TUI_write(index, make_char(_c, _color));
}

bool TUI_escapeConv(const char _c) {
    switch (_c) {
        // 如果是 \n
        case '\n': {
            // 行+1
            TUI_pos.row++;
            // 列归零
            TUI_pos.col = 0;
            return true;
        }
            // 如果是 \t
        case '\t': {
            // 取整对齐
            TUI_pos.col += (TUI_pos.col % 4 == 0) ? 4 : 4 - (TUI_pos.col % 4);
            return true;
        }
            // 如果是 \b
        case '\b': {
            if (TUI_pos.col > 0) {
                // 删除一个字符
                TUI_write(TUI_pos.row * TUI_WIDTH + --TUI_pos.col, make_char(' ', TUI_color));
            }
            return true;
        }
        default:
            break;
    }
    return false;
}

void TUI_scroll(void) {
    if (TUI_pos.row >= TUI_HEIGHT) {
        // 将所有行的显示数据复制到上一行
        for (size_t i = 0; i < (TUI_HEIGHT - 1) * TUI_WIDTH; i++) {
            TUI_write(i, TUI_read(i + TUI_WIDTH));
        }
        // 最后的一行数据现在填充空格，不显示任何字符
        for (size_t i = (TUI_HEIGHT - 1) * TUI_WIDTH; i < TUI_HEIGHT * TUI_WIDTH; i++) {
            TUI_write(i, make_char(' ', TUI_color));
        }
        // 向上移动了一行，所以 cursor_y 现在是 24
        TUI_pos.row = TUI_HEIGHT - 1;
    }
}

void TUI_setColor(color_t _color) {
    TUI_color = make_col(_color, COLOR_BLACK);
}

color_t TUI_getColor(void) {
    return (color_t) (TUI_color.back | TUI_color.fore);
}

void TUI_setPos(const pos_t _pos) {
    const uint16_t index = _pos.row * TUI_WIDTH + _pos.col;
    TUI_pos = _pos;
    // 光标的设置，见参考资料
    // 告诉 TUI 我们要设置光标的高字节
    Port_outb(TUI_ADDR, TUI_CURSOR_H);
    // 发送高 8 位
    Port_outb(TUI_DATA, index >> 8);
    // 告诉 TUI 我们要设置光标的低字节
    Port_outb(TUI_ADDR, TUI_CURSOR_L);
    // 发送低 8 位
    Port_outb(TUI_DATA, index);
}

void TUI_setRow(const size_t _row) {
    TUI_pos.row = _row;
    TUI_setPos(TUI_pos);
}

void TUI_setColumn(const size_t _col) {
    TUI_pos.col = _col;
    TUI_setPos(TUI_pos);
}

pos_t TUI_getPos(void) {
// 通过读写相应寄存器获取光标位置
    Port_outb(TUI_ADDR, TUI_CURSOR_H
    );
    size_t cursor_pos_h = Port_inb(TUI_DATA);
    Port_outb(TUI_ADDR, TUI_CURSOR_L
    );
    size_t cursor_pos_l = Port_inb(TUI_DATA);
// 返回光标位置
    return make_pos(cursor_pos_l, cursor_pos_h);
}

void TUI_write(const size_t _idx, char_t _data) {
    // 向缓冲区写数据
    TUI_buffer[_idx] = _data;
}

char_t TUI_read(const size_t _idx) {
// 从缓冲区读数据
    return TUI_buffer[_idx];
}

void TUI_putChar(const char _c) {
    // 转义字符处理
    if (TUI_escapeConv(_c) == false) {
        // 在指定位置输出指定字符
        TUI_putEntryAt(_c, TUI_color, TUI_pos.col, TUI_pos.row);
        // 如果到达最后一列则换行
        if (++TUI_pos.col >= TUI_WIDTH) {
            TUI_pos.col = 0;
            TUI_pos.row++;
        }
    }
    // 屏幕滚动
    TUI_scroll();
    // 更新光标位置
    TUI_setPos(TUI_pos);
}

uint8_t TUI_getChar(void) {
    return 0;
}

void TUI_writeString(const char *_s) {
    TUI_writeBuf(_s, strlen(_s));
}

void TUI_writeBuf(const char *_s, const size_t _len) {
    for (size_t i = 0; i < _len; i++) {
        TUI_putChar(_s[i]);
    }
}

void TUI_clear(void) {
    // 从左上角开始
    for (size_t y = 0; y < TUI_HEIGHT; y++) {
        for (size_t x = 0; x < TUI_WIDTH; x++) {
            const size_t index = y * TUI_WIDTH + x;
            // 用 ' ' 填满屏幕
            // 字体为灰色，背景为黑色
            TUI_write(index, make_char(' ', TUI_color));
        }
    }
    // 将光标位置设为屏幕左上角
    TUI_setPos(make_pos(0, 0));
}
