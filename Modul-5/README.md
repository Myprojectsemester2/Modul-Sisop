<div align="center">

# Operating System dan Bootloaders

</div>

**Bootloader** dan **Kernel** adalah komponen paling mendasar dari sistem operasi yang dieksekusi sejak komputer dinyalakan (melalui BIOS/UEFI) untuk menginisialisasi perangkat keras dan menyediakan antarmuka sistem.

> 📖 **Akses Materi Lengkap:** [Buka MATERI.md (Operating System dan Bootloaders)](./MATERI.md)  
> 👤 **Praktikan:** Raffa Al Azmi (`5027251079`) — [Repositori Asal: `SISOP-5-2026-IT-079`](https://github.com/raffaalazmi-png/SISOP-5-2026-IT-079)

---

# Table of Contents

- [Operating System dan Bootloaders](#operating-system-dan-bootloaders)
- [Table of Contents](#table-of-contents)
- [1. Materi Referensi Modul 5](#1-materi-referensi-modul-5)
- [2. Laporan Praktikum — Soal 2 (Final Challenge: Kernel x86 16-bit)](#2-laporan-praktikum--soal-2-final-challenge-kernel-x86-16-bit)
  - [2.1 Deskripsi Soal](#21-deskripsi-soal)
  - [2.2 Implementasi `_getChar` (`kernel.asm`)](#22-implementasi-_getchar-kernelasm)
  - [2.3 Proses Build dan Eksekusi (Bochs Emulator)](#23-proses-build-dan-eksekusi-bochs-emulator)
  - [2.4 Kendala dan Solusi Konfigurasi](#24-kendala-dan-solusi-konfigurasi)
  - [2.5 Evaluasi & Catatan Praktikum](#25-evaluasi--catatan-praktikum)

---

## 1. Materi Referensi Modul 5

Materi lengkap mengenai **Arsitektur Sistem Operasi**, **Proses Booting**, **Bootloader Assembly**, dan **Pembuatan Kernel 16-bit** beserta playground kode tersedia di dalam folder ini:

- 📄 **[Baca Materi Lengkap Modul 5 (`MATERI.md`)](./MATERI.md)**
- 💻 **[Playground Kode Referensi (`./playground/`)](./playground/)**

---

## 2. Laporan Praktikum — Soal 2 (Final Challenge: Kernel x86 16-bit)

📁 **Lokasi Kode:** [`./soal_2/`](./soal_2/) (`bootloader.asm`, `kernel.asm`, `kernel.c`, `Makefile`, `build.sh`, `bochsrc.txt`)

### 2.1 Deskripsi Soal

Pada modul ini praktikan diminta melengkapi *kernel* sederhana berbasis arsitektur **x86 16-bit** dengan mengimplementasikan fungsi dasar I/O serta *command shell*.

File utama yang dimodifikasi:
- [`soal_2/kernel.asm`](./soal_2/kernel.asm)
- [`soal_2/kernel.c`](./soal_2/kernel.c)

### 2.2 Implementasi `_getChar` (`kernel.asm`)

Fungsi `_getChar` pada `kernel.asm` digunakan untuk membaca karakter input dari keyboard menggunakan **BIOS Interrupt `0x16`**:

```asm
_getChar:
    mov ah, 0x00
    int 0x16
    mov ah, 0x00
    ret
```

### 2.3 Proses Build dan Eksekusi (Bochs Emulator)

1. **Build Image Floppy Disk:**
   ```bash
   make build
   ```
   Menghasilkan artefak *bootable*:
   - `bootloader.bin`
   - `kernel.bin`
   - `floppy.img`

2. **Menjalankan Emulator Bochs:**
   ```bash
   make run
   ```

### 2.4 Kendala dan Solusi Konfigurasi

1. **Error Linking (`undefined symbol: imod`):**  
   Terjadi ketika operasi modulo dipanggil pada kompilasi C 16-bit (`bcc`) tanpa definisi *helper* aritmatika yang sesuai.
2. **Display Library SDL2 Tidak Tersedia pada Bochs:**  
   Saat menjalankan Bochs muncul pesan error:
   ```text
   display library 'sdl2' not available
   ```
   **Solusi:** Mengganti konfigurasi pada `bochsrc.txt` menjadi:
   ```text
   display_library: x
   ```
   serta menyesuaikan path BIOS dan VGABIOS agar sesuai dengan direktori instalasi Bochs pada sistem.

### 2.5 Evaluasi & Catatan Praktikum

Proses *kompilasi* (`bootloader.bin`, `kernel.bin`, dan `floppy.img`) serta *booting* pada emulator Bochs telah berhasil dijalankan dan fungsi pembacaan karakter BIOS Interrupt `0x16` (`_getChar`) telah diimplementasikan. Untuk pengembangan lebih lanjut, fitur lanjutan pada *shell* (`kernel.c`) seperti operasi aritmatika (mengatasi `imod`), perhitungan faktorial, perubahan warna layar via interrupt BIOS `0x10`, pola segitiga, dan `clear` layar dapat dilengkapi di atas kerangka *kernel* yang sudah terbangun.
