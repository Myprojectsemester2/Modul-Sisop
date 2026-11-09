prepare:
	rm -f *.o *.bin floppy.img
	dd if=/dev/zero of=floppy.img bs=512 count=2880

bootloader:
	nasm -f bin bootloader.asm -o bootloader.bin
	dd if=bootloader.bin of=floppy.img bs=512 count=1 conv=notrunc

kernel:
	nasm -f as86 kernel.asm -o kernel-asm.o
	bcc -ansi -c kernel.c -o kernel.o
	# PERBAIKAN: kernel-asm.o wajib di depan, tambahkan flag -0
	ld86 -0 -d -o kernel.bin kernel-asm.o kernel.o
	# PERBAIKAN: Isi kernel dipotong sebanyak 30 sektor agar tidak korup
	dd if=kernel.bin of=floppy.img bs=512 seek=1 count=30 conv=notrunc

build: prepare bootloader kernel

run:
	bochs -f bochsrc.txt