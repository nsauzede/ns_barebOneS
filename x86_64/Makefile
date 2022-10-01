TARGET:=kernel64.elf kernel64.bin
GDB:=gdb
NASM:=nasm

Q:=qemu-system-x86_64 -cdrom iso64.iso -s
#DBG:=1
ifdef DBG
Q+=-S
endif

all: kernel64.elf

check64:    kernel64.elf
	grub-file --is-x86-multiboot kernel64.elf && echo "Success!" || (echo "Fail!!" ; false)
	mkdir -p isodir/boot/grub
	cp -f kernel64.elf isodir/boot/
	echo 'set default="0"' > isodir/boot/grub/grub.cfg
	echo 'set timeout=1' >> isodir/boot/grub/grub.cfg
	echo 'menuentry "kernel64 elf" {' >> isodir/boot/grub/grub.cfg
	echo 'multiboot /boot/kernel64.elf' >> isodir/boot/grub/grub.cfg
	echo '}' >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o iso64.iso isodir
	$(Q)

iso64: check64

kernel64.elf: boot.o kernel.o libc.o vid.o gdt.o panic.o idt.o
	gcc -m64 -T linker.ld -o kernel64.elf -ffreestanding -g -O0 $^ -nostdlib -lgcc

db64:
#	$(MAKE) check64 T=1 &
	$(GDB) -ex 'set confirm off' -ex 'target remote :1234' -ex 'disp/i$$cs*16+(unsigned short)$$rip' -ex 'disp/8bx$$cs*16+(unsigned short)$$rip' -ex 'disp/x$$rax' -ex 'disp/x$$rbx' -ex 'disp/x$$cs' -ex 'disp/x$$rip' -ex 'file src/kernel64/kernel64.elf' -ex 'b kernel_main'

CFLAGS:=-m64 -std=gnu99
CFLAGS+=-ffreestanding -z max-page-size=0x1000 -mno-red-zone -mno-mmx -mno-sse -mno-sse2
CFLAGS+=-g -O0 -Wall -Wextra -Werror -I../libc -I../kernel
%.o: ../libc/%.c
	gcc -c $< -o $@ $(CFLAGS)

%.o: ../kernel/%.c
	gcc -c $< -o $@ $(CFLAGS)

%.o:    %.asm
	$(NASM) -f elf -o $@ $<

%.o:    %.S
	gcc -m64 -c $< -o $@ $(CFLAGS) -DASSEMBLY

%.bin: %.elf
	objcopy --remove-section=.note.gnu.build-id -O binary $^ $@

all: $(TARGET)

clean:
	$(RM) $(TARGET) *.o *.iso

clobber: clean
	$(RM) -Rf isodir
