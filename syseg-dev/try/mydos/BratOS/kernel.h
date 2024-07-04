/*
 *    SPDX-FileCopyrightText: 2024 danmartsar <daniel.martins@lonvi.com.br>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative work from SYSeg (https://gitlab.com/monaco/syseg)
 *  and contains modifications carried out by the following author(s):
 *  danmartsar <daniel.martins@lonvi.com.br>
 */

#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>

// Buffer size
#define BUFF_SIZE 256

// Prompt for the shell
#define PROMPT "BratOS> "

// File System Header Address
#define FS_HEADER_ADDR 0x7E00

// Signature size in the file system header
#define SIGNATURE_SIZE 8

// Struct for command
struct cmd_t {
    const char *name;
    void (*funct)();
};

// Struct for the file system header
struct fs_header_t {
    char signature[SIGNATURE_SIZE];
    unsigned short total_number_of_sectors;
    unsigned short number_of_boot_sectors;
    unsigned short number_of_file_entries;
    unsigned short max_file_size;
    unsigned short unused_space;
};

// Function prototypes
void __attribute__((fastcall)) clear(void);
void __attribute__((fastcall)) kwrite(const char *str);
void __attribute__((fastcall)) kwrite_char(char c);
int __attribute__((fastcall)) kread(char *buffer);  // Ajustado para alinhar com bios2.h
void register_syscall_handler();
void splash();
void halt();
void bios_load_sector(int sector, char *buffer);
void my_memcpy(void *dest, const void *src, size_t n);
void kwrite_itoa(unsigned short num);
void kwrite_fixed_length_string(const char *str, size_t length);
void f_help();
void f_quit();
void f_info();
void f_ls();
void shell();
void kmain(void);




#endif // KERNEL_H

