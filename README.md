<div align="center">

# Modul Praktikum Sistem Operasi

**Portofolio Praktikum & Materi Sistem Operasi — Departemen Teknologi Informasi ITS**

**Nama:** Raffa Al Azmi &nbsp;|&nbsp; **NRP:** 5027251079 &nbsp;|&nbsp; **GitHub:** [@raffaalazmi-png](https://github.com/raffaalazmi-png)

</div>

---

## Daftar Modul

1. **[Modul 1: Shell Scripting, Cron, & AWK](./Modul-1/README.md)** — [📖 Materi Modul 1](./Modul-1/MATERI.md)
2. **[Modul 2: *Process* & Daemon](./Modul-2/README.md)** — [📖 Materi Modul 2](./Modul-2/MATERI.md)
3. **[Modul 3: Thread, IPC, & RPC](./Modul-3/README.md)** — [📖 Materi Modul 3](./Modul-3/MATERI.md)
4. **[Modul 4: Filesystem & Docker](./Modul-4/README.md)** — [📖 Materi Modul 4](./Modul-4/MATERI.md)
5. **[Modul 5: Operating System & Bootloaders](./Modul-5/README.md)** — [📖 Materi Modul 5](./Modul-5/MATERI.md)

---

## Ringkasan Pengerjaan Praktikum

| Modul | Topik Utama | Studi Kasus / Implementasi | Laporan & Kode | Materi Referensi |
| :--- | :--- | :--- | :---: | :---: |
| **Modul 1** | Shell Scripting, Cron, & AWK | Analisis Data Penumpang Kereta (`KANJ.sh`) & Ekspedisi Koordinat (`parserkoordinat.sh`, `nemupusaka.sh`) | [Buka Modul 1](./Modul-1/README.md) | [Materi 1](./Modul-1/MATERI.md) |
| **Modul 2** | *Process* & Daemon | Pengamanan Data Kedai (`kasir_muthu.c`), Monitoring Integritas File (`contract_daemon.c`), & Encode/Decode Daemon (`angel.c`) | [Buka Modul 2](./Modul-2/README.md) | [Materi 2](./Modul-2/MATERI.md) |
| **Modul 3** | Thread, IPC, & RPC | Multi-Client Chat & Admin RPC Server (`wired.c`, `navi.c`) serta Realtime Arena Battle IPC & Thread (`orion.c`, `eternal.c`) | [Buka Modul 3](./Modul-3/README.md) | [Materi 3](./Modul-3/MATERI.md) |
| **Modul 4** | Filesystem (FUSE) & Docker | Virtual Filesystem Passthrough (`kenz_rescue.c`) & Encrypted Storage FUSE + Dockerized Mini Database TCP (`fuse.c`, `client.c`) | [Buka Modul 4](./Modul-4/README.md) | [Materi 4](./Modul-4/MATERI.md) |
| **Modul 5** | Operating System & Bootloaders | Implementasi Kernel 16-bit x86 Sederhana & BIOS Interrupt di Bochs (`bootloader.asm`, `kernel.asm`, `kernel.c`) | [Buka Modul 5](./Modul-5/README.md) | [Materi 5](./Modul-5/MATERI.md) |

---

## Struktur Repositori

```text
Modul-Sisop/
├── Modul-1/          # Shell Scripting, Cron, dan AWK (SISOP-1-2026-IT-079)
│   ├── assets/       # Gambar & ilustrasi materi Modul 1
│   ├── soal_1/       # Script analisis CSV dengan AWK (KANJ.sh)
│   ├── soal_2/       # Script ekstraksi & kalkulasi koordinat JSON
│   ├── MATERI.md     # Materi lengkap Modul 1
│   └── README.md     # Laporan resmi & dokumentasi praktikum Modul 1
├── Modul-2/          # Process & Daemon (SISOP-2-2026-IT-079)
│   ├── assets/       # Gambar & ilustrasi materi Modul 2
│   ├── code/         # Contoh kode referensi Process & Daemon
│   ├── soal_1/       # Fork, exec, & waitpid (kasir_muthu.c)
│   ├── soal_2/       # File integrity daemon (contract_daemon.c)
│   ├── soal_3/       # Background daemon & Base64 decryption (angel.c)
│   ├── MATERI.md     # Materi lengkap Modul 2
│   └── README.md     # Laporan resmi & dokumentasi praktikum Modul 2
├── Modul-3/          # Thread, IPC, & RPC (SISOP-3-2026-IT-079)
│   ├── assets/       # Gambar & ilustrasi materi Modul 3
│   ├── soal1/        # Socket IPC & select() multi-client (wired.c, navi.c)
│   ├── soal2/        # Shared memory, message queue & thread game (orion.c, eternal.c)
│   ├── MATERI.md     # Materi lengkap Modul 3
│   └── README.md     # Laporan resmi & dokumentasi praktikum Modul 3
├── Modul-4/          # Filesystem & Docker (SISOP-4-2026-IT-079)
│   ├── img/          # Gambar & ilustrasi materi Modul 4
│   ├── soal1/        # FUSE mirror & virtual file (kenz_rescue.c)
│   ├── fuse.c        # Encrypted FUSE translator filesystem
│   ├── client.c      # TCP database client (port 9000)
│   ├── MATERI.md     # Materi lengkap Modul 4
│   └── README.md     # Laporan resmi & dokumentasi praktikum Modul 4
├── Modul-5/          # Operating System & Bootloaders (SISOP-5-2026-IT-079)
│   ├── img/          # Gambar & ilustrasi materi Modul 5
│   ├── playground/   # Contoh referensi bootloader & kernel
│   ├── soal_2/       # Kernel x86 16-bit & bootloader (kernel.asm, kernel.c)
│   ├── MATERI.md     # Materi lengkap Modul 5
│   └── README.md     # Laporan resmi & dokumentasi praktikum Modul 5
└── README.md
```
