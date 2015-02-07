#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <tty.h>

int printf(const char *format, ...) {
    va_list arg_list;
    va_start(arg_list, format);

    char *string = NULL;
    int digit = 0;

    size_t i;
    size_t len = strlen(format);
    for (i=0; i<len; i++) {
        switch (format[i]) {
            case '%':
                /* Format character goes next */
                switch (format[++i]) {
                    case 's':
                        /* String */
                        string = va_arg(arg_list, char *);
                        terminal_prints(string);
                        break;

                    case 'd': case 'i':
                        /* Signed integer */
                        digit = va_arg(arg_list, int);
                        terminal_printd(digit);
                        break;

                    case '%': default:
                        /* Just print it */
                        terminal_putchar('%');
                        terminal_putchar(format[i]);
                        break;
                }
                break;

            default:
                /* Usual character */
                terminal_putchar(format[i]);
                break;
        }
    }
    terminal_move(terminal_row, terminal_column);

    va_end(arg_list);

    return 0;
}