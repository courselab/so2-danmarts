/* kernel.h */

#ifndef KERNEL_H
#define KERNEL_H

#include "bios1.h"      /* For kwrite() etc.            */
#include "bios2.h"      /* For kread() etc.             */

/* Define FS signature length */
#define FS_SIGLEN 8

/* Define buffer size for shell */
#define BUFF_SIZE 256

/* Prompt for shell */
#define PROMPT "> "

/* Structure for FS header */
struct fs_header_t {
    unsigned char  signature[FS_SIGLEN];    /* The file system signature.              */
    unsigned short total_number_of_sectors; /* Number of 512-byte disk blocks.         */
    unsigned short number_of_boot_sectors;  /* Sectors reserved for boot code.         */
    unsigned short number_of_file_entries;  /* Maximum number of files in the disk.    */
    unsigned short max_file_size;           /* Maximum size of a file in blocks.       */
    unsigned int unused_space;              /* Remaining space less than max_file_size.*/
} __attribute__((packed));

typedef struct {
    char filename[32];
    int start_sector;
    int file_size;
} directory_entry_t;


/* Array with built-in command names and respective function pointers */
struct cmd_t {
    const char *name;
    void (*funct)(void);
};

/* Kernel function prototypes */
void __attribute__((fastcall)) kwriteln(const char*);
void itoa(int num, char *str, int base);
void kmain(void);
void shell(void);
void __attribute__((fastcall)) clear (void);
void list_files(void);
void f_help(void);
void f_quit(void);
void f_info(void);



#endif /* KERNEL_H */

