#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define SHM_KEY 120
#define MSG_KEY 1200
#define MAX_INPUT 30

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

// ===== INPUT AMAN =====
void safe_input(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    if (strlen(buffer) >= size - 1) {
        printf("Input too long! Max %d chars\n", size - 1);
        buffer[0] = '\0';
    }
}

int main() {
    int shmid, msgid;
    char *shared_mem;

    shmid = shmget(SHM_KEY, 1024, 0666);
    shared_mem = (char*) shmat(shmid, NULL, 0);

    msgid = msgget(MSG_KEY, 0666);

    struct msg_buffer message;
    message.msg_type = 1;

    while (1) {
        printf("\n=== SYSTEM MENU ===\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Choose: ");

        int choice;
        scanf("%d", &choice);
        getchar();

        char username[MAX_INPUT];
        char password[MAX_INPUT];

        if (choice == 1) {
            printf("Username: ");
            safe_input(username, MAX_INPUT);
            if (strlen(username) == 0) continue;

            printf("Password: ");
            safe_input(password, MAX_INPUT);
            if (strlen(password) == 0) continue;

            snprintf(message.msg_text, sizeof(message.msg_text),
                     "REGISTER:%s:%s", username, password);
        }

        else if (choice == 2) {
            printf("Username: ");
            safe_input(username, MAX_INPUT);
            if (strlen(username) == 0) continue;

            printf("Password: ");
            safe_input(password, MAX_INPUT);
            if (strlen(password) == 0) continue;

            snprintf(message.msg_text, sizeof(message.msg_text),
                     "LOGIN:%s:%s", username, password);
        }

        else if (choice == 3) {
            printf("Exit...\n");
            break;
        }

        else {
            printf("Invalid choice!\n");
            continue;
        }

        msgsnd(msgid, &message, sizeof(message), 0);

        sleep(1);

        printf("Response: %s\n", shared_mem);
    }

    return 0;
}
