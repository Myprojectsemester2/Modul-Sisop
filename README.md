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
```c
static int x_read(...)
```
Fungsi ini digunakan untuk membaca isi file.

Isi file dari:
1.txt
2.txt
hingga 7.txt

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
```c
1.txt
2.txt
3.txt
4.txt
5.txt
6.txt
7.txt
tujuan.txt
```
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

# SOAL 2 — MINI DATABASE SERVICE
Deskripsi Soal
praktikan diminta membuat sebuah filesystem virtual menggunakan FUSE yang berfungsi sebagai translator antara direktori terenkripsi dengan mount point yang dapat diakses user secara normal.

Project yang dibuat berupa mini database service dengan struktur:
```c
database/
```
dan file CSV sebagai tabel database yang berjalan melalui TCP Connection pada port:

# 9000

Client yang terhubung nantinya dapat menjalankan beberapa perintah database seperti:
```c
CREATE
INSERT
UPDATE
DELETE
SELECT
dan LIST TABLE
```
Selain itu praktikan diminta membuat filesystem FUSE yang akan menghubungkan:

folder asli terenkripsi:
```c
encrypted_storage
```
dengan mount point:
```c
fuse_mount
```
Filesystem tersebut tidak hanya melakukan passthrough biasa, tetapi juga melakukan proses enkripsi dan dekripsi otomatis terhadap file.

Langkah Pengerjaan
1. Membuat Struktur Folder

Langkah pertama yaitu membuat struktur direktori yang diperlukan.

Folder yang digunakan:
```c
encrypted_storage/
fuse_mount/
database/
```
serta file source:
```c
fuse.c dan client.c
```
Fungsi masing-masing:

encrypted_storage → menyimpan file terenkripsi
fuse_mount → mount point filesystem
database → penyimpanan database service
fuse.c → program filesystem
client.c → program client TCP

2. Membuat Program FUSE

Program fuse.c dibuat menggunakan library FUSE untuk menghubungkan direktori terenkripsi dengan mount point.

Filesystem harus mendukung operasi:
```c
getattr
readdir
mkdir
rmdir
create
open
read
write
truncate
unlink
access
utimens
```
Filesystem bekerja seperti filesystem Linux biasa sehingga user dapat:

membuat folder,
membaca file,
menghapus file,
melihat metadata,
serta mengedit file langsung melalui mount point.
Penjelasan Program
Menggunakan FUSE
#define FUSE_USE_VERSION 31

Bagian ini digunakan untuk menentukan versi FUSE yang dipakai program.
```c
Library yang Digunakan
#include <fuse3/fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
```
Library tersebut digunakan untuk:

operasi filesystem,
membaca file,
membaca direktori,
menangani error Linux,
serta operasi sistem.
Folder Encrypted Storage

Filesystem menggunakan direktori:
```c
encrypted_storage/
```
sebagai tempat penyimpanan file asli yang telah terenkripsi.
Seluruh perubahan file:
create,
write,
delete,
rename,

akan langsung tersimpan pada direktori ini.
```c
Mount Point Fuse
```
Mount point:
```c
fuse_mount/
```
digunakan user untuk mengakses filesystem secara normal.

User dapat menjalankan:
```c
ls
cat
mkdir
rm
touch
```
langsung dari mount point.

Konsep Enkripsi dan Dekripsi
Filesystem pada soal ini bukan pure passthrough.

Filesystem bertindak sebagai translator:

file asli di encrypted_storage disimpan dalam keadaan terenkripsi,
sedangkan user melihat isi file yang sudah didekripsi melalui fuse_mount.
Proses Saat File Dibuat

Ketika user membuat file pada:
```c
fuse_mount/
```
maka filesystem akan:
menerima isi file,
mengenkripsi isi file,
menyimpan hasil terenkripsi pada:
encrypted_storage/

dengan format:
```c
.enc
Proses Saat File Dibaca
```
Saat user membuka file melalui mount point:

filesystem membaca file terenkripsi,
melakukan dekripsi,
lalu menampilkan isi asli ke user.

Dengan demikian user tetap dapat membaca file secara normal walaupun file asli terenkripsi.

Operasi getattr
```c
static int x_getattr(...)
```
Digunakan untuk mengambil atribut file seperti:
ukuran file,
permission,
metadata,
tipe file.

Fungsi ini dipanggil saat Linux melakukan pengecekan file.

Operasi readdir
```c
static int x_readdir(...)
```
Digunakan untuk membaca isi direktori.

Saat user menjalankan:
```c
ls fuse_mount
```
filesystem akan menampilkan isi direktori hasil translasi dari:
encrypted_storage
Operasi mkdir dan rmdir
mkdir
rmdir

Digunakan untuk:
membuat folder,
menghapus folder.

Perubahan otomatis diterapkan ke direktori asli.

# Operasi create
Digunakan untuk membuat file baru melalui mount point.

File yang dibuat akan langsung terenkripsi saat disimpan.

# Operasi read dan write
```c
read → membaca file
write → menulis file
```
Saat write:
data dienkripsi sebelum disimpan.

Saat read:
data didekripsi sebelum ditampilkan.

# Operasi truncate
Digunakan untuk mengubah ukuran file.

# Operasi unlink
Digunakan untuk menghapus file.

Saat file dihapus dari mount point, file terenkripsi asli juga ikut terhapus.

Operasi access dan utimens
access
utimens

Digunakan untuk:
pengecekan akses file,
pengaturan timestamp file.
Menjalankan Filesystem

Filesystem dijalankan menggunakan:
```c
./fuse encrypted_storage fuse_mount
```
Penjelasan:

encrypted_storage → source terenkripsi
fuse_mount → mount point
Hasil Pengujian Fuse

1. Membuat File

Command:
```c
echo "halo" > fuse_mount/a.txt
```
Filesystem berhasil:
membuat file,
mengenkripsi isi file,
menyimpan file .enc pada encrypted_storage.

2. Membaca File

Command:
```c
cat fuse_mount/a.txt
```
Output:
isi file tampil normal.
Hal ini menunjukkan proses dekripsi berhasil.

3. Mengecek File Asli

Saat melihat isi:
```c
encrypted_storage/
```
isi file terlihat terenkripsi dan memiliki format:
```c
.enc
```
4. Testing notes.csv.env

Praktikan diminta membuat folder:
```c
tests/
```
pada:
encrypted_storage

Kemudian file:
```c
notes.csv.env
```
dimasukkan ke folder tersebut.
Saat filesystem aktif, isi file harus dapat dibaca dari:
```c
fuse_mount/tests/
```
Hal ini menunjukkan proses translasi dan dekripsi berhasil.
```c
Containerization
```
Setelah filesystem berhasil dibuat, tahap berikutnya yaitu menjalankan mini database service menggunakan Docker.

Base image yang digunakan:
```c
ubuntu:latest
```
Program database disalin ke:
```c
/app

Kemudian port:

9000
```
diexpose agar dapat diakses client.

Build Docker Image
Image dibuild menggunakan:
```c
docker build -t soal-2-modul-4-sisop .
```
Penjelasan:

docker build → membuat image
-t → memberi nama image
Menjalankan Container

Container dijalankan menggunakan:
docker run
Container berjalan pada background dan menggunakan bind mount menuju:
```c
fuse_mount
Integration Client
```
Setelah container aktif, dibuat program:
```c
client.c
```
untuk melakukan komunikasi TCP dengan server database.

Client dapat:
membuat database,
membuat tabel,
melakukan insert,
melihat isi tabel,
serta menjalankan command database lainnya.
Pengujian Client

Saat client dijalankan:
koneksi TCP berhasil,
command database berhasil dikirim,
hasil query berhasil diterima.

# Kesimpulan

Pada soal kedua ini berhasil dibuat filesystem virtual menggunakan FUSE yang berfungsi sebagai translator antara:

direktori terenkripsi,
dan mount point normal.

Filesystem berhasil:

membaca direktori,
membuat file,
membaca file,
mengenkripsi file,
mendekripsi file,
serta menyinkronkan seluruh perubahan dengan direktori asli.

