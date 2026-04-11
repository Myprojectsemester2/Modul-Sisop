SOAL 1 
---------------------------------------------------------
membantu uncle muthu untuk mendata data hutang pelanggannya karena datanya terkena virus, sekaligus membantu untuk memisahkan data pelanggan yang sudah belum lunas

Cara 
1. kita mendownload file/data terlebih dahulu
    ```c
    https://drive.google.com/uc?export=download&id=144hsFJGoAM5lbvBQjkTfZaD3GOil4yEw
    ```
2. membuat folder kasir_muthu untuk mengamankan data, dengan coddingan
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

    // langsung membuat brankas_kedai [child proses]
    pid = fork();  //membuat child proses dari parent
    if (pid == 0) {
        execlp("mkdir", "mkdir", "brankas_kedai", NULL);
        exit(EXIT_FAILURE);
    } else {  // parent proses
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0) error_exit();
    }

    // membuat cp buku_hutang.csv ke brankas
    pid = fork();
    if (pid == 0) {
        execlp("cp", "cp", "buku_hutang.csv", "brankas_kedai/", NULL);
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0) error_exit();
    }

    // grep "belum lunaske daftar penunggak.txt"
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

    // zip brankas jadi rahasia_muthu.zip
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
penjelasan singkat 
- fork() → bikin child process (Ipin)
- execlp() → jalankan command Linux
- waitpid() → parent (Upin) nunggu child selesai
- WEXITSTATUS(status) → cek berhasil

Output 
jIka berhasil
 ```c
[INFO] Fuhh, selamat! Buku hutang dan daftar penagihan berhasil diamankan.
 ```
jika gagal
 ```c
[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.
 ```

3. Run program
 ```c
gcc kasir_muthu.c -o kasir
./kasir
 ```
nanti akan muncul folder brankas_kedai, file daftar_penunggak.txt, file rahasia_muthu.zip

Setelah setiap child process selesai, program menggunakan waitpid() dan WEXITSTATUS untuk memastikan eksekusi berhasil. Jika gagal, program langsung dihentikan dengan exit(). Jika semua proses berhasil, program menampilkan pesan sukses.”

1. Menjalankan Program
 ```c
gcc kasir_muthu.c -o kasir
./kasir
 ```
Output:
 ```c
[INFO] Fuhh, selamat! Buku hutang dan daftar penagihan berhasil diamankan.
 ```
 2. Struktur Folder Setelah Eksekusi
 ```c
ls
 ```
Output:
 ```c
brankas_kedai  buku_hutang.csv  kasir_muthu.c  rahasia_muthu.zip
 ```
 3. Isi Folder Brankas
```c
ls brankas_kedai
 ```
Output:
```c
buku_hutang.csv  daftar_penunggak.txt
 ```
 4. Hasil Filter Data (Belum Lunas)
```c
cat brankas_kedai/daftar_penunggak.txt
 ```
Output:
```c
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
 5. File ZIP yang Dihasilkan
```c
ls rahasia_muthu.zip
 ```
Output:
```c
rahasia_muthu.zip
 ```
 6. Isi File CSV Awal
```c
cat buku_hutang.csv
 ```
Output:
```c
Nama Pelanggan,Pesanan,Total (RM),Status
Mail,Ayam Goreng 2 Paha,10.50,Belum Lunas
Ehsan,Kue Inti 5 Biji,5.00,Lunas
 ```
--------------------------------------------------------------------------------------------------------------------------------------------------------------------

## SOAL 2
Program ini merupakan implementasi daemon process dalam bahasa C yang berjalan di background untuk memonitor dan menjaga integritas file contract.txt.

Daemon akan:
- Menulis log secara berkala
- Membuat file otomatis
- Mendeteksi penghapusan dan perubahan file
- Melakukan restore
- Menangani proses termination

Cara kerjanya
Inisialisasi Daemon

Program dijalankan sebagai daemon dengan:
- fork() untuk membuat child process
- setsid() untuk detach dari terminal

 2. Logging Berkala (Setiap 5 Detik)

Daemon menulis ke file:
```c
work.log
 ```
Format:
```c
still working...
[awake/drifting/numbness]
 ```
Status dipilih secara acak.

🔹 3. Membuat File Contract (Saat Program Dimulai)

File:
```c
contract.txt
 ```
Isi:
```c
A promise to keep going, even when unseen.
created at: <timestamp>
 ```
4. Deteksi File Dihapus (Auto Restore)

Jika:
contract.txt dihapus maka daemon akan:

Membuat ulang file dalam 1–2 detik
Mengubah baris kedua menjadi:
```c
restored at: <timestamp>
 ```
🔹 5. Deteksi Perubahan File

Jika isi file diubah Daemon akan menulis:
```c
contract violated.
ke work.log
 ```
File akan otomatis dikembalikan (restore)
🔹 6. Saat Daemon Dihentikan

Jika daemon dihentikan menggunakan:
```c
kill <PID>
 ```
Maka akan menulis:
```c
We really weren't meant to be together
 ```
📁 Struktur File
soal_2/
├── contract_daemon.c
├── daemon
├── contract.txt
└── work.log

🚀 Cara Menjalankan
Compile
```c
gcc contract_daemon.c -o daemon
 ```
Run (background)
```c
./daemon &
 ```
Stop daemon
```c
ps aux | grep daemon
kill <PID>
 ```
📊 Hasil Eksekusi
1. Logging Berjalan
```c
cat work.log
 ```
Output:
```c
still working...
[awake]
still working...
[drifting]
 ```
2. File Contract Dibuat
```c
cat contract.txt
 ```
Output:
```c
A promise to keep going, even when unseen.
created at: 2026-04-07 14:30:00
 ```
3. File Dihapus → Restore
```c
rm contract.txt
cat contract.txt
 ```
Output:
```c
A promise to keep going, even when unseen.
restored at: 2026-04-07 14:32:10
 ```
4. File Diubah → Violation
```c
nano contract.txt
 ```
Tambahkan teks:
```c
HACKED 😈
 ```
Cek log:
```c
cat work.log
 ```
Output:
```c
contract violated.
 ```
5. Daemon Dihentikan
```c
kill <PID>
cat work.log
 ```
Output:
```c
We really weren't meant to be together
 ```
Konsep yang Digunakan
Daemon Process (fork, setsid)
File Monitoring
Signal Handling (SIGTERM)
File I/O (fopen, fgets, fwrite)
Infinite Loop Process
Random Number (rand())
---------------------------------------------------------------------------------------------------------------------------------------------------------
## SOAL 3 (ANGEL DAEMON)

alur dalam pengerjaan :
     1. Program dijalankan dengan `-daemon`
     2. Membuat proses background (fork + setsid)
     3. Mengganti nama proses menjadi `maya`
     4. Loop:
             - generate kalimat
             -  encode
             - simpan log
     5. `-decrypt` untuk decode
     6. `-kill` untuk menghentikan


dalam Program `angel.c` merupakan kita meng implementasi demon yang nama maya lalu kita mengcomplie supaya keluar LoveLetter.txt,ethereal log
dalam LoveLetter.txt berisi kata kata acak yang tidak bisa dibaca lalu saat di ./angel -decrpyt maka kalimat akan menujukan seacara acak dari kalimat ini 
    "aku akan fokus pada diriku sendiri",
    "aku mencintaimu dari sekarang hingga selamanya",
    "aku akan menjauh darimu, hingga takdir mempertemukan kita di versi kita yang terbaik.",
    "kalau aku dilahirkan kembali, aku tetap akan terus menyayangimu"
untuk mengecek jalannya program kita menggunakan ethereal.log

### Cara Menjalankan & Output
 Compile terlebih dahulu

```bash
gcc angel.c -o angel
./angel -daemon
```
### ✅ Output (tidak ada output langsung karena daemon)

Program berjalan di background

### 🔍 Cek proses:

```bash
ps aux | grep maya
```

### Output:

```bash
rappa     1021  0.0  0.0   2684  1152 ?   Ss   23:29   0:00 maya
```

##  2. Hasil File (Setelah Daemon Jalan) akan muncul beberapa file

```bash
ls
```

### ✅ Output:

```bash
LoveLetter.txt  angel  angel.c  ethereal.log
```

---

###  Isi File saat belum dicompile dan dijalankan

```bash
cat LoveLetter.txt
```

###  Output:

```bash
YWt1IG1lbmNpbnRhaW11IGRhcmkgc2VrYXJhbmcgaGluZ2dhIHNlbGFtYW55YQ==
```

 Isi ini berubah setiap ±10 detik saat kita complie menggunakan
Decrypt File

```bash
./angel -decrypt
```

### Output:

```bash
Decrypt sukses
```

### 🔍 Cek isi file:

```bash
cat LoveLetter.txt
```

### ✅ Output:

```bash
aku mencintaimu dari sekarang hingga selamanya
```
dan sertiap detik akan berubah secara random
---

##  4. Menghentikan Daemon

```bash
./angel -kill
```

### ✅ Output:

```bash
Daemon dimatikan
```

---

## 📝 5. File Log

```bash
cat ethereal.log
```

###  Output:
```bash
[10:04:2026]-[23:40:01]_compose_RUNNING
[10:04:2026]-[23:40:01]_compose_DONE
[10:04:2026]-[23:40:01]_encode_RUNNING
[10:04:2026]-[23:40:01]_encode_DONE
```

Log akan terus bertambah setiap 10 detik

---

