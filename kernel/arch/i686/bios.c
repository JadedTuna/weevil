#include <bios.h>

uint16_t bios_get_hardware_info(const uint32_t addr) {
    const uint16_t *hardware_ptr = (const uint16_t *) addr;
    return *hardware_ptr;
}

void outb(const uint16_t port, const uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(const uint16_t port) {
    uint8_t ret = 0;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}