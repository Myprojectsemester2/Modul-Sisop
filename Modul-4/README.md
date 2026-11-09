<div align="center">

# Filesystem dan Docker

</div>

**Filesystem in Userspace (FUSE)** memungkinkan pembuatan *virtual filesystem* di ruang pengguna tanpa memodifikasi *kernel*, sedangkan **Docker** menyediakan isolasi aplikasi melalui *containerization*.

> 📖 **Akses Materi Lengkap:** [Buka MATERI.md (Filesystem dan Docker)](./MATERI.md)  
> 👤 **Praktikan:** Raffa Al Azmi (`5027251079`) — [Repositori Asal: `SISOP-4-2026-IT-079`](https://github.com/raffaalazmi-png/SISOP-4-2026-IT-079)

---

# Table of Contents

- [Filesystem dan Docker](#filesystem-dan-docker)
- [Table of Contents](#table-of-contents)
- [1. Materi Referensi Modul 4](#1-materi-referensi-modul-4)
- [2. Laporan Praktikum — Soal 1 (Kenz Rescue FUSE Filesystem)](#2-laporan-praktikum--soal-1-kenz-rescue-fuse-filesystem)
  - [2.1 Deskripsi Soal](#21-deskripsi-soal)
  - [2.2 Langkah Pengerjaan & Arsitektur FUSE](#22-langkah-pengerjaan--arsitektur-fuse)
  - [2.3 Kompilasi, Eksekusi, dan Hasil Pengujian](#23-kompilasi-eksekusi-dan-hasil-pengujian)
- [3. Laporan Praktikum — Soal 2 (Mini Database Service & Encrypted FUSE)](#3-laporan-praktikum--soal-2-mini-database-service--encrypted-fuse)
  - [3.1 Deskripsi Soal](#31-deskripsi-soal)
  - [3.2 Implementasi Encrypted FUSE (`fuse.c`)](#32-implementasi-encrypted-fuse-fusec)
  - [3.3 Pengujian Filesystem FUSE](#33-pengujian-filesystem-fuse)
  - [3.4 Containerization dengan Docker & TCP Client (`client.c`)](#34-containerization-dengan-docker--tcp-client-clientc)
  - [3.5 Kesimpulan](#35-kesimpulan)

---

## 1. Materi Referensi Modul 4

Materi lengkap mengenai **Linux Filesystem**, **FUSE (Filesystem in Userspace)**, dan **Docker Containerization** beserta gambar ilustrasi tersedia di dalam folder ini:

- 📄 **[Baca Materi Lengkap Modul 4 (`MATERI.md`)](./MATERI.md)**

---

## 2. Laporan Praktikum — Soal 1 (Kenz Rescue FUSE Filesystem)

📁 **Lokasi Kode:** [`./soal1/kenz_rescue.c`](./soal1/kenz_rescue.c)

### 2.1 Deskripsi Soal

Membuat sebuah *filesystem virtual* menggunakan FUSE bernama `kenz_rescue` yang berfungsi sebagai perantara (*mirror/passthrough*) antara folder asli `amba_files` dengan *mount point* `mnt`, sekaligus menyisipkan sebuah **file virtual** bernama `tujuan.txt` yang hanya muncul di *mount point* (`mnt/tujuan.txt`) tanpa pernah ada secara fisik di `amba_files/`.

Isi dari `tujuan.txt` dibentuk secara dinamis dengan membaca file `1.txt` sampai `7.txt`, lalu menggabungkan seluruh fragmen teks yang diawali dengan prefix `KOORD:` sesuai urutan file.

### 2.2 Langkah Pengerjaan & Arsitektur FUSE

1. **Menyiapkan Struktur Direktori & Ekstraksi Sumber:**
   ```bash
   unzip amba_files.zip
   rm -rf amba_files.zip
   mkdir mnt
   ```
2. **Header & Konfigurasi FUSE (`kenz_rescue.c`):**
   ```c
   #define FUSE_USE_VERSION 31
   #include <fuse3/fuse.h>
   #include <stdio.h>
   #include <string.h>
   #include <errno.h>
   #include <fcntl.h>
   #include <unistd.h>
   #include <stdlib.h>
   #include <dirent.h>
   #include <sys/stat.h>
   ```
3. **Callback Utama FUSE yang Diimplementasikan:**
   - `fullpath(char fpath[1024], const char *path)` → Menggabungkan path `source_dir` (`amba_files`) dengan path relatif dari `mnt`.
   - `x_getattr` → Mengambil metadata/atribut file (dan memberikan atribut *regular file* virtual untuk `/tujuan.txt`).
   - `x_readdir` → Menampilkan seluruh isi `amba_files` ditambah entri virtual `tujuan.txt`.
   - `x_open` & `x_read` → Meneruskan pembacaan file asli `1.txt`–`7.txt`, atau menyusun gabungan baris `KOORD:` secara dinamis ketika file yang dibaca adalah `/tujuan.txt`.

### 2.3 Kompilasi, Eksekusi, dan Hasil Pengujian

```bash
# Compile
gcc kenz_rescue.c `pkg-config fuse3 --cflags --libs` -o kenz_rescue

# Mount filesystem
./kenz_rescue amba_files mnt
```

1. **Mengecek Isi Mount Point (`ls mnt`):**
   ```text
   1.txt  2.txt  3.txt  4.txt  5.txt  6.txt  7.txt  tujuan.txt
   ```
2. **Memastikan `tujuan.txt` Tidak Ada di Folder Asli (`ls amba_files`):**
   ```text
   1.txt  2.txt  3.txt  4.txt  5.txt  6.txt  7.txt
   ```
3. **Membaca Koordinat Gabungan (`cat mnt/tujuan.txt`):**  
   Menampilkan hasil gabungan fragmen koordinat dari `1.txt` hingga `7.txt` secara dinamis.

---

## 3. Laporan Praktikum — Soal 2 (Mini Database Service & Encrypted FUSE)

📁 **Lokasi Kode:** [`./fuse.c`](./fuse.c), [`./client.c`](./client.c), [`./encrypted_storage/`](./encrypted_storage/), [`./fuse_mount/`](./fuse_mount/)

### 3.1 Deskripsi Soal

Membuat *virtual filesystem* berbasis FUSE (`fuse.c`) yang bertindak sebagai *translator* enkripsi/dekripsi otomatis antara direktori penyimpanan terenkripsi (`encrypted_storage/`) dan *mount point* (`fuse_mount/`), yang kemudian diintegrasikan dengan **Mini Database Service** berbasis CSV di dalam **Docker Container** pada TCP port **`9000`** (`CREATE`, `INSERT`, `UPDATE`, `DELETE`, `SELECT`, `LIST TABLE`).

### 3.2 Implementasi Encrypted FUSE (`fuse.c`)

Filesystem mengimplementasikan operasi lengkap:
```text
getattr, readdir, mkdir, rmdir, create, open, read, write, truncate, unlink, access, utimens
```

- **Saat File Dibuat / Ditulis (`write`):** Data yang ditulis ke `fuse_mount/` otomatis dienkripsi dan disimpan ke `encrypted_storage/` dengan ekstensi `.enc`.
- **Saat File Dibaca (`read`):** File `.enc` di `encrypted_storage/` didekripsi secara *on-the-fly* sehingga pengguna maupun container database dapat membacanya secara normal melalui `fuse_mount/`.

### 3.3 Pengujian Filesystem FUSE

```bash
# Menjalankan FUSE
./fuse encrypted_storage fuse_mount

# Uji tulis dan baca
echo "halo" > fuse_mount/a.txt
cat fuse_mount/a.txt
```

- Di `fuse_mount/a.txt`, isi file terbaca normal (`halo`).
- Di `encrypted_storage/`, file tersimpan dalam keadaan terenkripsi dengan ekstensi `.enc`.
- File pengujian `encrypted_storage/tests/notes.csv.env` juga berhasil ditranslasi dan dibaca melalui `fuse_mount/tests/`.

### 3.4 Containerization dengan Docker & TCP Client (`client.c`)

1. **Build Docker Image (`ubuntu:latest`, expose port `9000`, working dir `/app`):**
   ```bash
   docker build -t soal-2-modul-4-sisop .
   ```
2. **Menjalankan Container dengan Bind Mount ke `fuse_mount`:**
   Container dijalankan di *background* dengan *bind mount* mengarah ke `fuse_mount` sehingga seluruh tabel CSV yang dibuat oleh database server otomatis terenkripsi di `encrypted_storage`.
3. **Komunikasi Client TCP (`client.c`):**
   Program `client.c` terhubung ke port `9000` untuk mengirimkan perintah database (`CREATE`, `INSERT`, `SELECT`, dll.) dan menampilkan respons query dari server.

### 3.5 Kesimpulan

Pada Modul 4 ini berhasil diimplementasikan *virtual filesystem* FUSE baik untuk penyisipan file virtual dinamis (`kenz_rescue`) maupun enkripsi/dekripsi transparan (`fuse.c`) yang terintegrasi dengan layanan database ter-containerisasi menggunakan Docker.
