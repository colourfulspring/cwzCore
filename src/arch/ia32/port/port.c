#include "port.h"

uint8_t port_inb(uint32_t _port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "dN"(_port));
    return ret;
}

uint16_t port_inw(uint32_t _port) {
    uint16_t ret;
    __asm__ volatile("inw %1, %0" : "=a"(ret) : "dN"(_port));
    return ret;
}

uint32_t port_ind(uint32_t _port) {
    uint32_t ret;
    __asm__ volatile("inl %1, %0" : "=a"(ret) : "dN"(_port));
    return ret;
}

void port_outw(uint32_t _port, uint16_t _data) {
    __asm__ volatile("outw %1, %0" : : "dN"(_port), "a"(_data));
}

void port_outb(uint32_t _port, uint8_t _data) {
    __asm__ volatile("outb %1, %0" : : "dN"(_port), "a"(_data));
}

void port_outd(uint32_t _port, uint32_t _data) {
    __asm__ volatile("outl %1, %0" : : "dN"(_port), "a"(_data));
}
