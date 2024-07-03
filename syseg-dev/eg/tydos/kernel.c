#include "bios1.h"
#include "bios2.h"
#include "kernel.h"
#include "kaux.h"

char buffer[BUFF_SIZE];
int go_on = 1;

struct cmd_t cmds[] = {
    {"help", f_help},
    {"quit", f_quit},
    {"info", f_info},
    {"ls", f_ls},
    {NULL, NULL}
};

void shell()
{
    int i;
    clear();
    kwrite("BratOS 1.0\n");

    while (go_on)
    {
        do
        {
            kwrite(PROMPT);
            kread(buffer);
        } while (!buffer[0]);

        i = 0;
        while (cmds[i].name != NULL)
        {
            if (!strcmp(buffer, cmds[i].name))
            {
                cmds[i].funct();
                break;
            }
            i++;
        }

        if (cmds[i].name == NULL)
            kwrite("Command not found\n");
    }
}

void f_info()
{
    struct fs_header_t fs_header;

    bios_load_sector(FS_HEADER_ADDR / 512, (char *) &fs_header);

    kwrite("File System Information:\n");
    kwrite("  Signature: ");
    kwrite(fs_header.signature);
    kwrite("\n  Total Number of Sectors: ");
    kwrite_itoa(fs_header.total_number_of_sectors);
    kwrite("\n  Number of Boot Sectors: ");
    kwrite_itoa(fs_header.number_of_boot_sectors);
    kwrite("\n  Number of File Entries: ");
    kwrite_itoa(fs_header.number_of_file_entries);
    kwrite("\n  Maximum File Size: ");
    kwrite_itoa(fs_header.max_file_size);
    kwrite("\n  Unused Space: ");
    kwrite_itoa(fs_header.unused_space);
    kwrite("\n");
}

void f_help()
{
    kwrite("...me, Charli xcx, you're my only hope!\n\n");
    kwrite("   But we can try also some commands:\n");
    kwrite("      info    (to display file system information)\n");
    kwrite("      ls      (to list the directory contents)\n");
    kwrite("      quit    (to exit BratOS)\n");
}

void f_quit()
{
    kwrite("Program halted. Bye.");
    go_on = 0;
}

void f_ls()
{
    kwrite("Listing files:\n");
}

void bios_load_sector(int sector, char *buffer)
{
    unsigned char *ram_ptr = (unsigned char *) FS_HEADER_ADDR;
    my_memcpy(buffer, ram_ptr + (sector * 512), sizeof(struct fs_header_t));
}

void my_memcpy(void *dest, const void *src, size_t n)
{
    char *d = dest;
    const char *s = src;
    while (n--)
        *d++ = *s++;
}

void kwrite_itoa(unsigned short num)
{
    char buffer[6];
    int i = 4;
    buffer[5] = '\0';
    if (num == 0)
    {
        buffer[4] = '0';
        i--;
    }
    else
    {
        while (num > 0 && i >= 0)
        {
            buffer[i] = '0' + (num % 10);
            num /= 10;
            i--;
        }
    }
    kwrite(&buffer[i + 1]);
}

void kmain(void)
{
  register_syscall_handler();
  splash();
  shell();
  halt();
}

