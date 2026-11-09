<div align="center">

# Shell Scripting, Cron, dan AWK

</div>

Shell Scripting, Cron, dan AWK adalah tiga komponen penting yang digunakan dalam sistem operasi mirip Unix untuk otomatisasi tugas, penjadwalan pekerjaan, dan pemrosesan teks.

> 📖 **Akses Materi Lengkap:** [Buka MATERI.md (Shell Scripting, Cron, dan AWK)](./MATERI.md)  
> 👤 **Praktikan:** Raffa Al Azmi (`5027251079`) — [Repositori Asal: `SISOP-1-2026-IT-079`](https://github.com/raffaalazmi-png/SISOP-1-2026-IT-079)

---

# Table of Contents

- [Shell Scripting, Cron, dan AWK](#shell-scripting-cron-dan-awk)
- [Table of Contents](#table-of-contents)
- [1. Materi Referensi Modul 1](#1-materi-referensi-modul-1)
- [2. Laporan Praktikum — Soal 1 (Kereta Argo Ngawi Jesgejes / KANJ)](#2-laporan-praktikum--soal-1-kereta-argo-ngawi-jesgejes--kanj)
  - [2.1 Deskripsi Soal](#21-deskripsi-soal)
  - [2.2 Cara Menjalankan Program](#22-cara-menjalankan-program)
  - [2.3 Penjelasan Program (`KANJ.sh`)](#23-penjelasan-program-kanjsh)
    - [a) Total Seluruh Penumpang](#a-total-seluruh-penumpang)
    - [b) Jumlah Gerbong Unik](#b-jumlah-gerbong-unik)
    - [c) Penumpang Tertua](#c-penumpang-tertua)
    - [d) Rata-rata Usia Penumpang](#d-rata-rata-usia-penumpang)
    - [e) Penumpang Business Class](#e-penumpang-business-class)
- [3. Laporan Praktikum — Soal 2 (Ekspedisi Peta Gunung Kawi)](#3-laporan-praktikum--soal-2-ekspedisi-peta-gunung-kawi)
  - [3.1 Langkah Pengerjaan](#31-langkah-pengerjaan)
  - [3.2 Script Parsing Koordinat (`parserkoordinat.sh`)](#32-script-parsing-koordinat-parserkoordinatsh)
  - [3.3 Script Titik Pusat Pusaka (`nemupusaka.sh`)](#33-script-titik-pusat-pusaka-nemupusakash)
  - [3.4 Hasil Akhir Output](#34-hasil-akhir-output)

---

## 1. Materi Referensi Modul 1

Materi lengkap mengenai **Shell Scripting**, **Cron**, dan **AWK** beserta gambar ilustrasi telah disertakan di dalam folder ini dan dapat dibaca langsung melalui tautan berikut:

- 📄 **[Baca Materi Lengkap Modul 1 (`MATERI.md`)](./MATERI.md)**
  - `1. Shell Scripting` (Apa itu Shell, Pemrograman Shell, Perintah Dasar, Variabel, Input/Output, Quoting, Operator, Percabangan, Perulangan, Function)
  - `2. Cron` (Pengertian Cron, Struktur Waktu Crontab, Penggunaan Crontab)
  - `3. AWK` (Pengertian AWK, Struktur Program AWK, Built-in Variables, Contoh Penggunaan)

---

## 2. Laporan Praktikum — Soal 1 (Kereta Argo Ngawi Jesgejes / KANJ)

📁 **Lokasi Kode:** [`./soal_1/`](./soal_1/) (`KANJ.sh`, `passenger.csv`)

### 2.1 Deskripsi Soal

Pada suatu perjalanan Kereta Argo Ngawi Jesgejes (KANJ), kondektur magang Babang Rusdi diminta untuk menganalisis data penumpang yang tersimpan dalam file `passenger.csv`. Analisis dilakukan menggunakan `awk` dalam bash script untuk menghasilkan berbagai informasi penting mengenai penumpang.

### 2.2 Cara Menjalankan Program

```bash
awk -f KANJ.sh passenger.csv [a/b/c/d/e]
```

Setiap opsi (`a`, `b`, `c`, `d`, `e`) memiliki fungsi analisis masing-masing.

### 2.3 Penjelasan Program (`KANJ.sh`)

1. **Mengunduh file `passenger.csv` terlebih dahulu:**
   ```text
   https://docs.google.com/spreadsheets/d/1NHmyS6wRO7To7ta-NLOOLHkPS6valvNaX7tawsv1zfE/export?format=csv&gid=0
   ```
   Setelah diunduh, simpan file tersebut di dalam folder `soal_1/` agar kode dapat berjalan.

2. **Implementasi tiap opsi pada `KANJ.sh`:**

#### a) Total Seluruh Penumpang

```awk
NR > 1 { count++ }
END { print "Jumlah seluruh penumpang KANJ adalah " count " orang" }
```

- **Output:**
  ```text
  Jumlah seluruh penumpang KANJ adalah 208 orang
  ```
- **Penjelasan:** `NR` (*Number of Record*) menunjukkan nomor baris yang sedang diproses. Kondisi `NR > 1` digunakan untuk melewati baris pertama (*header*), sehingga hanya data penumpang yang dihitung.

#### b) Jumlah Gerbong Unik

Menghitung jumlah gerbong unik menggunakan *associative array*:

```awk
NR > 1 { carriage[$4] = 1 }
END { print "Jumlah gerbong penumpang KANJ adalah " length(carriage) }
```

- **Output:**
  ```text
  Jumlah gerbong penumpang KANJ adalah 4
  ```
- **Penjelasan:** Data gerbong pada kolom ke-4 (`$4`) dimasukkan ke dalam array `carriage[]` sebagai *key*. Karena array hanya menyimpan *key* unik, duplikasi otomatis tereliminasi dan jumlah gerbong dihitung dengan `length(carriage)`.

#### c) Penumpang Tertua

Mencari penumpang dengan usia tertinggi:

```awk
NR > 1 {
    if ($3 + 0 > max_age + 0) {
        max_age = $3
        oldest = $2
    }
}
END { print oldest " adalah penumpang kereta tertua dengan usia " max_age " tahun" }
```

- **Output:**
  ```text
  Jaja Mihardja adalah penumpang kereta tertua dengan usia 85 tahun
  ```
- **Penjelasan:** `$3` adalah kolom usia dan `$2` adalah nama penumpang. Penambahan `+ 0` memastikan nilai dibandingkan secara numerik. Setiap kali ditemukan usia yang lebih besar dari `max_age`, variabel `max_age` dan `oldest` diperbarui.

#### d) Rata-rata Usia Penumpang

Menghitung rata-rata usia seluruh penumpang:

```awk
NR > 1 { total_age += $3; count++ }
END {
    avg = int(total_age / count)
    print "Rata-rata usia penumpang adalah " avg " tahun"
}
```

- **Output:**
  ```text
  Rata-rata usia penumpang adalah 38 tahun
  ```
- **Penjelasan:** `total_age` mengakumulasi seluruh nilai kolom usia (`$3`), sementara `count` menghitung jumlah penumpang. Rata-rata dihitung pada blok `END` dan dibulatkan menggunakan `int()`.

#### e) Penumpang Business Class

Menghitung jumlah penumpang dengan kelas `"Business"`:

```awk
NR > 1 {
    if ($5 == "Business") business++
}
END { print "Jumlah penumpang business class ada " business " orang" }
```

- **Output:**
  ```text
  Jumlah penumpang business class ada 70 orang
  ```
- **Penjelasan:** `$5` adalah kolom kelas penumpang. Blok `END` dijalankan setelah seluruh baris selesai diproses untuk menampilkan hasil akhir.

---

## 3. Laporan Praktikum — Soal 2 (Ekspedisi Peta Gunung Kawi)

📁 **Lokasi Kode:** [`./soal_2/ekspedisi/`](./soal_2/ekspedisi/)

### 3.1 Langkah Pengerjaan

1. **Mengunduh File dari Google Drive menggunakan `gdown`:**
   ```bash
   pip install gdown
   gdown "https://drive.google.com/uc?id=1q10pHSC3KFfvEiCN3V6PTroPR7YGHF6Q"
   ```
   File yang didapat berupa `peta-ekspedisi-amba.pdf`.

2. **Menyiapkan Folder Ekspedisi:**
   ```bash
   mkdir ekspedisi
   mv peta-ekspedisi-amba.pdf ekspedisi/
   ```

3. **Membaca Isi File PDF untuk Mencari Clue:**
   ```bash
   cat peta-ekspedisi-amba.pdf
   ```
   File PDF dibaca sebagai teks untuk menemukan tautan (*link*) tersembunyi menuju repositori GitHub ekspedisi.

4. **Clone Repository GitHub:**
   ```bash
   git clone <link_repo>
   cd peta-gunung-kawi
   ```
   Repositori berisi file data koordinat penting: `gsxtrack.json`.

### 3.2 Script Parsing Koordinat (`parserkoordinat.sh`)

Mengambil data `id`, `site_name`, `latitude`, dan `longitude` dari `gsxtrack.json`:

```bash
#!/bin/bash
cat gsxtrack.json \
| grep -E "node_|site_name|latitude|longitude" \
| sed 's/[",]//g' \
| awk '
/node_/ {id=$1}
/site_name/ {$1=""; name=substr($0,2)}
/latitude/ {lat=$2}
/longitude/ {
    long=$2
    print id ", " name ", " lat ", " long
}' > titik-penting.txt
```

### 3.3 Script Titik Pusat Pusaka (`nemupusaka.sh`)

Menentukan titik tengah (koordinat pusat) dari seluruh koordinat di `titik-penting.txt`:

```bash
#!/bin/bash

awk -F", " '
{
    lat += $3
    long += $4
    count++
}
END {
    print "Koordinat pusat:", lat/count ",", long/count
}' titik-penting.txt > posisipusaka.txt
```

### 3.4 Hasil Akhir Output

```text
titik-penting.txt:
node_001, Titik A, -7.xxx, 112.xxx
node_002, Titik B, -7.xxx, 112.xxx
...

posisipusaka.txt:
Koordinat pusat: -7.xxx, 112.xxx
```
