.PHONY: all clean run run-halted

CC:=gcc
CFLAGS:=-I../linux-6.17_build/sysroot/include -fno-stack-protector -nostdlib -nostdinc -static

all: initramfs.cpio

initramfs.cpio: init
	find init | cpio -o --format=newc > initramfs.cpio

init: init.c
	$(CC) $(CFLAGS) -o init init.c

clean:
	rm -f init initramfs.cpio

run: initramfs.cpio
	qemu-system-x86_64 --enable-kvm -nographic -no-reboot -s \
		-kernel ../linux-6.17_build/arch/x86/boot/bzImage \
		-initrd initramfs.cpio \
		-append "HOST=x86_64 console=ttyS0"

run-halted: initramfs.cpio
	qemu-system-x86_64 --enable-kvm -nographic -no-reboot -s -S \
		-kernel ../linux-6.17_build/arch/x86/boot/bzImage \
		-initrd initramfs.cpio \
		-append "HOST=x86_64 console=ttyS0"
