#include "vga_driver.h"


volatile uint8_t *vga_text_addr_base = NULL;
volatile uint8_t *vga_text_addr = NULL;

uint32_t power(uint32_t number, uint32_t power) {
    uint32_t base = 1;
    while (power--)
        base *= number;

    return base;
}

uint16_t detect_bios_hardware(void) {
    const uint16_t *hardware_ptr = (const uint16_t *) BIOS_DETECTED_HARDWARE_ADDR;
    return *hardware_ptr;
}

VideoMode get_bios_video_mode(void) {
    return (VideoMode) (detect_bios_hardware() & 0x30);
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

    vga_driver_move(0, 0);
    uint8_t rows = 0;
    uint8_t columns = 0;

    for (;rows < VGA_MAX_ROW; rows++) {
        for (;columns < VGA_MAX_COLUMN; columns++)
            vga_driver_putchar(' ', 0x07);
        columns = 0;
    }

    vga_driver_move(0, 0);

    return true;
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
    uint8_t row = 0, column = 0;
    vga_driver_get_pos(&row, &column);

    if (rows > VGA_MAX_ROW)
        rows = VGA_MAX_ROW;

    volatile uint8_t *temp = vga_text_addr_base;
    vga_driver_move(rows, 0);

    volatile uint8_t *end = (vga_text_addr +
                            (VGA_MAX_COLUMN * 2 * VGA_MAX_ROW));

    for (;vga_text_addr < end;) {
        *temp++ = *vga_text_addr;
        *vga_text_addr++ = ' ';

        *temp++ = *vga_text_addr;
        *vga_text_addr++ = 0x07;
    }

    if (row == 0)
        vga_driver_move(0, column);
    else
        vga_driver_move(row - rows, column);
}

void vga_driver_putchar(uint8_t c, uint8_t color) {
    if (c == '\n') {
        // Newline
        uint8_t rows = 0;
        uint8_t columns = 0;
        vga_driver_get_pos(&rows, &columns);
        vga_driver_move(rows + 1, 0);
        return;
    }
    *vga_text_addr++ = c;
    *vga_text_addr++ = color;
    if (vga_text_addr >= vga_text_addr_end) {
        vga_driver_scroll(1);
        vga_driver_move(VGA_MAX_ROW - 1, 0);

        vga_driver_move(VGA_MAX_ROW - 1, 1);
    }
}

void vga_driver_print_number(int32_t number, uint8_t color) {
    if (number < 0) {
        vga_driver_putchar('-', color);
        number *= -1;
    }
    uint16_t num_len = 1;
    uint32_t temp_num = number;
    for (; temp_num >= 10; temp_num /= 10, num_len++);

    for (;num_len;)
        vga_driver_putchar((number/power(10, --num_len)) % 10 + '0', color);
}

void vga_driver_print(const char *string, uint16_t color) {
    while (*string != '\0') {
        vga_driver_putchar(*string++, color);
    }
}
