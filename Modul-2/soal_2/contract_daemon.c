#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <time.h>

FILE *log_file;

// fungsi tulis log
void write_log(const char *msg) {
    log_file = fopen("work.log", "a");
    if (log_file != NULL) {
        fprintf(log_file, "%s\n", msg);
        fflush(log_file); // penting biar langsung ke file
        fclose(log_file);
    }
}

// signal handler
void handle_sigterm(int sig) {
    write_log("We really weren't meant to be together");
    exit(0);
}

// cek apakah file ada
int file_exists(const char *filename) {
    return access(filename, F_OK) == 0;
}

// restore file
void restore_file() {
    FILE *f = fopen("contract.txt", "w");
    if (f != NULL) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        fprintf(f, "A promise to keep going, even when unseen.\n");
        fprintf(f, "restored at: %04d-%02d-%02d %02d:%02d:%02d\n",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec);

        fclose(f);
    }
}

int main() {
    pid_t pid;

    // fork pertama
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    // buat session baru
    if (setsid() < 0) exit(EXIT_FAILURE);

    // fork kedua (optional biar lebih proper daemon)
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    // set permission
    umask(0);

    // pindah working directory
    chdir(".");

    // tutup standard file descriptor
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    // set signal handler
    signal(SIGTERM, handle_sigterm);

    char buffer[256];

    while (1) {
        // log kerja
        write_log("still working...");

        // random state
        int state = rand() % 3;
        if (state == 0) write_log("[awake]");
        else if (state == 1) write_log("[drifting]");
        else write_log("[numbness]");

        // cek file
        if (!file_exists("contract.txt")) {
            write_log("contract violated.");
            restore_file();
        } else {
            // cek isi file (validasi sederhana)
            FILE *f = fopen("contract.txt", "r");
            if (f != NULL) {
                fgets(buffer, sizeof(buffer), f);
                fclose(f);

                if (strncmp(buffer, "A promise", 9) != 0) {
                    write_log("contract violated.");
                }
            }
        }

        sleep(1); // penting biar tidak spam CPU & log
    }

    return 0;
}
