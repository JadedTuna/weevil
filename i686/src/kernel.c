#include "kernel.h"
#include "../../lib/string.h"
#include "vga_driver.h"
#include "keyboard_driver.h"

uint16_t detect_bios_hardware(uint32_t addr) {
    const uint16_t *hardware_ptr = (const uint16_t *) addr;
    return *hardware_ptr;
}

void print_center(const char *str) {
    uint8_t row = 0, column = 0;
    vga_driver_get_pos(&row, &column);
    column = (VGA_MAX_COLUMN - strlen(str))/2;
    vga_driver_move(row, column);

    vga_driver_print(str);
}

void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret = 0;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void kernel_main() {
    if (!vga_driver_init())
        // Something went wrong
        return;

    if (!keyboard_driver_init()) {
        // Unlucky user has no keyboard?
        vga_driver_set_color(0x4f);
        vga_driver_clear(0x4f);
        print_center("Keyboard not found\n");
        print_center("Press any key to continue . . .");
        return;
    }

    vga_driver_move(0, 0);
    vga_driver_update_cursor();
    int i = 0;

    vga_driver_set_color(0x18);
    for (i=0; i<25; i++) {
        vga_driver_print_number(i);
        _vga_driver_putchar('\n', true);
    }
    vga_driver_move(10, 0);
    vga_driver_scroll(1);

    keyboard_driver_getchar();
}