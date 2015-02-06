#ifndef _KERNEL_H
#define _KERNEL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


uint16_t detect_bios_hardware(uint32_t);
void outb(uint16_t, uint8_t);
uint8_t inb(uint16_t);

#endif /* _KERNEL_H */