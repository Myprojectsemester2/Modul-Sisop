int cursor = 0;
char color = 0x07;

void putInMemory(int segment, int address, char character);
int getChar();

void printChar(char c) {
    if (c == '\n') {
        int next_row = 0;
        while (next_row <= cursor) {
            next_row += 160;
        }
        cursor = next_row;
    } else if (c == '\r') {
        int row_start = cursor;
        while (row_start >= 160) {
            row_start -= 160;
        }
        cursor = cursor - row_start;
    } else {
        putInMemory(0xB800, cursor, c);
        putInMemory(0xB800, cursor + 1, color);
        cursor += 2;
    }
}

void printString(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        printChar(str[i]);
        i++;
    }
}

void newline() {
    printChar('\n');
}

void clearScreen() {
    int i;
    for (i = 0; i < 4000; i += 2) {
        putInMemory(0xB800, i, ' ');
        putInMemory(0xB800, i + 1, color);
    }
    cursor = 0;
}

void readString(char* buffer) {
    int len = 0;
    while (1) {
        char c = getChar();
        if (c == '\r' || c == '\n') {
            buffer[len] = '\0';
            break;
        } else if (c == '\b') {
            if (len > 0) {
                len--;
                cursor -= 2;
                printChar(' ');
                cursor -= 2;
            }
        } else {
            if (len < 60) {
                buffer[len] = c;
                printChar(c);
                len++;
            }
        }
    }
}

int atoi(char* str) {
    int res = 0;
    int i = 0;
    while (str[i] == ' ') i++;
    while (str[i] >= '0' && str[i] <= '9') {
        res = res * 10 + (str[i] - '0');
        i++;
    }
    return res;
}

/* Optimasi: Cetak angka langsung ke layar tanpa penampung array string ganda */
void printInt(int num) {
    char temp[7];
    int i = 0;
    if (num == 0) {
        printChar('0');
        return;
    }
    if (num < 0) {
        printChar('-');
        num = -num;
    }
    while (num > 0) {
        int next_num = 0;
        int rem = num;
        while (rem >= 10) {
            rem -= 10;
            next_num++;
        }
        temp[i] = rem + '0';
        i++;
        num = next_num;
    }
    while (i > 0) {
        i--;
        printChar(temp[i]);
    }
}

void main() {
    char cmd[64];
    clearScreen();

    printString("Welcome to Mini OS Shell");
    newline();
    printString("type 'help' to see commands");
    newline();
    newline();

    while (1) {
        printString("> ");
        readString(cmd);
        newline();

        /* Optimasi Ukuran: Pencocokan string langsung tanpa fungsi eksternal */
        if (cmd[0] == 'h' && cmd[1] == 'e' && cmd[2] == 'l' && cmd[3] == 'p' && cmd[4] == '\0') {
            printString("check add sub fac season triangle clear about");
        }
        else if (cmd[0] == 'c' && cmd[1] == 'h' && cmd[2] == 'e' && cmd[3] == 'c' && cmd[4] == 'k' && cmd[5] == '\0') {
            printString("System is running optimally.");
        }
        else if (cmd[0] == 'c' && cmd[1] == 'l' && cmd[2] == 'e' && cmd[3] == 'a' && cmd[4] == 'r' && cmd[5] == '\0') {
            clearScreen();
            continue;
        }
        else if (cmd[0] == 'a' && cmd[1] == 'b' && cmd[2] == 'o' && cmd[3] == 'u' && cmd[4] == 't' && cmd[5] == '\0') {
            printString("Mini x86 16-bit OS Shell Project.");
        }
        else if (cmd[0] == 'a' && cmd[1] == 'd' && cmd[2] == 'd' && cmd[3] == ' ') {
            char* p = cmd + 4;
            int a = atoi(p);
            int b;
            while (*p != ' ' && *p != '\0') p++;
            while (*p == ' ') p++;
            b = atoi(p);
            printInt(a + b);
        }
        else if (cmd[0] == 's' && cmd[1] == 'u' && cmd[2] == 'b' && cmd[3] == ' ') {
            char* p = cmd + 4;
            int a = atoi(p);
            int b;
            while (*p != ' ' && *p != '\0') p++;
            while (*p == ' ') p++;
            b = atoi(p);
            printInt(a - b);
        }
        else if (cmd[0] == 'f' && cmd[1] == 'a' && cmd[2] == 'c' && cmd[3] == ' ') {
            int n = atoi(cmd + 4);
            if (n >= 8 || n < 0) {
                printString("know your limit little bro.");
            } else {
                int fact = 1;
                int i;
                for (i = 1; i <= n; i++) {
                    fact *= i;
                }
                printInt(fact);
            }
        }
        else if (cmd[0] == 's' && cmd[1] == 'e' && cmd[2] == 'a' && cmd[3] == 's' && cmd[4] == 'o' && cmd[5] == 'n' && cmd[6] == ' ') {
            char* m = cmd + 7;
            if (m[0] == 'w' && m[1] == 'i' && m[2] == 'n' && m[3] == 't' && m[4] == 'e' && m[5] == 'r' && m[6] == '\0') {
                color = 0x01; printString("winter mode");
            } else if (m[0] == 's' && m[1] == 'p' && m[2] == 'r' && m[3] == 'i' && m[4] == 'n' && m[5] == 'g' && m[6] == '\0') {
                color = 0x0A; printString("spring mode");
            } else if (m[0] == 's' && m[1] == 'u' && m[2] == 'm' && m[3] == 'm' && m[4] == 'e' && m[5] == 'r' && m[6] == '\0') {
                color = 0x02; printString("summer mode");
            } else if (m[0] == 'f' && m[1] == 'a' && m[2] == 'l' && m[3] == 'l' && m[4] == '\0') {
                color = 0x06; printString("fall mode");
            } else if (m[0] == 'r' && m[1] == 'a' && m[2] == 'd' && m[3] == 'i' && m[4] == 'a' && m[5] == 'n' && m[6] == 't' && m[7] == '\0') {
                color = 0x0D; printString("radiant mode");
            } else {
                printString("Unknown season.");
            }
        }
        else if (cmd[0] == 't' && cmd[1] == 'r' && cmd[2] == 'i' && cmd[3] == 'a' && cmd[4] == 'n' && cmd[5] == 'g' && cmd[6] == 'l' && cmd[7] == 'e' && cmd[8] == ' ') {
            int n = atoi(cmd + 9);
            int i, j;
            for (i = 1; i <= n; i++) {
                for (j = 1; j <= i; j++) {
                    printChar('x');
                }
                if (i < n) newline();
            }
        }
        else {
            printString("Command tidak dikenali. Ketik 'help'.");
        }
        newline();
    }
}