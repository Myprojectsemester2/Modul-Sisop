<div align="center">

# *Process* dan Daemon

</div>

*Process* adalah program yang sedang dieksekusi di dalam sistem operasi, sedangkan **Daemon** adalah proses yang berjalan di latar belakang (*background*) tanpa kontrol langsung dari terminal interaktif pengguna.

> 📖 **Akses Materi Lengkap:** [Buka MATERI.md (*Process* dan Daemon)](./MATERI.md)  
> 👤 **Praktikan:** Raffa Al Azmi (`5027251079`) — [Repositori Asal: `SISOP-2-2026-IT-079`](https://github.com/raffaalazmi-png/SISOP-2-2026-IT-079)

---

# Table of Contents

- [*Process* dan Daemon](#process-dan-daemon)
- [Table of Contents](#table-of-contents)
- [1. Materi Referensi Modul 2](#1-materi-referensi-modul-2)
- [2. Laporan Praktikum — Soal 1 (Kasir Uncle Muthu)](#2-laporan-praktikum--soal-1-kasir-uncle-muthu)
  - [2.1 Deskripsi & Alur Penyelesaian](#21-deskripsi--alur-penyelesaian)
  - [2.2 Implementasi Kode (`kasir_muthu.c`)](#22-implementasi-kode-kasir_muthuc)
  - [2.3 Cara Menjalankan & Hasil Eksekusi](#23-cara-menjalankan--hasil-eksekusi)
- [3. Laporan Praktikum — Soal 2 (Contract Integrity Daemon)](#3-laporan-praktikum--soal-2-contract-integrity-daemon)
  - [3.1 Deskripsi & Mekanisme Daemon](#31-deskripsi--mekanisme-daemon)
  - [3.2 Struktur File & Cara Menjalankan](#32-struktur-file--cara-menjalankan)
  - [3.3 Hasil Pengujian Daemon](#33-hasil-pengujian-daemon)
- [4. Laporan Praktikum — Soal 3 (Angel / Maya Daemon)](#4-laporan-praktikum--soal-3-angel--maya-daemon)
  - [4.1 Alur Pengerjaan](#41-alur-pengerjaan)
  - [4.2 Cara Menjalankan & Output Pengujian](#42-cara-menjalankan--output-pengujian)

---

## 1. Materi Referensi Modul 2

Materi lengkap mengenai **Process**, **Fork & Exec**, **Wait**, serta **Daemon** beserta contoh kode bahasa C tersedia di dalam folder ini:

- 📄 **[Baca Materi Lengkap Modul 2 (`MATERI.md`)](./MATERI.md)**
- 💻 **[Contoh Kode Referensi Modul 2 (`./code/`)](./code/)**

---

## 2. Laporan Praktikum — Soal 1 (Kasir Uncle Muthu)

📁 **Lokasi Kode:** [`./soal_1/`](./soal_1/) (`kasir_muthu.c`, `buku_hutang.csv`, `brankas_kedai/`, `rahasia_muthu.zip`)

### 2.1 Deskripsi & Alur Penyelesaian

Membantu Uncle Muthu untuk mendata dan mengamankan data hutang pelanggannya karena terkena virus, sekaligus memisahkan data pelanggan yang berstatus **`Belum Lunas`** menggunakan pembuatan proses anak (`fork`), eksekusi perintah (`execlp`), dan sinkronisasi (`waitpid`).

1. Mengunduh data awal `buku_hutang.csv`:
   ```text
   https://drive.google.com/uc?export=download&id=144hsFJGoAM5lbvBQjkTfZaD3GOil4yEw
   ```
2. Membuat program `kasir_muthu.c` untuk membuat direktori `brankas_kedai`, menyalin `buku_hutang.csv`, memfilter pelanggan `Belum Lunas` ke `daftar_penunggak.txt`, serta mengarsipkan folder menjadi `rahasia_muthu.zip`.

### 2.2 Implementasi Kode (`kasir_muthu.c`)

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void error_exit() {
    printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.\n");
    exit(EXIT_FAILURE);
}

int main() {
    pid_t pid;
    int status;

    // 1. Membuat folder brankas_kedai [child process]
    pid = fork();
    if (pid == 0) {
        execlp("mkdir", "mkdir", "brankas_kedai", NULL);
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0) error_exit();
    }

    // 2. Menyalin buku_hutang.csv ke brankas_kedai/
    pid = fork();
    if (pid == 0) {
        execlp("cp", "cp", "buku_hutang.csv", "brankas_kedai/", NULL);
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0) error_exit();
    }

    // 3. Memfilter status "Belum Lunas" ke daftar_penunggak.txt
    pid = fork();
    if (pid == 0) {
        execlp("bash", "bash", "-c",
               "grep 'Belum Lunas' brankas_kedai/buku_hutang.csv > brankas_kedai/daftar_penunggak.txt",
               NULL);
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0) error_exit();
    }

    // 4. Mengarsipkan brankas_kedai menjadi rahasia_muthu.zip
    pid = fork();
    if (pid == 0) {
        execlp("zip", "zip", "-r", "rahasia_muthu.zip", "brankas_kedai", NULL);
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0) error_exit();
    }

    printf("[INFO] Fuhh, selamat! Buku hutang dan daftar penagihan berhasil diamankan.\n");
    return 0;
}
```

**Penjelasan Singkat:**
- `fork()` → membuat *child process*
- `execlp()` → menjalankan command Linux di dalam *child process*
- `waitpid()` → *parent process* menunggu *child process* selesai
- `WEXITSTATUS(status)` → memeriksa kode keluar (*exit status*) untuk memastikan tahapan berhasil

### 2.3 Cara Menjalankan & Hasil Eksekusi

```bash
gcc kasir_muthu.c -o kasir
./kasir
```

- **Output Jika Berhasil:**
  ```text
  [INFO] Fuhh, selamat! Buku hutang dan daftar penagihan berhasil diamankan.
  ```
- **Output Jika Gagal:**
  ```text
  [ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.
  ```
- **Struktur Folder Setelah Eksekusi (`ls` & `ls brankas_kedai`):**
  ```text
  brankas_kedai  buku_hutang.csv  kasir_muthu.c  rahasia_muthu.zip
  brankas_kedai/buku_hutang.csv  brankas_kedai/daftar_penunggak.txt
  ```
- **Hasil Filter Data (`cat brankas_kedai/daftar_penunggak.txt`):**
  ```text
  Mail,Ayam Goreng 2 Paha,10.50,Belum Lunas
  Fizi,Es ABC Spesial,3.50,Belum Lunas
  Ahtong,Kopi O Tarik,2.00,Belum Lunas
  Upin,Nasi Lemak Telur Mata,4.00,Belum Lunas
  Ipin,Ayam Goreng 1 Dada,5.00,Belum Lunas
  Mei Mei,Susu Kedelai,2.50,Belum Lunas
  Susanti,Mie Goreng Mamak,5.50,Belum Lunas
  Ijat,Es Limau,1.50,Belum Lunas
  Ehsan,Nasi Lemak Ayam Goreng,9.50,Belum Lunas
  ```

---

## 3. Laporan Praktikum — Soal 2 (Contract Integrity Daemon)

📁 **Lokasi Kode:** [`./soal_2/`](./soal_2/) (`contract_daemon.c`, `contract.txt`, `work.log`)

### 3.1 Deskripsi & Mekanisme Daemon

Implementasi *daemon process* dalam bahasa C yang berjalan di *background* untuk memonitor dan menjaga integritas file `contract.txt`.

1. **Inisialisasi Daemon:** Menggunakan `fork()` dan `setsid()` agar proses berjalan independen dari terminal.
2. **Logging Berkala (Setiap 5 Detik):** Menulis status acak ke `work.log`:
   ```text
   still working...
   [awake/drifting/numbness]
   ```
3. **Pembuatan File `contract.txt` Saat Dimulai:**
   ```text
   A promise to keep going, even when unseen.
   created at: <timestamp>
   ```
4. **Deteksi File Dihapus (*Auto Restore*):** Membuat ulang `contract.txt` dalam 1–2 detik dengan baris kedua `restored at: <timestamp>`.
5. **Deteksi Perubahan File (*Violation*):** Menulis `contract violated.` ke `work.log` dan mengembalikan isi file seperti semula.
6. **Penanganan *Signal Termination* (`SIGTERM`):** Saat dihentikan dengan `kill <PID>`, daemon mencatat pesan perpisahan:
   ```text
   We really weren't meant to be together
   ```

### 3.2 Struktur File & Cara Menjalankan

```text
soal_2/
├── contract_daemon.c
├── daemon
├── contract.txt
└── work.log
```

```bash
# Compile
gcc contract_daemon.c -o daemon

# Run di background
./daemon &

# Menghentikan daemon
ps aux | grep daemon
kill <PID>
```

### 3.3 Hasil Pengujian Daemon

1. **Logging Berjalan (`cat work.log`):**
   ```text
   still working...
   [awake]
   still working...
   [drifting]
   ```
2. **File Contract Dibuat (`cat contract.txt`):**
   ```text
   A promise to keep going, even when unseen.
   created at: 2026-04-07 14:30:00
   ```
3. **File Dihapus → Auto Restore (`rm contract.txt && cat contract.txt`):**
   ```text
   A promise to keep going, even when unseen.
   restored at: 2026-04-07 14:32:10
   ```
4. **File Diubah → Violation & Restore:**
   Menambahkan teks ke `contract.txt` menghasilkan log `contract violated.` di `work.log` dan isi file dikembalikan.
5. **Daemon Dihentikan (`kill <PID>`):**
   ```text
   We really weren't meant to be together
   ```

---

## 4. Laporan Praktikum — Soal 3 (Angel / Maya Daemon)

📁 **Lokasi Kode:** [`./soal_3/`](./soal_3/) (`angel.c`)

### 4.1 Alur Pengerjaan

1. Program dijalankan dengan argumen `-daemon`.
2. Membuat proses *background* (`fork` + `setsid`) dan mengubah nama proses menjadi `maya`.
3. Melakukan *loop* setiap ±10 detik untuk memilih kalimat secara acak, melakukan *encoding* Base64 ke `LoveLetter.txt`, dan mencatat status ke `ethereal.log`:
   - `"aku akan fokus pada diriku sendiri"`
   - `"aku mencintaimu dari sekarang hingga selamanya"`
   - `"aku akan menjauh darimu, hingga takdir mempertemukan kita di versi kita yang terbaik."`
   - `"kalau aku dilahirkan kembali, aku tetap akan terus menyayangimu"`
4. Opsi `-decrypt` digunakan untuk mendekode isi `LoveLetter.txt`.
5. Opsi `-kill` digunakan untuk menghentikan daemon.

### 4.2 Cara Menjalankan & Output Pengujian

1. **Compile dan Jalankan Daemon:**
   ```bash
   gcc angel.c -o angel
   ./angel -daemon
   ps aux | grep maya
   ```
   ```text
   rappa     1021  0.0  0.0   2684  1152 ?   Ss   23:29   0:00 maya
   ```

2. **Cek Isi File Terenkripsi (`cat LoveLetter.txt`):**
   ```text
   YWt1IG1lbmNpbnRhaW11IGRhcmkgc2VrYXJhbmcgaGluZ2dhIHNlbGFtYW55YQ==
   ```

3. **Dekripsi File (`./angel -decrypt`):**
   ```bash
   ./angel -decrypt
   cat LoveLetter.txt
   ```
   ```text
   aku mencintaimu dari sekarang hingga selamanya
   ```

4. **Cek Log Aktivitas (`cat ethereal.log`):**
   ```text
   [10:04:2026]-[23:40:01]_compose_RUNNING
   [10:04:2026]-[23:40:01]_compose_DONE
   [10:04:2026]-[23:40:01]_encode_RUNNING
   [10:04:2026]-[23:40:01]_encode_DONE
   ```

5. **Menghentikan Daemon (`./angel -kill`):**
   ```text
   Daemon dimatikan
   ```
