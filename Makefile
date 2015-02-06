CC = i686-elf-gcc
ASM = nasm

CFLAGS = -fstack-protector -fdiagnostics-color=auto -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CFLAGS2 = -fstack-protector -ffreestanding -O2 -nostdlib
AFLAGS = -felf32

INCLUDES = -Ikernel/include -Ilibc
LIBS = -lgcc

OBJDIR = obj
OBJ_LINK_LIST = $(OBJDIR)/*.o

KERNELDIR = kernel

ARCH = i686
ARCHDIR = $(KERNELDIR)/arch/$(ARCH)

LIBCDIR = libc

all: weevil.kernel

weevil.kernel: compile_obj
	@$(CC) -T $(ARCHDIR)/linker.ld -o weevil-$(ARCH).bin $(CFLAGS2) $(OBJ_LINK_LIST) \
		$(LIBS)

compile_obj:
	@mkdir -p $(OBJDIR)
	@$(ASM) $(AFLAGS) $(ARCHDIR)/boot.asm -o $(OBJDIR)/boot.o
	@for source_file in $(ARCHDIR)/*.c; do \
		filename=$$(basename $$source_file); \
		$(CC) $(CFLAGS) $(INCLUDES) -c $$source_file -o $(OBJDIR)/$$filename.o; \
	done

	@for source_file in $$(find $(LIBCDIR) -name '*.c'); do \
		filename=$$(basename $$source_file); \
		$(CC) $(CFLAGS) $(INCLUDES) -c $$source_file -o $(OBJDIR)/$$filename.o; \
	done

	@for source_file in $(KERNELDIR)/*.c; do \
		filename=$$(basename $$source_file); \
		$(CC) $(CFLAGS) $(INCLUDES) -c $$source_file -o $(OBJDIR)/$$filename.o; \
	done

iso:
	@mkdir -p isodir/boot/grub
	@cp weevil-$(ARCH).bin isodir/boot/
	@cp grub.cfg isodir/boot/grub/
	@grub-mkrescue -o weevil-$(ARCH).iso isodir/
	@rm -fr isodir

clean:
	@rm -fr obj/*.o weevil-$(ARCH).bin weevil-$(ARCH).iso

run:
	qemu-system-i386 -kernel weevil-$(ARCH).bin