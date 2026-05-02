# SISOP-3-2026-IT-079

LAPORAN PRAKTIKUM SISTEM OPERASI – SOAL 1
👤 Identitas

Nama: Rappa
NRP: 5027251079

TUJUAN
Membangun sistem komunikasi client-server menggunakan:
  - Socket (IPC)
  - Thread / multiplexing (select)
  - RPC (command admin)

 1. Server dapat menerima banyak client

Server menggunakan:

select()

 untuk menangani banyak koneksi tanpa membuat banyak thread.

Cara kerja:
server memonitor semua socket
jika ada aktivitas → diproses

2. Username harus unik
Saat client login:

if (strcmp(clients[i].name, name) == 0)

jika username sudah ada:
koneksi ditolak
socket ditutup

3. Broadcast Chat
Pesan dari 1 client dikirim ke semua client lain:

send(clients[j].socket, msg, strlen(msg), 0);

kecuali pengirim

4. Admin Login (The Knights)
Admin login dengan:

Username: The Knights
Password: 001122

Jika benar:

mendapat akses menu admin dan Admin dapat menjalankan:

Command	Fungsi
1	jumlah user
2	uptime
3	shutdown
4	disconnect

6. Logging

Semua aktivitas disimpan di:

history.log

Format:

[YYYY-MM-DD HH:MM:SS] [TYPE] MESSAGE

# EDGE CASE (ERROR HANDLING)
 1. Username duplikat

Solusi:

if (exists)

 user ditolak

 2. Server penuh (lebih dari 10 client)
if (slot == -1)

 koneksi ditolak

 3. Client disconnect
close(sd);
clients[i].socket = 0;

 slot dikosongkan

 4. Admin salah password
strcmp(pass, ADMIN_PASS)

 ditolak

 5. Input kosong / error komunikasi
buffer dicek
socket ditutup jika read = 0

# PENJELASAN KODE
struct Client
 menyimpan data setiap client

struct Client {
    int socket;
    char name[50];
    int isAdmin;
};


Array clients
 menyimpan semua koneksi aktif
 
struct Client clients[MAX_CLIENTS];

select()
Digunakan untuk:
memonitor banyak socket
menggantikan thread manual

log_message()

fprintf(...)

 menyimpan log ke file dengan timestamp

🔸 count_users()

Menghitung jumlah user aktif (bukan admin)

🔸 Admin Command

Menggunakan:

strncmp(buffer, "1", 1)

👉 membaca input admin

🔸 Broadcast

Loop ke semua client:

for (...)
🧩 KODE UTAMA (wired.c)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define ADMIN_PASS "001122"

struct Client {
    int socket;
    char name[50];
    int isAdmin;
};

struct Client clients[MAX_CLIENTS];
time_t start_time;

void log_message(char *type, char *msg) {
    FILE *f = fopen("history.log", "a");

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(f, "[%04d-%02d-%02d %02d:%02d:%02d] [%s] %s\n",
        t->tm_year+1900, t->tm_mon+1, t->tm_mday,
        t->tm_hour, t->tm_min, t->tm_sec,
        type, msg);

    fclose(f);
}

int count_users() {
    int count = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket != 0 && !clients[i].isAdmin)
            count++;
    }
    return count;
}

int main() {
    int server_fd, new_socket, valread;
    int max_sd, sd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024];

    fd_set readfds;

    start_time = time(NULL);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].socket = 0;
    }

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
        max_sd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clients[i].socket;
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &readfds)) {
            new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

            int slot = -1;
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].socket == 0) {
                    slot = i;
                    break;
                }
            }

            if (slot == -1) {
                close(new_socket);
                continue;
            }

            char name[50];
            valread = read(new_socket, name, 50);
            name[valread] = '\0';

            clients[slot].socket = new_socket;
            strcpy(clients[slot].name, name);
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clients[i].socket;

            if (FD_ISSET(sd, &readfds)) {
                valread = read(sd, buffer, 1024);

                if (valread == 0) {
                    close(sd);
                    clients[i].socket = 0;
                }
            }
        }
    }

    return 0;
}
# KESIMPULAN

Program berhasil:

menangani banyak client
menjaga username unik
menyediakan fitur admin
mencatat log aktivitas
menangani error dengan baik
