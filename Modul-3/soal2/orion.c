#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#define SHM_KEY 120
#define MSG_KEY 1200

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

// ================= REGISTER =================
int register_user(char *data) {
    FILE *f = fopen("users.txt", "a+");
    char line[100];
    char username[50], password[50];

    if (f == NULL) return 0;

    sscanf(data, "%[^:]:%s", username, password);

    rewind(f);
    while (fgets(line, sizeof(line), f)) {
        char existing_user[50], existing_pass[50];

        line[strcspn(line, "\n")] = 0;
        sscanf(line, "%[^:]:%s", existing_user, existing_pass);

        if (strcmp(existing_user, username) == 0) {
            fclose(f);
            return -1;
        }
    }

    fprintf(f, "%s\n", data);
    fclose(f);
    return 1;
}

// ================= LOGIN =================
int login_user(char *data) {
    FILE *f = fopen("users.txt", "r");
    char line[100];

    if (f == NULL) return 0;

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, data) == 0) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

int main() {
    int shmid, msgid;
    char *shared_mem;

    shmid = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT);
    shared_mem = (char*) shmat(shmid, NULL, 0);

    msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);

    printf("Orion is ready...\n");

    while (1) {
        struct msg_buffer message;

        msgrcv(msgid, &message, sizeof(message), 1, 0);

        printf("Request: %s\n", message.msg_text);

        // REGISTER
        if (strncmp(message.msg_text, "REGISTER:", 9) == 0) {
            int result = register_user(message.msg_text + 9);

            if (result == 1)
                sprintf(shared_mem, "Register Success");
            else if (result == -1)
                sprintf(shared_mem, "Username already exists!");
            else
                sprintf(shared_mem, "Register Failed");
        }

        // LOGIN
        else if (strncmp(message.msg_text, "LOGIN:", 6) == 0) {
            if (login_user(message.msg_text + 6))
                sprintf(shared_mem, "Login Success");
            else
                sprintf(shared_mem, "Login Failed");
        }

        else {
            sprintf(shared_mem, "Unknown Command");
        }
    }

    return 0;
}
