# LAPORAN PRAKTIKUM SISTEM OPERASI – SOAL 1

Implementasi Client-Server (wired.c dan navi.c)

1. Tujuan

Membuat sistem komunikasi client-server menggunakan:

Socket (sebagai IPC)
Mekanisme penanganan banyak client (menggunakan select)
Fitur admin (RPC sederhana)
Logging aktivitas
2. Gambaran Sistem

Sistem terdiri dari:

Server (wired.c) → pusat komunikasi
Client (navi.c) → pengguna

Semua client terhubung ke server, dan server akan:

menerima pesan
mengelola user
menyebarkan pesan ke client lain
3. Penjelasan Berdasarkan Poin Soal
3.1 Server dapat menangani banyak client

Digunakan fungsi:

select()

Fungsi ini memungkinkan server:

memantau banyak socket sekaligus
mengetahui client mana yang mengirim data

Tanpa select, server hanya bisa melayani satu client dalam satu waktu.

3.2 Username harus unik

Saat client masuk:
```c
if (strcmp(clients[i].name, name) == 0)
```
Server membandingkan username baru dengan semua username yang sudah ada.
Jika sama:

koneksi ditolak
socket ditutup
3.3 Broadcast pesan

Ketika client mengirim pesan:
```c
send(clients[j].socket, msg, strlen(msg), 0);
```
Server akan:

mengirim pesan ke semua client lain
kecuali pengirim
3.4 Admin (The Knights)

Admin dikenali dengan:
```c
Username: The Knights
Password: 001122
```
Jika login berhasil:

user diberi hak admin
dapat menjalankan perintah khusus
3.5 RPC (Remote Procedure Call)

Admin dapat mengirim perintah:

1 → melihat jumlah user
2 → melihat uptime server
3 → shutdown server
4 → disconnect

Implementasi:
```c
strncmp(buffer, "1", 1)
```
Server membaca input dan menjalankan fungsi tertentu.

3.6 Logging

Semua aktivitas dicatat ke file:
```c
history.log
```
Implementasi:
```c
FILE *f = fopen("history.log", "a");
fprintf(f, "[%s] %s\n", type, msg);
```
4. Edge Case dan Penanganannya
4.1 Username duplikat

Masalah:
Dua client menggunakan username yang sama.

Solusi:
```c
if (exists)
```
Koneksi ditolak agar tidak terjadi konflik.

4.2 Server penuh

Jika jumlah client mencapai batas maksimum:
```c
if (slot == -1)
```
Client baru tidak diterima.

4.3 Client disconnect

Jika client keluar:
```c
if (valread == 0)
```
Solusi:
```c
close(sd);
clients[i].socket = 0;
```
Slot dikosongkan agar bisa dipakai client lain.

4.4 Password admin salah

Jika password tidak cocok:
```c
strcmp(pass, ADMIN_PASS)
```
Akses admin ditolak.

4.5 Buffer overflow

Masalah:
Input terlalu panjang dapat merusak memori.

Solusi:

batasi ukuran buffer
gunakan fungsi aman seperti snprintf

# 5. Penjelasan Kode

 Struktur Client
```c
struct Client {
    int socket;
    char name[50];
    int isAdmin;
};
```
Digunakan untuk menyimpan:

socket client
username
status admin

5.2 Array Client
```c
struct Client clients[MAX_CLIENTS];
```
Menyimpan semua client yang aktif.

5.3 Socket
```c
socket(AF_INET, SOCK_STREAM, 0);
```
Membuat jalur komunikasi antara server dan client.

5.4 Bind
```c
bind(server_fd, ...);
```
Menghubungkan socket dengan port.

5.5 Listen
```c
listen(server_fd, 5);
```
Server siap menerima koneksi.

5.6 Accept
```c
accept(...)
```
Menerima koneksi client baru.

5.7 Select
select(...)
```
Mengawasi semua socket aktif.

5.8 Read
```c
read(sd, buffer, 1024);

Menerima pesan dari client.

5.9 Send
```c
send(...)
```

Mengirim pesan ke client.

5.10 Logging
```c
fprintf(...)
```
Menyimpan aktivitas ke file.

6. Kode Utama (wired.c)
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080
#define MAX_CLIENTS 10

struct Client {
    int socket;
    char name[50];
};

struct Client clients[MAX_CLIENTS];

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    fd_set readfds;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Server running...\n");

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);

        select(server_fd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &readfds)) {
            new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            printf("Client connected\n");
        }
    }

    return 0;
}
```
7. Kesimpulan

Program yang dibuat:

mampu menangani banyak client
memastikan username unik
menyediakan komunikasi antar client
memiliki sistem admin dengan command
mencatat aktivitas ke file log
menangani berbagai kondisi error

Dengan pendekatan ini, sistem menjadi stabil dan sesuai dengan kebutuhan soal.

# SOAL 2 – SISTEM ARENA (CLIENT-SERVER GAME)
mensimulasikan game implemen IPC dan thread 

Struktur program:
```c
orion.c sebagai server
eternal.c sebagai client
arena.h sebagai definisi struktur data
Makefile untuk kompilasi dan manajemen IPC

Cara compile dan Menjalankan
```c
make
```
Menjalankan Server
```c
./orion
./eternal
```
1. REGISTER DAN LOGIN SYSTEM
Tujuan

Membuat sistem autentikasi pengguna yang aman dan mencegah duplikasi data serta login ganda.

Cara Kerja
Register

Pengguna memasukkan username dan password. Sistem akan:

Mengecek apakah username sudah digunakan
Jika belum, data disimpan ke shared memory
Login

Pengguna memasukkan username dan password. Sistem akan:

Memverifikasi kecocokan data
Mengecek apakah akun sedang digunakan oleh client lain
Struktur Data
```c
typedef struct {
    char username[50];
    char password[50];
    int gold;
    int level;
    int xp;
    int weapon;
    int online;
} Player;
```
Kendala
Duplikasi username
Login bersamaan dari dua client

Solusi
Validasi username saat register
Menggunakan flag online untuk mencegah login ganda
Mutex untuk sinkronisasi akses data

2. MATCHMAKING SYSTEM
Tujuan

Menghubungkan dua pemain untuk bertarung dalam satu arena.

Cara Kerja
Client mengirim request matchmaking ke server menggunakan message queue
Server menyimpan player pertama sebagai waiting
Ketika player kedua masuk, server memasangkan keduanya
Server mengirim pesan ke kedua client bahwa match ditemukan
Mekanisme
```c
Menggunakan message type MT_MATCH_REQ dan MT_MATCH_FOUND
Server menyimpan sementara ID player yang menunggu
Timeout
```
Jika tidak ditemukan lawan dalam waktu tertentu, sistem akan mengarahkan pemain untuk melawan bot.

Kendala
Player bisa masuk lebih dari satu matchmaking
Sinkronisasi antar player tidak konsisten
Solusi
Menggunakan flag in_match
Mengunci shared memory saat proses matchmaking

3. BATTLE SYSTEM (REALTIME)
Membuat sistem pertarungan antar pemain secara realtime tanpa blocking.

Cara Kerja
Input
```c
a untuk serangan biasa
u untuk ultimate
Perhitungan Damage
Damage = 10 + (XP / 50) + Weapon

Ultimate:

Damage = Damage × 3
Perhitungan HP
HP = 100 + (XP / 10)
Realtime System
```
Battle dijalankan menggunakan multithreading:

Thread utama menangani input user
Thread kedua menerima damage dari lawan
Komunikasi

Damage dikirim melalui message queue dari satu client ke client lain melalui server.


4. ARMORY SYSTEM
 fitur pembelian senjata untuk meningkatkan kekuatan pemain.

Cara Kerja

Player dapat membeli:
Wood Sword dengan tambahan damage kecil
Iron Sword dengan tambahan damage lebih besar
Mekanisme
Sistem mengecek jumlah gold sebelum pembelian
Weapon akan diperbarui hanya jika lebih kuat dari sebelumnya
Contoh
```c
if(me->weapon < 15) me->weapon = 15;
Kendala
Gold menjadi negatif
Weapon tertimpa oleh nilai lebih kecil
Solusi
Validasi gold sebelum pembelian
Bandingkan nilai weapon sebelum update
```
