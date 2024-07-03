#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>

#define FS_SIGLEN 8
#define BUFF_SIZE 256
#define PROMPT "BratOS> "
#define FS_HEADER_ADDR 0x10000 /* Endereço do cabeçalho do sistema de arquivos na RAM */

// Definição da estrutura fs_header_t
struct fs_header_t {
    unsigned char signature[FS_SIGLEN];    /* The file system signature.              */
    unsigned short total_number_of_sectors; /* Number of 512-byte disk blocks.         */
    unsigned short number_of_boot_sectors;  /* Sectors reserved for boot code.         */
    unsigned short number_of_file_entries;  /* Maximum number of files in the disk.    */
    unsigned short max_file_size;           /* Maximum size of a file in blocks.       */
    unsigned int unused_space;              /* Remaining space less than max_file_size.*/
} __attribute__((packed));

// Definição da estrutura cmd_t
struct cmd_t {
    const char *name;
    void (*funct)(void);
};

// Funções declaradas para uso no kernel.c
void my_memcpy(void *dest, const void *src, size_t n);
void kmain(void);
void f_info(void);
void f_help(void);
void f_quit(void);
void f_ls(void);
void bios_load_sector(int sector, char *buffer);
void kwrite_itoa(unsigned short num);

#endif /* KERNEL_H */

