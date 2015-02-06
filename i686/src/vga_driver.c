#include "vga_driver.h"


volatile uint8_t *vga_text_addr_base = NULL;
volatile uint8_t *vga_text_addr = NULL;

uint8_t VGA_COLOR = 0;

uint32_t power(uint32_t number, uint32_t power) {
    uint32_t base = 1;
    while (power--)
        base *= number;

    return base;
}

VideoMode get_bios_video_mode(void) {
    return (VideoMode) (detect_bios_hardware(BIOS_DETECTED_HARDWARE_ADDR) & 0x30);
}

uint16_t get_bios_io_port(void) {
    return detect_bios_hardware(BIOS_VIDEO_IO_PORT_ADDR);
}


bool vga_driver_init(void) {
    VideoMode mode = get_bios_video_mode();
    switch (mode) {
        case VIDEO_TYPE_NONE:
            // Huh? No video device?? Abandon ship!
            return false;
            break;
        case VIDEO_TYPE_COLOR:
            vga_text_addr = vga_text_addr_base = \
                (volatile uint8_t *) VGA_COLOR_ADDR;
            vga_text_addr_end = vga_text_addr + (VGA_MAX_ROW * VGA_MAX_COLUMN * 2);
            break;
        case VIDEO_TYPE_MONOCHROME:
            vga_text_addr = vga_text_addr_base = \
                (volatile uint8_t *) VGA_MONOCHROME_ADDR;
            vga_text_addr_end = vga_text_addr + (VGA_MAX_ROW * VGA_MAX_COLUMN * 2);
            break;
    }

    VGA_IO_PORT = get_bios_io_port();

    vga_driver_clear(VGA_COLOR_DEFAULT);

    return true;
}

void vga_driver_clear(uint8_t color) {
    vga_driver_move(0, 0);
    uint8_t row = 0;
    uint8_t column = 0;

    VGA_COLOR = color;
    for (;row < VGA_MAX_ROW; row++) {
        for (;column < VGA_MAX_COLUMN; column++)
            vga_driver_putchar(' ');
        column = 0;
    }

    vga_driver_move(0, 0);
}

void vga_driver_move(uint8_t row, uint8_t column) {
    if (row > VGA_MAX_ROW)
        row = VGA_MAX_ROW;

    if (column > VGA_MAX_COLUMN)
        column = VGA_MAX_COLUMN;

    vga_text_addr = vga_text_addr_base + (row * 2 * VGA_MAX_COLUMN) + column * 2;
}


void vga_driver_get_pos(uint8_t *rows, uint8_t *columns) {
    uint32_t temp = (uint32_t) (vga_text_addr - vga_text_addr_base);
    temp /= 2;
    *rows  = temp/VGA_MAX_COLUMN;
    *columns = temp - (*rows * VGA_MAX_COLUMN);
}

void vga_driver_scroll(uint8_t rows) {
    if (!rows) return;

    uint8_t row = 0, column = 0;
    vga_driver_get_pos(&row, &column);

    if (rows > VGA_MAX_ROW)
        rows = VGA_MAX_ROW;

    volatile uint8_t *temp = (uint8_t *) vga_text_addr_base;
    vga_driver_move(1, 0);

    for (;vga_text_addr < vga_text_addr_end;) {
        *temp++ = *vga_text_addr;
        *vga_text_addr++ = ' ';

        *temp++ = *vga_text_addr;
        *vga_text_addr++ = VGA_COLOR;
    }

    vga_driver_move(row, 0);
    vga_driver_update_cursor();
}

void vga_driver_update_cursor(void) {
    uint8_t row = 0, column = 0;
    vga_driver_get_pos(&row, &column);

    uint16_t position = (row * VGA_MAX_COLUMN) + column;

    outb(VGA_IO_PORT, 0x0F);
    outb(VGA_IO_PORT + 1, (uint8_t) (position & 0xFF));

    outb(VGA_IO_PORT, 0x0E);
    outb(VGA_IO_PORT + 1, (uint8_t) ((position >> 8) & 0xFF));
}

void _vga_driver_putchar(uint8_t c, bool move_cursor) {
    uint8_t row = 0;
    uint8_t column = 0;
    if (c == '\n') {
        // Newline
        vga_driver_get_pos(&row, &column);
        vga_driver_move(row + 1, 0);
    } else {
        *vga_text_addr++ = c;
        *vga_text_addr++ = VGA_COLOR;
    }

    if (vga_text_addr > vga_text_addr_end) {
        vga_driver_scroll(1);
        vga_driver_move(VGA_MAX_ROW - 1, 0);
    }

    if (move_cursor)
        vga_driver_update_cursor();
}

void vga_driver_putchar(uint8_t c) {
    _vga_driver_putchar(c, false);
}

void vga_driver_print_number(int32_t number) {
    if (number < 0) {
        vga_driver_putchar('-');
        number *= -1;
    }
    uint16_t num_len = 1;
    uint32_t temp_num = number;
    for (; temp_num >= 10; temp_num /= 10, num_len++);

    for (;num_len;)
        vga_driver_putchar((number/power(10, --num_len)) % 10 + '0');
}

void vga_driver_print(const char *string) {
    while (*(string + 1) != '\0') {
        vga_driver_putchar(*string++);
    }
    _vga_driver_putchar(*string++, true);
}

void vga_driver_set_color(uint8_t color) {
    VGA_COLOR = color;
}
