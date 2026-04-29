#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

#define PORT 8080

int sock;

void *receive_msg(void *arg) {
    char buffer[1024];

    while (1) {
        int valread = read(sock, buffer, 1024);
        if (valread > 0) {
            buffer[valread] = '\0';
            printf("%s", buffer);
        }
    }
}

void *send_msg(void *arg) {
    char message[1024];

    while (1) {
        fgets(message, 1024, stdin);
        send(sock, message, strlen(message), 0);
    }
}

int main() {
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Connected to server!\n");

    // input username
    char name[50];
    printf("Enter username: ");
    fgets(name, 50, stdin);
    send(sock, name, strlen(name), 0);

    pthread_t send_thread, recv_thread;

    pthread_create(&recv_thread, NULL, receive_msg, NULL);
    pthread_create(&send_thread, NULL, send_msg, NULL);

    pthread_join(send_thread, NULL);
    pthread_join(recv_thread, NULL);

    return 0;
}
