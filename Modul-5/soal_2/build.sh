#!/bin/bash
set -e

WORKDIR=$(pwd)

# 1. Bersihkan file lama agar tidak ada sisa kompilasi gagal
rm -f bootloader.bin kernel.bin kernel-asm.o kernel.o floppy.img

# 2. Buat floppy kosong
dd if=/dev/zero of=floppy.img bs=512 count=2880
echo "created floppy.img"

echo ""
nasm -f bin bootloader.asm -o bootloader.bin
echo "bootloader.bin OK"

echo ""
docker run --rm \
  -v "$WORKDIR":/build \
  -w /build \
  ubuntu:22.04 \
  bash -c "
    set -e

    apt-get update -qq
    apt-get install -y -qq nasm bcc bin86 binutils 2>/dev/null

    echo '[Docker] tools installed'

    nasm -f as86 kernel.asm -o kernel-asm.o
    bcc -ansi -c kernel.c -o kernel.o

    # URUTAN LINKER: kernel-asm.o WAJIB DI DEPAN KERNEL.O
    ld86 -0 -d -o kernel.bin kernel-asm.o kernel.o

    echo '[Docker] kernel.bin OK'
  "

echo ""
echo "=== HASIL KOMPILASI ==="
ls -lh bootloader.bin kernel.bin floppy.img
echo "======================="

echo ""
# 3. Masukkan bootloader ke Sektor 1 (512 byte)
dd if=bootloader.bin of=floppy.img bs=512 count=1 conv=notrunc

# 4. Masukkan kernel ke Sektor 2 dan seterusnya (sebanyak 30 sektor)
dd if=kernel.bin of=floppy.img bs=512 seek=1 count=30 conv=notrunc
echo "floppy.img ready"

echo ""
echo "done"