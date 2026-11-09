 Laporan Resmi Praktikum Sistem Operasi Modul 1

Nama : Raffa Al Azmi
NRP : 5027251079

SOAL 1
Deskripsi soal :
Pada suatu perjalanan Kereta Argo Ngawi Jesgejes (KANJ), kondektur magang Babang Rusdi diminta untuk menganalisis data penumpang yang tersimpan dalam file passenger.csv.
Analisis dilakukan menggunakan awk dalam bash script untuk menghasilkan berbagai informasi penting mengenai penumpang.

CARA MENJALANKAN PROGRAM SOAL_1
```c
awk -f KANJ.sh passenger.csv [a/b/c/d/e]
```
setiap opsi memiliki fungsi masing - masing

Penjelasan Prgoram
1. mendownload file passenger terlebih dahulu
```c
https://docs.google.com/spreadsheets/d/1NHmyS6wRO7To7ta-NLOOLHkPS6valvNaX7tawsv1zfE/export?format=csv&gid=0
```
setelah itu jadikan file tersebut sebagai file dalam folder
supaya dapat berjalan codenya

2. membuat file KANJ.sh
   dengan code seperti dibawah
   
```c
NR > 1 { count++ }
END { print "Jumlah seluruh penumpang KANJ adalah " count " orang" }
```
Output:
Jumlah seluruh penumpang KANJ adalah 208 orang

NR (Number of Record) menunjukkan nomor baris yang sedang diproses untuk Kondisi NR > 1 digunakan untuk melewati baris pertama (header).
Artinya, hanya data penumpang saja yang diproses.

 b) Jumlah Gerbong Unik

Menghitung jumlah gerbong unik menggunakan array.
```c
NR > 1 { carriage[$4] = 1 }
END { print "Jumlah gerbong penumpang KANJ adalah " length(carriage) }
```
Output:
Jumlah gerbong penumpang KANJ adalah 4

Data gerbong dimasukkan ke dalam array carriage[] sebagai key Karena array hanya menyimpan key unik, maka otomatis tidak ada duplikasi.
Nanti jumlah gerbong dihitung dengan length(carriage)

🔹 c) Penumpang Tertua
Mencari penumpang dengan usia tertinggi.
```c
NR > 1 {
    if ($3 + 0 > max_age + 0) {
        max_age = $3
        oldest = $2
    }
}
END { print oldest " adalah penumpang kereta tertua dengan usia " max_age " tahun" }
```
Output:
Jaja Mihardja adalah penumpang kereta tertua dengan usia 85 tahun

$3 adalah kolom usia, $2 adalah nama, + 0 digunakan agar nilai dibaca sebagai angka (numerik).
Program membandingkan setiap usia dengan max_age.
Jika lebih besar:
max_age diperbarui
oldest menyimpan nama penumpang tersebut

🔹 d) Rata-rata Usia
Menghitung rata-rata usia seluruh penumpang.
```c
NR > 1 { total_age += $3; count++ }
END {
    avg = int(total_age / count)
    print "Rata-rata usia penumpang adalah " avg " tahun"
}
```
Output:
Rata-rata usia penumpang adalah 38 tahun

$3 adalah usia.
total_age menyimpan jumlah seluruh usia, count menghitung jumlah penumpang.
Nantinya rata-rata dihitung di bagian END.

🔹 e) Penumpang Business Class
Menghitung jumlah penumpang dengan kelas "Business".
```c
NR > 1 {
    if ($5 == "Business") business++
}
END { print "Jumlah penumpang business class ada " business " orang" }
```
Output:
Jumlah penumpang business class ada 70 orang

$5 adalah kolom kelas penumpang.
Program mengecek apakah nilainya "Business". Jika iya, maka business akan bertambah.

```c
END {
```
END dijalankan setelah semua data selesai diproses.
Digunakan untuk:
- Menampilkan hasil akhir
- Menghitung nilai akhir seperti rata-rata
Semua output program akan ditampilkan di bagian ini.

-------------------------------------------------------------------
soal 2
Langkah 1
Mengunduh File dari Google Drive Menggunakan tools gdown untuk mendownload file:
```c
pip install gdown
gdown "(https://drive.google.com/uc?id=1q10pHSC3KFfvEiCN3V6PTroPR7YGHF6Q)e"
```
File yang didapat berupa:
peta-ekspedisi-amba.pdf

 2. Menyiapkan Folder Ekspedisi
```c
mkdir ekspedisi
mv peta-ekspedisi-amba.pdf ekspedisi/
```
File dipindahkan agar lebih terorganisir.

 3. Membaca Isi File PDF
```c
cat peta-ekspedisi-amba.pdf
```
File PDF dibaca sebagai teks untuk mencari clue tersembunyi.
Dari sini ditemukan sebuah tautan (link) menuju repository.

 4. Clone Repository GitHub
```c
git clone <link_repo>
cd nama_repo
```
Repository berisi file penting:
gsxtrack.json

5. parsing file JSON
   mengambil data (id,site_name,latitude,longitude)

6. isi parsekoordinat.sh
```c
   !/bin/bash
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
7. menentukan titik tengah dari koordinat
```c
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
hasil akhir output
```c
titik-penting.txt:
node_001, Titik A, -7.xxx, 112.xxx
node_002, Titik B, -7.xxx, 112.xxx
...

posisipusaka.txt:
Koordinat pusat: -7.xxx, 112.xxx
```
