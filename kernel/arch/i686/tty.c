#include <vga.h>
#include <tty.h>
#include <bios.h>
#include <kernel.h>
#include <string.h>

bool __TTY_DID_INIT = false;

uint8_t terminal_row = 0;
uint8_t terminal_column = 0;

uint64_t power(const uint32_t num, uint8_t pow) {
    uint64_t base = 1;
    while (pow--)
        base *= num;

    return base;
}

/* Initialize the terminal (set default color, etc.).
    Nothing fancy for now. */
bool terminal_init(void) {
    TERMINAL_COLOR = 0x07; /* MS-DOS default */

    terminal_clean(TERMINAL_COLOR);
    __TTY_DID_INIT = true;

    return true;
}

bool terminal_check_init(void) {
    return __TTY_DID_INIT;
}

void terminal_set_color(const uint8_t color) {
    TERMINAL_COLOR = color;
}

void terminal_clean(const uint8_t color) {
    TERMINAL_COLOR = color;
    terminal_row = 0;
    terminal_column = 0;
    VGA_VIDEO_ADDR = VGA_VIDEO_ADDR_BASE;

    for (;VGA_VIDEO_ADDR < VGA_VIDEO_ADDR_END;) {
        *VGA_VIDEO_ADDR++ = ' ';
        *VGA_VIDEO_ADDR++ = TERMINAL_COLOR;
    }

    VGA_VIDEO_ADDR = VGA_VIDEO_ADDR_BASE;
    terminal_move(0, 0);
}

void terminal_move(const uint8_t row, const uint8_t column) {
    uint16_t position = (row * VGA_WIDTH) + column;

    /*
     This is taken from wiki.osdev.org
     WHY the second call should be IO_PORT + 1? Seems... weird...
     */
    outb(VGA_IO_PORT, 0x0F);
    outb(VGA_IO_PORT + 1, (uint8_t) (position & 0xFF));

    outb(VGA_IO_PORT, 0x0E);
    outb(VGA_IO_PORT + 1, (uint8_t) ((position >> 8) & 0xFF));
}

void terminal_scroll(uint8_t rows) {
    if (!rows) return;

    if (rows > VGA_HEIGHT)
        rows = VGA_HEIGHT;

    uint8_t *temp = VGA_VIDEO_ADDR_BASE;
    uint8_t *save = VGA_VIDEO_ADDR;

    VGA_VIDEO_ADDR = VGA_VIDEO_ADDR_BASE + (VGA_WIDTH * 2 * rows);

    for (;VGA_VIDEO_ADDR < save;) {
        *temp++ = *VGA_VIDEO_ADDR;
        *VGA_VIDEO_ADDR++ = ' ';

        *temp++ = *VGA_VIDEO_ADDR;
        *VGA_VIDEO_ADDR++ = TERMINAL_COLOR;
    }

    for (;VGA_VIDEO_ADDR < VGA_VIDEO_ADDR_END;) {
        *VGA_VIDEO_ADDR++ = ' ';
        *VGA_VIDEO_ADDR++ = TERMINAL_COLOR;
    }

    VGA_VIDEO_ADDR = save;
    terminal_move(terminal_row, terminal_column);
}

void terminal_putchar(const uint8_t character) {
    if (character == '\n') {
        // Newline
        terminal_column = 0;
        if (++terminal_row >= VGA_HEIGHT) {
            terminal_row = VGA_HEIGHT - 1;
            terminal_scroll(1);
        }
        VGA_VIDEO_ADDR = VGA_VIDEO_ADDR_BASE + (terminal_row * VGA_WIDTH * 2);
        return; 
    }

    *VGA_VIDEO_ADDR++ = character;
    *VGA_VIDEO_ADDR++ = TERMINAL_COLOR;
    if (++terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row >= VGA_HEIGHT) {
            terminal_row = VGA_HEIGHT - 1;
            terminal_scroll(1);
        }
    }
}

void terminal_write(const char *string, const size_t size) {
    size_t i;
    for (i=0; i<size; i++) {
        if (string[i] == '\xFF') {
            // Color support :)
            terminal_set_color(string[++i]);
            continue;
        }
        terminal_putchar(string[i]);
    }

    terminal_move(terminal_row, terminal_column);
}

void terminal_printd(const int64_t number) {
    int64_t temp_num = number;
    if (temp_num < 0) {
        terminal_putchar('-');
        temp_num *= -1;
    }
    size_t num_len = 1;
    for (; temp_num >= 10; temp_num /= 10, num_len++);

    for (;num_len;)
        terminal_putchar((number/power(10, --num_len)) % 10 + '0');
}

void terminal_prints(const char *string) {
    terminal_write(string, strlen(string));
}