/*
 *    SPDX-FileCopyrightText: 2024 danmartsar <daniel.martins@lonvi.com.br>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative work from SYSeg (https://gitlab.com/monaco/syseg)
 *  and contains modifications carried out by the following author(s):
 *  danmartsar <daniel.martins@lonvi.com.br>
 */

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
    kwrite_fixed_length_string(fs_header.signature, SIGNATURE_SIZE);
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
    char buffer[6]; // Alocamos espaço para até 5 dígitos + 1 para o terminador '\0'
    int i = 5; // Começamos do último índice do buffer
    
    buffer[5] = '\0'; // Terminador de string

    // Caso especial: se num for 0, apenas escrevemos "0"
    if (num == 0)
    {
        buffer[--i] = '0';
    }
    else
    {
        // Convertendo cada dígito do número para caracteres ASCII
        while (num != 0 && i >= 0)
        {
            buffer[--i] = '0' + (num % 10);
            num /= 10;
        }
    }

    // Agora escrevemos a string no buffer usando kwrite
    kwrite(&buffer[i]);
}



// Implementação simples de putchar para um ambiente simulado
void putchar(char c) {
    // Aqui, você deve substituir com a lógica real de saída
    // Por exemplo, enviar para um dispositivo de saída ou buffer de tela
    // Esta é uma implementação simples apenas para demonstração

    // Suponha que você tenha uma variável global ou um endereço de memória que simule o buffer de vídeo
    // Neste exemplo, vamos simular um buffer de vídeo básico usando uma matriz de caracteres
    // Substitua isso com sua lógica real de saída para o emulador ou simulador

    // Exemplo de simulação básica de um buffer de vídeo
    static char video_buffer[25][80]; // Tamanho simulado do buffer de vídeo (25 linhas x 80 colunas)
    static int cursor_x = 0;
    static int cursor_y = 0;

    if (c == '\n') {
        // Nova linha: avance para a próxima linha
        cursor_x = 0;
        cursor_y++;
    } else {
        // Escreva o caractere no buffer de vídeo simulado
        video_buffer[cursor_y][cursor_x] = c;
        cursor_x++;

        // Se atingir o final da linha, vá para a próxima linha
        if (cursor_x >= 80) {
            cursor_x = 0;
            cursor_y++;
        }

        // Se atingir o final do buffer de vídeo, role para cima (simulação simples)
        if (cursor_y >= 25) {
            // Implemente sua lógica de rolagem aqui
            // Por exemplo, mover todas as linhas para cima e limpar a última linha
            // Esta é uma simulação muito básica apenas para fins de demonstração
        }
    }
}

// Implementação de kwrite_char usando putchar
void __attribute__((fastcall)) kwrite_char(char c) {
    putchar(c);
}

// Função para imprimir uma string de comprimento fixo usando kwrite_char
void kwrite_fixed_length_string(const char *str, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        // Converta o caractere para um byte ASCII imprimível
        char printable_char = str[i];
        if (printable_char < 32 || printable_char > 126) {
            printable_char = '.';  // Caracteres não imprimíveis são substituídos por '.'
        }
        kwrite_char(printable_char);
    }
}



void kmain(void)
{
  register_syscall_handler();
  splash();
  shell();
  halt();
}

