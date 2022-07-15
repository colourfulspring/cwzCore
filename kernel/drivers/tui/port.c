#include "port.h"

uint8_t Port_inb(uint32_t _port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "dN"(_port));
    return ret;
}

uint16_t Port_inw(uint32_t _port) {
    uint16_t ret;
    __asm__ volatile("inw %1, %0" : "=a"(ret) : "dN"(_port));
    return ret;
}

uint32_t Port_ind(uint32_t _port) {
    uint32_t ret;
    __asm__ volatile("inl %1, %0" : "=a"(ret) : "dN"(_port));
    return ret;
}

void Port_outw(uint32_t _port, uint16_t _data) {
    __asm__ volatile("outw %1, %0" : : "dN"(_port), "a"(_data));
}

void Port_outb(uint32_t _port, uint8_t _data) {
    __asm__ volatile("outb %1, %0" : : "dN"(_port), "a"(_data));
}

void Port_outd(uint32_t _port, uint32_t _data) {
    __asm__ volatile("outl %1, %0" : : "dN"(_port), "a"(_data));
}
