#include <tty.h>
#include <vga.h>
#include <kernel.h>
#include <stdio.h>

/* Print error message to the screen && dump register contents
    (in case VGA driver & terminal were init()-ed)
    and halt the CPU */
void kernel_panic(const char* message) {
    UNUSED(message);
    /* If this function gets called, something went COMPLETELY wrong */

    if (vga_check_init() && terminal_check_init()) {
        /* There is no use in printing to the screen in case
           we couldn't initialize it */
        ;//printf("%s\n", message);
}
    
    kernel_halt();
}

void kernel_early(void) {
    /* Init VGA driver and collect required information */
    if (!vga_init())
        /* Call kernel_panic so we get some logs atleast */
        kernel_panic("Failed to initialize VGA driver");

    /* Init terminal */
    if (!terminal_init())
        /* Again, for the logs */
        kernel_panic("Failed to initialize terminal");

    terminal_prints("Starting WEEVIL...\n");
    /* That's some lying - we have initialized driver already.
       But it still looks cool :) */
    terminal_prints("Initializing VGA driver          [ \xff\x02OK\xff\x07 ]\n");
}

void kernel_main(void) {
    terminal_prints("Kernel initialized.\nWelcome to WEEVIL!\n");
}

void kernel_halt(void) {
    /* Halt the CPU */
    asm ("hlt");
    kernel_halt(); /* Security never hurts */
}