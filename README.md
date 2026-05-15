## SISOP MODUL 4
SOAL 1 — KENZ RESCUE
Deskripsi Soal

Pada soal pertama modul 4 ini, praktikan diminta membuat sebuah filesystem virtual menggunakan FUSE dengan nama kenz_rescue. Filesystem tersebut berfungsi sebagai perantara antara folder asli amba_files dengan folder mount mnt. Seluruh isi file pada folder sumber harus dapat diakses melalui mount point tanpa mengubah isi file aslinya.
Selain melakukan mirror terhadap file asli, filesystem juga diminta menambahkan sebuah file virtual bernama tujuan.txt. File tersebut tidak boleh benar-benar ada pada folder amba_files, tetapi harus muncul ketika filesystem dijalankan melalui mount point.

Isi dari tujuan.txt diperoleh dengan cara membaca seluruh file 1.txt sampai 7.txt, kemudian mengambil bagian teks yang diawali dengan prefix:
```c
KOORD:
```
Seluruh fragmen koordinat tersebut kemudian digabung sesuai urutan file sehingga menghasilkan lokasi tujuan Mas Amba.

Filesystem yang dibuat harus dapat:

membaca isi direktori,
membuka file,
membaca isi file,
membuat file virtual,
serta menampilkan isi file melalui mount point.
Langkah Pengerjaan
1. Membuat Struktur Folder

Tahap pertama yaitu menyiapkan struktur direktori yang diperlukan.

Folder yang digunakan:
```c
amba_files/
mnt/
```
Selain itu dibuat juga file source program:
```c
kenz_rescue.c
```
Fungsi masing-masing:

amba_files → folder sumber file asli
mnt → mount point filesystem
kenz_rescue.c → source code FUSE

2. Menyiapkan File Sumber

Selanjutnya file:
1.txt
2.txt
3.txt
4.txt
5.txt
6.txt
7.txt

dimasukkan ke dalam folder:
```c
amba_files/
```
File-file tersebut berisi catatan ekspedisi Mas Amba dan masing-masing memiliki fragmen koordinat.

3. Mengekstrak Arsip ZIP

Arsip zip diekstrak menggunakan command:
```c
unzip amba_files.zip
```
Setelah proses ekstraksi selesai, file zip dihapus sesuai ketentuan soal menggunakan:
```c
rm -rf amba_files.zip
```
Hal ini dilakukan agar working directory hanya menyisakan folder hasil ekstraksi.

4. Membuat Program FUSE

Program kenz_rescue.c dibuat menggunakan library FUSE. Program bertugas:

membaca file dari source directory,
menampilkan isi file pada mount point,
membuat file virtual,
serta menggabungkan koordinat dari seluruh file.

Filesystem bekerja menggunakan beberapa callback utama seperti:

getattr
readdir
open
read
Penjelasan Program
Menggunakan Versi FUSE
```c
#define FUSE_USE_VERSION 31
```
Bagian ini digunakan untuk menentukan versi FUSE yang dipakai pada program.

Library yang Digunakan
```c
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
Library tersebut digunakan untuk:

operasi filesystem,
membaca file,
membaca direktori,
menangani error,
serta operasi sistem Linux.
Variabel Global
static char source_dir[1024];

Variabel ini digunakan untuk menyimpan lokasi folder sumber yaitu amba_files.

Fungsi Full Path
```c
static void fullpath(char fpath[1024], const char *path)
```
Fungsi ini bertugas menggabungkan:

path folder sumber,
dengan path file dari mount point.

Contoh:
```c
mnt/1.txt
akan diarahkan menuju:
amba_files/1.txt
Fungsi getattr
static int x_getattr(...)
```
Fungsi ini digunakan untuk mengambil atribut file seperti:

ukuran file,
permission,
tipe file,
metadata file.

Fungsi ini penting karena Linux selalu memeriksa atribut file sebelum file dibuka.

# Fungsi readdir
```c
static int x_readdir(...)
```
Digunakan untuk membaca isi direktori.

Saat user menjalankan:
```c
ls mnt
```
fungsi ini akan:

membaca isi folder asli,
lalu menampilkan seluruh file pada mount point.

Selain file asli, fungsi ini juga menampilkan:
```c
tujuan.txt
```
sebagai file virtual.

# Fungsi open
```c
static int x_open(...)
```
Digunakan untuk membuka file yang diakses user.

Contohnya saat user menjalankan:
```c
cat mnt/1.txt
```
filesystem akan membuka file asli dari source directory.

# Fungsi read
static int x_read(...)

Fungsi ini digunakan untuk membaca isi file.

Isi file dari:
```c
1.txt
2.txt
hingga 7.txt
```
ditampilkan melalui mount point tanpa mengubah isi aslinya.

File Virtual tujuan.txt

Filesystem juga menambahkan file virtual:

tujuan.txt

File ini:

hanya muncul di mount point,
tidak ada secara fisik pada amba_files.

Ketika user menjalankan:
```c
cat mnt/tujuan.txt
```
program akan:

membaca seluruh file,
mengambil baris dengan prefix KOORD:,
menggabungkan fragmen koordinat,
menampilkan hasil akhir.
Struktur Fuse Operations
static struct fuse_operations x_oper

Bagian ini digunakan untuk menghubungkan operasi FUSE dengan fungsi yang telah dibuat.

Contohnya:

getattr → fungsi mengambil atribut file
readdir → fungsi membaca direktori
open → fungsi membuka file
read → fungsi membaca isi file
Fungsi Main
int main(int argc, char *argv[])

Merupakan fungsi utama program.

Fungsi ini bertugas:

menerima argumen program,
membaca source directory,
menentukan mount point,
menjalankan filesystem FUSE.
Compile Program

Program di-compile menggunakan command:
```c
gcc kenz_rescue.c `pkg-config fuse3 --cflags --libs` -o kenz_rescue
```
Penjelasan:

gcc → compiler bahasa C
pkg-config → mengambil konfigurasi library FUSE
-o → menentukan nama output program
Menjalankan Filesystem

Filesystem dijalankan menggunakan:
```c
./kenz_rescue amba_files mnt
```
Penjelasan:

amba_files → source directory
mnt → mount point
Hasil Pengujian
1. Mengecek Isi Mount Point

Command:
```c
ls mnt
```
Output:

1.txt
2.txt
3.txt
4.txt
5.txt
6.txt
7.txt
tujuan.txt

Hal ini menunjukkan filesystem berhasil melakukan mirror file serta menambahkan file virtual.

2. Mengecek Isi File

Command:
```c
cat mnt/1.txt
```
menghasilkan isi yang sama persis dengan:
```c
cat amba_files/1.txt
```
Artinya proses passthrough berhasil dilakukan.

3. Mengecek tujuan.txt

Command:
```c
cat mnt/tujuan.txt
```
Output menampilkan hasil gabungan koordinat dari seluruh file.

Hal ini menunjukkan bahwa filesystem berhasil membuat isi file secara dinamis.

4. Memastikan tujuan.txt Tidak Ada di Folder Asli

Command:
```c
ls amba_files
```
Output hanya berisi:

1.txt
2.txt
3.txt
4.txt
5.txt
6.txt
7.txt

Tidak terdapat:
```c
tujuan.txt
```
Hal ini membuktikan bahwa file tersebut merupakan virtual file.
