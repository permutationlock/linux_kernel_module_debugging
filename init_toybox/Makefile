.PHONY: all clean run run-halted

all: initramfs.cpio

initramfs.cpio: sysroot
	cd sysroot; find . | cpio -o --format=newc > ../initramfs.cpio

sysroot:
	mkdir -p sysroot/bin
	mkdir -p sysroot/dev
	mkdir -p sysroot/home
	mkdir -p sysroot/proc
	mkdir -p sysroot/sys
	cp ../toybox sysroot/bin/toybox
	cd sysroot/bin; for i in $$(./toybox); do ln -s toybox $$i; done
	cp init sysroot/init

clean:
	rm -rf sysroot initramfs.cpio

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
