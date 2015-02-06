#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

uint8_t TERMINAL_COLOR;
uint8_t terminal_row;
uint8_t terminal_column;

bool __TTY_DID_INIT;

bool terminal_init(void);
bool terminal_check_init(void);
void terminal_set_color(const uint8_t);
void terminal_clean(const uint8_t);
void terminal_move(const uint8_t, const uint8_t);
void terminal_scroll(uint8_t);
void terminal_putchar(const uint8_t);
void terminal_write(const char *, const size_t);
void terminal_printd(const int64_t);
void terminal_prints(const char *);

#endif /* _KERNEL_TTY_H */