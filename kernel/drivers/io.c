
// This file is a part of Simple-XX/SimpleKernel
// (https://github.com/Simple-XX/SimpleKernel).
//
// io.cpp for Simple-XX/SimpleKernel.
// IO 接口实现

#include "stdarg.h"
#include "stddef.h"
#include "string.h"


#include "port.h"

#include "io.h"
#include "stdio.h"

void IO_init() {
    TUI_init();
}

uint8_t IO_inb(uint32_t port) {
    return Port_inb(port);
}

uint16_t IO_inw(uint32_t port) {
    return Port_inw(port);
}

uint32_t IO_ind(uint32_t port) {
    return Port_ind(port);
}

void IO_outb(uint32_t port, uint8_t data) {
    Port_outb(port, data);
}

void IO_outw(uint32_t port, uint16_t data) {
    Port_outw(port, data);
}

void IO_outd(uint32_t port, uint32_t data) {
    Port_outd(port, data);
}

// MMIO 实现
uint8_t IO_read8(void *_addr) {
    return *(uint8_t *) _addr;
}

void IO_write8(void *_addr, uint8_t _val) {
    *(uint8_t *) _addr = _val;
}

uint16_t IO_read16(void *_addr) {
    return *(uint16_t *) _addr;
}

void IO_write16(void *_addr, uint16_t _val) {
    *(uint16_t *) _addr = _val;
}

uint32_t IO_read32(void *_addr) {
    return *(uint32_t *) _addr;
}

void IO_write32(void *_addr, uint32_t _val) {
    *(uint32_t *) _addr = _val;
}

uint64_t IO_read64(void *_addr) {
    return *(uint64_t *) _addr;
}

void IO_write64(void *_addr, uint64_t _val) {
    *(uint64_t *) _addr = _val;
}

color_t IO_get_color(void) {
    // 通过对应的 io 方式获取颜色
    return TUI_getColor();
}

void IO_set_color(const color_t color) {
    // 通过对应的 io 方式设置颜色
    TUI_setColor(color);
}

void IO_put_char(char c) {
    // 通过对应的 io 方式输出字符
    TUI_putChar(c);
}

char IO_get_char(void) {
    TUI_getChar();
}

int32_t IO_write_string(const char *s) {
    TUI_writeString(s);
    return 0;
}

/// 输出缓冲区
char buf[IO_BUF_SIZE];

/**
 * @brief printf 定义
 * @param  _fmt           格式化字符串
 * @return int32_t        输出的长度
 */
int32_t printf(const char *_fmt, ...) {
    va_list va;
    va_start(va, _fmt);
    // 交给 src/libc/src/stdio/vsprintf.c 中的 _vsnprintf
    // 处理格式，并将处理好的字符串保存到 buf 中
    const int ret = _vsnprintf(buf, IO_BUF_SIZE, _fmt, va);
    va_end(va);
    // 输出 buf
    IO_write_string(buf);
    // 清空数据
    bzero(buf, IO_BUF_SIZE);
    return ret;
}

/**
 * @brief 与 printf 类似，只是颜色不同
 */
int32_t info(const char *_fmt, ...) {
    color_t curr_color = IO_get_color();
    IO_set_color(COLOR_CYAN);
    va_list va;
    int32_t i;
    va_start(va, _fmt);
    i = vsnprintf_(buf, IO_BUF_SIZE, _fmt, va);
    va_end(va);
    IO_write_string(buf);
    bzero(buf, IO_BUF_SIZE);
    IO_set_color(curr_color);
    return i;
}

/**
 * @brief 与 printf 类似，只是颜色不同
 */
int32_t warn(const char *_fmt, ...) {
    color_t curr_color = IO_get_color();
    IO_set_color(COLOR_YELLOW);
    va_list va;
    int32_t i;
    va_start(va, _fmt);
    i = vsnprintf_(buf, IO_BUF_SIZE, _fmt, va);
    va_end(va);
    IO_write_string(buf);
    bzero(buf, IO_BUF_SIZE);
    IO_set_color(curr_color);
    return i;
}

/**
 * @brief 与 printf 类似，只是颜色不同
 */
int32_t err(const char *_fmt, ...) {
    color_t curr_color = IO_get_color();
    IO_set_color(COLOR_LIGHT_RED);
    va_list va;
    int32_t i;
    va_start(va, _fmt);
    i = vsnprintf_(buf, IO_BUF_SIZE, _fmt, va);
    va_end(va);
    IO_write_string(buf);
    bzero(buf, IO_BUF_SIZE);
    IO_set_color(curr_color);
    return i;
}

void putc(char ch) {
    IO_put_char(ch);
}

char getc(void) {
    return IO_get_char();
}