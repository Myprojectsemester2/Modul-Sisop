# SISOP-1-2026-IT-079

Repository ini berisi penyelesaian Modul 1 Praktikum Sistem Operasi.

## Soal 1

Program ini menggunakan AWK untuk menganalisis data penumpang kereta KANJ yang terdapat pada file `passenger.csv`.

Script yang digunakan adalah `KANJ.sh`.

### Cara Menjalankan Program

Masuk ke folder soal1.c

cd soal1.c

Menampilkan jumlah seluruh penumpang

awk -f KANJ.sh passenger.csv a

Menampilkan jumlah gerbong kereta

awk -f KANJ.sh passenger.csv b

Menampilkan penumpang dengan usia tertua

awk -f KANJ.sh passenger.csv c

Menampilkan rata-rata usia penumpang

awk -f KANJ.sh passenger.csv d

Menampilkan jumlah penumpang business class

awk -f KANJ.sh passenger.csv e

Jika input tidak sesuai maka akan muncul pesan:

Soal tidak dikenali. Gunakan a, b, c, d, atau e.

### Struktur Repository

SISOP-1-2026-IT-079
│
├── README.md
│
└── soal1.c
    ├── KANJ.sh
    └── passenger.csv
