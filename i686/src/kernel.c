#include "kernel.h"
#include "string.h"
#include "vga_driver.h"

void print_center(const char *str, uint8_t color) {
    uint8_t row = 0, column = 0;
    vga_driver_get_pos(&row, &column);
    column = (VGA_MAX_COLUMN - strlen(str))/2;
    vga_driver_move(row, column);

    vga_driver_print(str, color);
}

void kernel_main() {
    if (!vga_driver_init())
        // Something went wrong
        return;
    vga_driver_print("This is not visible\n", 0x42);
    print_center("The above line is not visible\n", 0x24);
    print_center(" This text is centered \n", 0xF8);
    print_center(" And this too! \n", 0x42);

    vga_driver_scroll(1);
    print_center("The text should be now scrolled\n", 0x10);
}