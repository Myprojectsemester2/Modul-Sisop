<div align="center">

# Thread, IPC, dan RPC

</div>

**Thread**, **Inter-Process Communication (IPC)**, dan **Remote Procedure Call (RPC)** memungkinkan eksekusi konkuren serta pertukaran data antar-proses maupun antar-layanan jaringan secara efisien.

> 📖 **Akses Materi Lengkap:** [Buka MATERI.md (Thread, IPC, dan RPC)](./MATERI.md)  
> 👤 **Praktikan:** Raffa Al Azmi (`5027251079`) — [Repositori Asal: `SISOP-3-2026-IT-079`](https://github.com/raffaalazmi-png/SISOP-3-2026-IT-079)

---

# Table of Contents

- [Thread, IPC, dan RPC](#thread-ipc-dan-rpc)
- [Table of Contents](#table-of-contents)
- [1. Materi Referensi Modul 3](#1-materi-referensi-modul-3)
- [2. Laporan Praktikum — Soal 1 (Client-Server `wired.c` & `navi.c`)](#2-laporan-praktikum--soal-1-client-server-wiredc--navic)
  - [2.1 Tujuan & Gambaran Sistem](#21-tujuan--gambaran-sistem)
  - [2.2 Penjelasan Berdasarkan Poin Soal](#22-penjelasan-berdasarkan-poin-soal)
  - [2.3 Edge Case dan Penanganannya](#23-edge-case-dan-penanganannya)
  - [2.4 Penjelasan Struktur & Komponen Kode](#24-penjelasan-struktur--komponen-kode)
  - [2.5 Kode Utama (`wired.c`)](#25-kode-utama-wiredc)
- [3. Laporan Praktikum — Soal 2 (Sistem Arena Client-Server Game)](#3-laporan-praktikum--soal-2-sistem-arena-client-server-game)
  - [3.1 Struktur Program & Cara Menjalankan](#31-struktur-program--cara-menjalankan)
  - [3.2 Register dan Login System (Shared Memory)](#32-register-dan-login-system-shared-memory)
  - [3.3 Matchmaking System (Message Queue)](#33-matchmaking-system-message-queue)
  - [3.4 Battle System Realtime (Multithreading)](#34-battle-system-realtime-multithreading)
  - [3.5 Armory System](#35-armory-system)

---

## 1. Materi Referensi Modul 3

Materi lengkap mengenai **Thread (POSIX Threads)**, **IPC (Pipes, Message Queues, Shared Memory, Sockets)**, dan **RPC** beserta diagram ilustrasi tersedia di dalam folder ini:

- 📄 **[Baca Materi Lengkap Modul 3 (`MATERI.md`)](./MATERI.md)**

---

## 2. Laporan Praktikum — Soal 1 (Client-Server `wired.c` & `navi.c`)

📁 **Lokasi Kode:** [`./soal1/`](./soal1/) (`wired.c`, `navi.c`, `protocol.h`, `history.log`)

### 2.1 Tujuan & Gambaran Sistem

Membuat sistem komunikasi *client-server* menggunakan:
- **Socket** sebagai mekanisme IPC
- **Multiplexing I/O (`select`)** untuk menangani banyak *client* sekaligus
- **Fitur Admin** dengan mekanisme RPC sederhana
- **Logging Aktivitas** ke dalam `history.log`

Sistem terdiri dari:
- **Server (`wired.c`)** → pusat komunikasi yang menerima pesan, mengelola *user*, dan menyebarkan pesan ke *client* lain.
- **Client (`navi.c`)** → antarmuka pengguna untuk terhubung ke server.

### 2.2 Penjelasan Berdasarkan Poin Soal

1. **Server dapat menangani banyak client (`select()`):**  
   Fungsi `select()` memungkinkan server memantau banyak *socket descriptor* sekaligus dan mengetahui *client* mana yang mengirim data tanpa *blocking* satu per satu.
2. **Username harus unik:**  
   Saat *client* masuk, server memeriksa seluruh *slot* aktif:
   ```c
   if (strcmp(clients[i].name, name) == 0)
   ```
   Jika *username* sudah ada, koneksi ditolak dan *socket* ditutup.
3. **Broadcast pesan:**  
   Ketika *client* mengirim pesan, server meneruskannya ke semua *client* lain kecuali pengirim:
   ```c
   send(clients[j].socket, msg, strlen(msg), 0);
   ```
4. **Autentikasi Admin (`The Knights`):**  
   ```text
   Username: The Knights
   Password: 001122
   ```
   Jika autentikasi berhasil, *user* mendapatkan hak akses admin.
5. **RPC (*Remote Procedure Call*) Admin:**  
   Admin dapat memanggil fungsi di server melalui perintah:
   - `1` → Melihat jumlah *user* aktif
   - `2` → Melihat *uptime* server
   - `3` → *Shutdown* server
   - `4` → *Disconnect*
6. **Logging (`history.log`):**  
   Semua aktivitas dicatat ke dalam `history.log`:
   ```c
   FILE *f = fopen("history.log", "a");
   fprintf(f, "[%s] %s\n", type, msg);
   ```

### 2.3 Edge Case dan Penanganannya

- **Username duplikat:** Ditolak saat proses *handshake* awal (`if (exists)`).
- **Server penuh:** Jika `slot == -1` (mencapai `MAX_CLIENTS`), koneksi baru ditolak dengan pesan penuh.
- **Client disconnect tiba-tiba:** Ketika `valread == 0`, *socket* ditutup (`close(sd)`) dan `clients[i].socket = 0` dikosongkan.
- **Password admin salah:** Dicek dengan `strcmp(pass, ADMIN_PASS)`, akses ditolak jika tidak cocok.
- **Buffer overflow:** Membatasi ukuran *buffer* dan menggunakan `snprintf`.

### 2.4 Penjelasan Struktur & Komponen Kode

```c
struct Client {
    int socket;
    char name[50];
    int isAdmin;
};

struct Client clients[MAX_CLIENTS];
```

- `socket(AF_INET, SOCK_STREAM, 0)` → Membuat *endpoint* komunikasi TCP.
- `bind(server_fd, ...)` → Mengikat *socket* ke port `8080`.
- `listen(server_fd, 5)` → Menyiapkan *socket* menerima antrean koneksi.
- `accept(...)` → Menerima koneksi *client* baru.
- `select(...)` → Memantau seluruh *file descriptor* aktif.
- `read(sd, buffer, 1024)` & `send(...)` → Membaca dan mengirim pesan melalui *socket*.

### 2.5 Kode Utama (`wired.c`)

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

---

## 3. Laporan Praktikum — Soal 2 (Sistem Arena Client-Server Game)

📁 **Lokasi Kode:** [`./soal2/`](./soal2/) (`orion.c`, `eternal.c`, `makefile`, `users.txt`)

### 3.1 Struktur Program & Cara Menjalankan

Mensimulasikan game *arena battle* dengan implementasi **Shared Memory**, **Message Queue (IPC)**, dan **Multithreading**:
- `orion.c` → Server game
- `eternal.c` → Client pemain
- `makefile` → Kompilasi dan manajemen IPC

```bash
# Compile
make

# Menjalankan Server & Client
./orion
./eternal
```

### 3.2 Register dan Login System (Shared Memory)

Membuat sistem autentikasi pengguna yang aman serta mencegah duplikasi akun dan login ganda.

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

- **Register:** Memvalidasi apakah *username* sudah ada; jika belum, disimpan ke *shared memory*.
- **Login:** Memverifikasi kecocokan *password* dan memeriksa *flag* `online` serta *mutex* untuk mencegah login ganda dari dua *client*.

### 3.3 Matchmaking System (Message Queue)

Menghubungkan dua pemain untuk bertarung dalam satu arena menggunakan *Message Queue* (`MT_MATCH_REQ` dan `MT_MATCH_FOUND`):
- *Client* mengirim *request* matchmaking ke server.
- Server menyimpan pemain pertama dalam status *waiting*, lalu memasangkannya saat pemain kedua masuk.
- Jika melewati batas *timeout*, pemain diarahkan melawan *bot*.
- Menggunakan *flag* `in_match` dan penguncian *shared memory* agar sinkronisasi tetap konsisten.

### 3.4 Battle System Realtime (Multithreading)

Pertarungan antar-pemain berjalan secara *realtime* tanpa *blocking* menggunakan dua *thread*:
- **Thread utama:** Menangani input pemain (`a` untuk serangan biasa, `u` untuk *ultimate*).
- **Thread kedua:** Menerima *damage* dari lawan melalui *message queue*.

**Perhitungan Battle:**
- `HP = 100 + (XP / 10)`
- `Damage = 10 + (XP / 50) + Weapon`
- `Ultimate Damage = Damage * 3`

### 3.5 Armory System

Fitur pembelian senjata (*Wood Sword* dan *Iron Sword*) menggunakan *Gold*:
- Memvalidasi kecukupan *gold* agar tidak bernilai negatif.
- Memastikan senjata hanya diperbarui jika nilai *damage* senjata baru lebih tinggi:
  ```c
  if (me->weapon < 15) me->weapon = 15;
  ```
