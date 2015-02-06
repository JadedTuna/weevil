CC = i686-elf-gcc
CP_CFLAGS = -fdiagnostics-color=auto -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LK_CFLAGS = -fdiagnostics-color=auto -ffreestanding -O2 -nostdlib

BOOT = boot
LINKER = linker.ld
NAME-i686 = weevil-i686

i686-all:
	@nasm -felf32 i686/$(BOOT).asm -o $(BOOT).o
	@$(CC) -c i686/src/*.c lib/*.c $(CP_CFLAGS)
	
	@$(CC) -T i686/$(LINKER) -o $(NAME-i686).bin $(LK_CFLAGS) *.o -lgcc

	@rm *.o

i686-iso:
	@mkdir -p i686/isodir/boot/grub
	@cp $(NAME-i686).bin i686/isodir/boot/
	@cp i686/grub.cfg i686/isodir/boot/grub/
	@grub-mkrescue -o $(NAME-i686).iso i686/isodir

i686-run:
	@qemu-system-i386 -kernel $(NAME-i686).bin

i686-clean:
	@rm -fr $(NAME-i686).bin $(NAME-i686).iso i686/isodir/