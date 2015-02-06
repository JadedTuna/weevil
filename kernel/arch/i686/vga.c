#include <vga.h>
#include <bios.h>
#include <kernel.h>

bool __VGA_DID_INIT = false;
uint16_t VGA_IO_PORT;

/* Most of the inline functions are for nicer syntax */
static inline VideoMode vga_get_video_mode(void) {
    return (VideoMode) (bios_get_hardware_info(BIOS_ADDR_DETECTED_HARDWARE) & 0x30);
}

static inline uint16_t vga_get_io_port(void) {
    return bios_get_hardware_info(BIOS_ADDR_VIDEO_IO_PORT);
}

/* Initialize the VGA driver and all the needed variables.
    Nothing can go wrong, right? Right?.. */
bool vga_init(void) {
    VGA_VIDEO_MODE = vga_get_video_mode();
    switch (VGA_VIDEO_MODE) {
        case VIDEO_MODE_NONE:
            /* Poor user has no display */
            return false;
            break;
        case VIDEO_MODE_COLOR:
            VGA_VIDEO_ADDR = VGA_VIDEO_ADDR_BASE = (uint8_t *) VGA_ADDR_COLOR;
            VGA_VIDEO_ADDR_END = VGA_VIDEO_ADDR + (VGA_WIDTH * VGA_HEIGHT * 2);
            break;
        case VIDEO_MODE_MONOCHROME:
            VGA_VIDEO_ADDR = VGA_VIDEO_ADDR_BASE = (uint8_t *) VGA_ADDR_MONOCHROME;
            VGA_VIDEO_ADDR_END = VGA_VIDEO_ADDR + (VGA_WIDTH * VGA_HEIGHT * 2);
            break;
        default:
            /* Bad */
            return false;
            break;
    }

    VGA_IO_PORT = vga_get_io_port();

    /* Now vga_check_init returns true */
    __VGA_DID_INIT = true;
    return true;
}

bool vga_check_init(void) {
    return __VGA_DID_INIT;
}
