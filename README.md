# SISOP-1-2026-IT-07
Repository ini berisi penyelesaian Modul 1 Praktikum Sistem Operasi.

## Soal 1

🧾 Deskripsi Soal

Pada soal ini, praktikan diminta untuk menghitung jumlah seluruh penumpang pada file passenger.csv.

Ketentuan:

Data berbentuk file CSV
Baris pertama adalah header (tidak dihitung)
Hasil harus ditampilkan dengan format:
Jumlah seluruh penumpang KANJ adalah X orang

### Cara Menjalankan Program

📌 Script: KANJ.sh
BEGIN{
    option = ARGV[2]
    ARGV[2] = ""
}

NR>1{
    count_passenger++
}

END{
    if(option=="a"){
        print "Jumlah seluruh penumpang KANJ adalah " count_passenger " orang"
    }
}

🔍 Penjelasan Script

1. BEGIN Block
option = ARGV[2]
ARGV[2] = ""
ARGV[2] digunakan untuk mengambil input opsi (a/b/c/d/e)
Dihapus agar tidak dianggap sebagai file oleh awk

2. Proses Data
NR>1{
    count_passenger++
}
NR = nomor baris
NR > 1 digunakan untuk melewati header
Setiap baris data akan dihitung sebagai 1 penumpang

3. END Block
if(option=="a"){
    print "Jumlah seluruh penumpang KANJ adalah " count_passenger " orang"
}
Menampilkan hasil akhir setelah semua data dibaca
▶️ Cara Menjalankan Program
awk -f KANJ.sh passenger.csv a

Penjelasan:
- -f KANJ.sh → menjalankan file awk
- passenger.csv → file input
- a → memilih soal bagian a

### Struktur Repository

SISOP-1-2026-IT-079
│
├── README.md
│
└── soal1.c
    ├── KANJ.sh
    └── passenger.csv

--------------------------------------------------------------------------------------------------------------------
📌 Soal 2 - Ekspedisi Peta Gunung Kawi
📖 Deskripsi

Pada soal ini dibuat sistem berbasis shell script untuk mengolah data koordinat dari file JSON guna menentukan lokasi pusaka berdasarkan titik-titik penting.

📂 Struktur Folder
soal_2/
└── ekspedisi/
    ├── gsxtrack.json
    ├── parserkoordinat.sh
    ├── nemupusaka.sh
    ├── titik-penting.txt
    └── posisipusaka.txt

⚙️ Cara Kerja Program
- Parsing Data JSON
- File gsxtrack.json diproses menggunakan parserkoordinat.sh
- Mengambil data koordinat penting
- Output disimpan ke titik-penting.txt
- Menentukan Lokasi Pusaka
- Script nemupusaka.sh membaca titik-penting.txt
- Menghitung pusat koordinat
- Menentukan lokasi pusaka
- Output disimpan ke posisipusaka.txt
▶️ Cara Menjalankan

Masuk ke folder:

cd soal_2/ekspedisi

Jalankan:

 bash parserkoordinat.sh
 bash nemupusaka.sh

🧠 Alur Program
 JSON → Parsing → titik penting
 titik penting → perhitungan → posisi pusaka

