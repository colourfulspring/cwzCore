#include "stdint.h"

void main() {
    __asm__ __volatile__(
        "mov $0x1301, %%ax;"
        "mov $0x000f, %%bx;"
        "mov $19, %%cx;"
        "mov $0, %%dx;"
        "push %%ax;"
        "mov %%ds, %%ax;"
        "mov %%ax, %%es;"
        "pop %%ax;"
        "mov StartBootMessage, %%bp;"
        "int $0x10;"
        :
        :);
    while (1) {
    }
}