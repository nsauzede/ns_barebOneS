TARGET:=kernel32.elf kernel32.bin
GDB:=gdb
NASM:=nasm

Q:=qemu-system-i386 -cdrom iso32.iso -s
#DBG:=1
ifdef DBG
Q+=-S
endif

all: check32

check32:    kernel32.elf
	grub-file --is-x86-multiboot kernel32.elf && echo "Success!" || echo "Fail!!"
	mkdir -p isodir/boot/grub
	cp -f kernel32.elf isodir/boot/
	echo 'set default="0"' > isodir/boot/grub/grub.cfg
	echo 'set timeout=1' >> isodir/boot/grub/grub.cfg
	echo 'menuentry "kernel32 elf" {' >> isodir/boot/grub/grub.cfg
	echo 'multiboot /boot/kernel32.elf' >> isodir/boot/grub/grub.cfg
	echo '}' >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o iso32.iso isodir
	$(Q)

iso32: check32

kernel32.elf: boot.o kernel.o libc.o vid.o gdt.o panic.o idt.o idt_wrappers.o
	gcc -m32 -T ia32/linker.ld -o kernel32.elf -ffreestanding -g -O0 -nostdlib $^ -lgcc

db32:
#	$(MAKE) check32 T=1 &
	$(GDB) -ex 'set confirm off' -ex 'target remote :1234' -ex 'disp/i$$cs*16+(unsigned short)$$eip' -ex 'disp/8bx$$cs*16+(unsigned short)$$eip' -ex 'disp/x$$eax' -ex 'disp/x$$ebx' -ex 'disp/x$$cs' -ex 'disp/x$$eip' -ex 'file kernel32.elf' -ex 'b kernel_main'

CFLAGS:=-m32 -std=gnu99 -ffreestanding -Wall -Wextra -Werror -Ilibc -Ikernel
CFLAGS+=-g
CFLAGS+=-O0
#CFLAGS+=-O2
kernel.o:CFLAGS+=-O2
%.o: libc/%.c
	gcc -c $< -o $@ $(CFLAGS)

%.o: kernel/%.c
	gcc -c $< -o $@ $(CFLAGS)

%.o:    ia32/%.asm
	$(NASM) -f elf -o $@ $<

%.o:    ia32/%.S
	gcc -m32 -c $< -o $@ $(CFLAGS) -DASSEMBLY

%.bin: %.elf
	objcopy --remove-section=.note.gnu.build-id -O binary $^ $@

all: $(TARGET)

clean:
	$(RM) $(TARGET) *.o *.iso

clobber: clean
	$(RM) -Rf isodir
