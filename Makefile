CP := cp
RM := rm -rf
MKDIR := mkdir -pv

BIN = kernel
CFG = grub.cfg
ISO_PATH := iso
BOOT_PATH := $(ISO_PATH)/boot
GRUB_PATH := $(BOOT_PATH)/grub

.PHONY: all
all: bootloader kernel int drivers utils linker iso
	echo "Make has completed."

bootloader: src/boot.asm
	nasm -f elf32 src/boot.asm -o boot.o

int: src/interrupts/interrupt.asm src/interrupts/idt.c src/interrupts/isr.c
	nasm -f elf32 src/interrupts/interrupt.asm -o interrupt.o

	gcc -m32 -c src/interrupts/idt.c -o idt.o
	gcc -m32 -c src/interrupts/isr.c -o isr.o -fno-stack-protector

drivers: src/drivers/vga/display.c src/drivers/kbd/kbd.c src/drivers/ports/ports.c
	gcc -m32 -c src/drivers/vga/display.c -o display.o
	gcc -m32 -c src/drivers/kbd/kbd.c -o kbd.o -fno-stack-protector
	gcc -m32 -c src/drivers/ports/ports.c -o ports.o

kernel: src/kernel.c
	gcc -m32 -c src/kernel.c -o kernel.o

utils: src/utils/util.c
	gcc -m32 -c src/utils/util.c -o util.o

linker: linker.ld boot.o kernel.o interrupt.o idt.o isr.o display.o kbd.o ports.o util.o
	ld -m elf_i386 -T linker.ld -o kernel kernel.o boot.o interrupt.o idt.o isr.o display.o kbd.o ports.o util.o

iso: kernel
	$(MKDIR) $(GRUB_PATH)
	$(CP) $(BIN) $(BOOT_PATH)
	$(CP) $(CFG) $(GRUB_PATH)
	grub-file --is-x86-multiboot $(BOOT_PATH)/$(BIN)
	grub-mkrescue -o my-kernel.iso $(ISO_PATH)
.PHONY: clean
clean:
	$(RM) *.o $(BIN) *iso