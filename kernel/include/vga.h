#ifndef _KERNEL_VGA_H
#define _KERNEL_VGA_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum {
    VGA_COLOR_BLACK         = 0x0,
    VGA_COLOR_BLUE          = 0x1,
    VGA_COLOR_GREEN         = 0x2,
    VGA_COLOR_CYAN          = 0x3,
    VGA_COLOR_RED           = 0x4,
    VGA_COLOR_MAGENTA       = 0x5,
    VGA_COLOR_BROWN         = 0x6,
    VGA_COLOR_LIGHT_GREY    = 0x7,
    VGA_COLOR_DARK_GREY     = 0x8,
    VGA_COLOR_LIGHT_BLUE    = 0x9,
    VGA_COLOR_LIGHT_GREEN   = 0xA,
    VGA_COLOR_LIGHT_CYAN    = 0xB,
    VGA_COLOR_LIGHT_RED     = 0xC,
    VGA_COLOR_LIGHT_MAGENTA = 0xD,
    VGA_COLOR_LIGHT_BROWN   = 0xE,
    VGA_COLOR_WHITE         = 0xF
} VGAColor;

typedef enum {
    VIDEO_MODE_NONE = 0x00,
    VIDEO_MODE_COLOR = 0x20,
    VIDEO_MODE_MONOCHROME = 0x30,
} VideoMode;

// Video mode (color or monochrome)
VideoMode VGA_VIDEO_MODE;

static const uint32_t VGA_ADDR_MONOCHROME = 0xB0000;
static const uint32_t VGA_ADDR_COLOR = 0xB8000;

static const size_t VGA_WIDTH  = 80;
static const size_t VGA_HEIGHT = 25;

bool __VGA_DID_INIT;
uint8_t *VGA_VIDEO_ADDR_BASE;
uint8_t *VGA_VIDEO_ADDR;
uint8_t *VGA_VIDEO_ADDR_END;

uint16_t VGA_IO_PORT;

static inline uint8_t vga_make_color(const VGAColor foreground, const VGAColor background) {
    return (background << 4) | foreground;
}

bool vga_init(void);
bool vga_check_init(void);

#endif /* _KERNEL_VGA_H */