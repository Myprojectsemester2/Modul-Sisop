#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <sys/prctl.h>

void handle_sigterm(int sig) {
    exit(0);
}

int main(int argc, char *argv[]) {
    pid_t pid;

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    if (setsid() < 0) exit(EXIT_FAILURE);

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    signal(SIGTERM, handle_sigterm);

    // Nama internal
    prctl(PR_SET_NAME, "maya", 0, 0, 0);

    // 🔥 FIX UTAMA (bersihin + isi ulang)
    memset(argv[0], 0, strlen(argv[0]));
    strcpy(argv[0], "maya");

    while (1) {
        sleep(10);
    }

    return 0;
}
