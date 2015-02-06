#ifndef _KERNEL_BIOS_H
#define _KERNEL_BIOS_H

#include <stddef.h>
#include <stdint.h>

static const uint32_t BIOS_ADDR_DETECTED_HARDWARE = 0x410;
static const uint32_t BIOS_ADDR_VIDEO_IO_PORT     = 0x463;

uint16_t bios_get_hardware_info(const uint32_t);
void outb(const uint16_t, const uint8_t);
uint8_t inb(const uint16_t);

#endif /* _KERNEL_BIOS_H */