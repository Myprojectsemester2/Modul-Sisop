#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include "protocol.h"

#define ADMIN_PASS "001122"

struct Client {
    int socket;
    char name[50];
    int isAdmin;
};

struct Client clients[MAX_CLIENTS];
time_t start_time;

// ================= LOGGING =================
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

// ================= HITUNG USER =================
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

    // init
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].socket = 0;
        clients[i].isAdmin = 0;
        strcpy(clients[i].name, "");
    }

    // create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Server running on port %d...\n", PORT);
    log_message("System", "SERVER ONLINE");

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

        // ================= NEW CONNECTION =================
        if (FD_ISSET(server_fd, &readfds)) {
            new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

            // cek kapasitas
            int slot = -1;
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].socket == 0) {
                    slot = i;
                    break;
                }
            }

            if (slot == -1) {
                char *msg = "[System] Server full (max 10 users)\n";
                send(new_socket, msg, strlen(msg), 0);
                close(new_socket);
                log_message("System", "Connection rejected (server full)");
                continue;
            }

            // baca username
            char name[50];
            valread = read(new_socket, name, 50);
            name[valread] = '\0';
            name[strcspn(name, "\n")] = 0;

            int isAdmin = 0;

            // ===== ADMIN LOGIN =====
            if (strcmp(name, "The Knights") == 0) {
                char *ask = "Enter Password: ";
                send(new_socket, ask, strlen(ask), 0);

                char pass[50];
                valread = read(new_socket, pass, 50);
                pass[valread] = '\0';
                pass[strcspn(pass, "\n")] = 0;

                if (strcmp(pass, ADMIN_PASS) == 0) {
                    isAdmin = 1;

                    char *menu =
                    "[System] Authentication Successful. Granted Admin privileges.\n"
                    "=== THE KNIGHTS CONSOLE ===\n"
                    "1. Check Active Entities (Users)\n"
                    "2. Check Server Uptime\n"
                    "3. Execute Emergency Shutdown\n"
                    "4. Disconnect\n";

                    send(new_socket, menu, strlen(menu), 0);
                    log_message("Admin", "LOGIN SUCCESS");
                } else {
                    send(new_socket, "Wrong password\n", 15, 0);
                    close(new_socket);
                    continue;
                }
            }

            // cek username unik
            int exists = 0;
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (strcmp(clients[i].name, name) == 0) {
                    exists = 1;
                    break;
                }
            }

            if (exists) {
                send(new_socket, "Username already taken\n", 24, 0);
                close(new_socket);
            } else {
                clients[slot].socket = new_socket;
                clients[slot].isAdmin = isAdmin;
                strcpy(clients[slot].name, name);

                printf("%s connected\n", name);

                char logbuf[100];
                sprintf(logbuf, "User '%s' connected", name);
                log_message("System", logbuf);
            }
        }

        // ================= HANDLE MESSAGE =================
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clients[i].socket;

            if (FD_ISSET(sd, &readfds)) {
                valread = read(sd, buffer, 1024);

                if (valread == 0) {
                    close(sd);
                    clients[i].socket = 0;
                    continue;
                }

                buffer[valread] = '\0';

                // exit
                if (strncmp(buffer, "/exit", 5) == 0) {
                    char logbuf[100];
                    sprintf(logbuf, "User '%s' disconnected", clients[i].name);
                    log_message("System", logbuf);

                    close(sd);
                    clients[i].socket = 0;
                    continue;
                }

                // ===== ADMIN COMMAND =====
                if (clients[i].isAdmin) {

                    if (strncmp(buffer, "1", 1) == 0 || strncmp(buffer, "/users", 6) == 0) {
                        int count = count_users();
                        char msg[100];
                        sprintf(msg, "Active Users: %d\n", count);
                        send(sd, msg, strlen(msg), 0);
                        log_message("Admin", "RPC_GET_USERS");
                        continue;
                    }

                    if (strncmp(buffer, "2", 1) == 0 || strncmp(buffer, "/uptime", 7) == 0) {
                        time_t now = time(NULL);
                        int sec = now - start_time;

                        char msg[100];
                        sprintf(msg, "Server Uptime: %d seconds\n", sec);
                        send(sd, msg, strlen(msg), 0);
                        log_message("Admin", "RPC_GET_UPTIME");
                        continue;
                    }

                    if (strncmp(buffer, "3", 1) == 0 || strncmp(buffer, "/shutdown", 9) == 0) {
                        log_message("Admin", "RPC_SHUTDOWN");
                        log_message("System", "EMERGENCY SHUTDOWN INITIATED");
                        printf("Server shutdown by admin\n");
                        exit(0);
                    }

                    if (strncmp(buffer, "4", 1) == 0) {
                        close(sd);
                        clients[i].socket = 0;
                        log_message("Admin", "DISCONNECT");
                        continue;
                    }
                }

                // ===== CHAT =====
                char msg[1200];
                sprintf(msg, "[%s]: %s", clients[i].name, buffer);

                printf("%s", msg);
                log_message("User", msg);

                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (clients[j].socket != 0 && clients[j].socket != sd) {
                        send(clients[j].socket, msg, strlen(msg), 0);
                    }
                }
            }
        }
    }

    return 0;
}