#ifndef _KERNEL_KERNEL_H
#define _KERNEL_KERNEL_H

#define UNUSED(x) (void)x;

#include <stdint.h>
#include <stdbool.h>

void kernel_panic(const char *);
void kernel_early(void);
void kernel_main(void);
void kernel_halt(void);

#endif /* _KERNEL_KERNEL_H */