# Soal 2 - Final Challenge
# Deskripsi Soal

Pada soal ini praktikan diminta melengkapi kernel sederhana berbasis x86 16-bit dengan mengimplementasikan beberapa fungsi dasar dan command shell.

File yang diperbolehkan untuk diubah:

* `kernel.asm`
* `kernel.c`

# Pengerjaan

## 1. Implementasi `_getChar`

Fungsi `_getChar` pada `kernel.asm` digunakan untuk membaca input keyboard menggunakan BIOS Interrupt `0x16`.

```asm
_getChar:
    mov ah, 0x00
    int 0x16
    mov ah, 0x00
    ret
```
---
# Hasil

## Build

```bash
make build
```

Berhasil menghasilkan:

* `bootloader.bin`
* `kernel.bin`
* `floppy.img`

## Run

```bash
make run
```
## Kendala

Saat proses linking muncul error:

- undefined symbol: imod

- Display Library SDL2 Tidak Tersedia

Saat menjalankan Bochs muncul error:

- display library 'sdl2' not available

Masalah diselesaikan dengan mengganti konfigurasi menjadi:
display_library: x
BIOS Path Tidak Sesuai

Kesimpulan

Pada praktikum ini saya tidak berhasil membuat sebuah shell sederhana berbasis kernel x86 16-bit. Kernel tidak mampu menerima input pengguna, melakukan pemrosesan string, menjalankan operasi aritmatika, menghitung factorial, mengubah warna tampilan, membuat pola segitiga, serta membersihkan layar. 
