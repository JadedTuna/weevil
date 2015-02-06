#ifndef _VGA_DRIVER_H
#define _VGA_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


#define BIOS_DETECTED_HARDWARE_ADDR 0x410
#define VGA_COLOR_ADDR              0xB8000
#define VGA_MONOCHROME_ADDR         0xB0000

#define VGA_MAX_ROW    25
#define VGA_MAX_COLUMN 80


volatile uint8_t *vga_text_addr_base;
volatile uint8_t *vga_text_addr_end;
volatile uint8_t *vga_text_addr;

typedef enum {
    VIDEO_TYPE_NONE = 0x00,
    VIDEO_TYPE_COLOR = 0x20,
    VIDEO_TYPE_MONOCHROME = 0x30
} VideoMode;

VideoMode get_bios_video_mode(void);

bool vga_driver_init(void);
void vga_driver_move(uint8_t, uint8_t);
void vga_driver_get_pos(uint8_t *, uint8_t *);
void vga_driver_putchar(uint8_t, uint8_t);
void vga_driver_print_number(int32_t, uint8_t);
void vga_driver_print(const char *, uint16_t);
void vga_driver_scroll(uint8_t);

#endif /* _VGA_DRIVER_H */