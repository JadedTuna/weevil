CC = i686-elf-gcc
CP_CFLAGS = -fdiagnostics-color=auto -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LK_CFLAGS = -fdiagnostics-color=auto -ffreestanding -O2 -nostdlib

BOOT = boot
LINKER = linker.ld
NAME = weevil

all:
	@nasm -felf32 $(BOOT).asm -o $(BOOT).o
	@$(CC) -c src/*.c $(CP_CFLAGS)
	
	@$(CC) -T $(LINKER) -o $(NAME).bin $(LK_CFLAGS) *.o -lgcc

	@rm *.o

iso:
	@mkdir -p isodir/boot/grub
	@cp $(NAME).bin isodir/boot/
	@cp grub.cfg isodir/boot/grub/
	@grub-mkrescue -o $(NAME).iso isodir

run:
	@qemu-system-i386 -kernel $(NAME).bin

clean:
	@rm -fr $(NAME).bin $(NAME).iso isodir/