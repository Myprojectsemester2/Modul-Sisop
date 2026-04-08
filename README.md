SOAL 1 
---------------------------------------------------------
membantu uncle muthu untuk mendata data hutang pelanggannya karena datanya terkena virus, sekaligus membantu untuk memisahkan data pelanggan yang sudah belum lunas

Cara 
1. kita mendownload file/data terlebih dahulu
    ```c
    https://drive.google.com/uc?export=download&id=144hsFJGoAM5lbvBQjkTfZaD3GOil4yEw
    ```
2. membuat folder kasir_muthu untuk mengamankan data, dengan coddingan
 ```c
 #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void error_exit() {
    printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.\n");
    exit(EXIT_FAILURE);
}

int main() {
    pid_t pid;
    int status;

    // langsung membuat brankas_kedai [child proses]
    pid = fork();  //membuat child proses dari parent
    if (pid == 0) {
        execlp("mkdir", "mkdir", "brankas_kedai", NULL);
        exit(EXIT_FAILURE);
    } else {  // parent proses
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0) error_exit();
    }

    // membuat cp buku_hutang.csv ke brankas
    pid = fork();
    if (pid == 0) {
        execlp("cp", "cp", "buku_hutang.csv", "brankas_kedai/", NULL);
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0) error_exit();
    }

    // grep "belum lunaske daftar penunggak.txt"
    pid = fork();
    if (pid == 0) {
        execlp("bash", "bash", "-c",
               "grep 'Belum Lunas' brankas_kedai/buku_hutang.csv > brankas_kedai/daftar_penunggak.txt",
               NULL);
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0) error_exit();
    }

    // zip brankas jadi rahasia_muthu.zip
    pid = fork();
    if (pid == 0) {
        execlp("zip", "zip", "-r", "rahasia_muthu.zip", "brankas_kedai", NULL);
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0) error_exit();
    }

    printf("[INFO] Fuhh, selamat! Buku hutang dan daftar penagihan berhasil diamankan.\n");
    return 0;
}
 ```
penjelasan singkat 
*'fork()' → bikin child process (Ipin)
*'execlp()'→ jalankan command Linux
*'waitpid()' → parent (Upin) nunggu child selesai
*'WEXITSTATUS(status)' → cek berhasil

Output 
jIka berhasil
 ```c
[INFO] Fuhh, selamat! Buku hutang dan daftar penagihan berhasil diamankan.
 ```
jika gagal
 ```c
[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.
 ```

3. Run program
 ```c
gcc kasir_muthu.c -o kasir
./kasir
 ```
nanti akan muncul folder brankas_kedai, file daftar_penunggak.txt, file rahasia_muthu.zip

Setelah setiap child process selesai, program menggunakan waitpid() dan WEXITSTATUS untuk memastikan eksekusi berhasil. Jika gagal, program langsung dihentikan dengan exit(). Jika semua proses berhasil, program menampilkan pesan sukses.”
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

SOAL 2
