#include "kernel.h"     /* Funções essenciais do kernel. */
#include "kaux.h"       /* Funções auxiliares do kernel. */


/* Endereços onde o cabeçalho do sistema de arquivos e o buffer de diretório serão carregados */
#define FS_HEADER_ADDRESS   0x8000  // Endereço de exemplo para o cabeçalho do sistema de arquivos
#define DIR_BUFFER_ADDRESS  0x9000  // Endereço de exemplo para o buffer de diretório

/* Variáveis globais */
char buffer[BUFF_SIZE];
int go_on = 1;

/* Comandos embutidos */
static struct cmd_t cmds[] = {
    {"help", f_help},
    {"quit", f_quit},
    {"info", f_info},
    {"ls", list_files},
    {0, 0}
};

/* Função principal do kernel */
void kmain(void) {
    register_syscall_handler(); /* Registra o manipulador de syscall no int 0x21 */
    splash();       /* Tela de splash desnecessária */
    shell();        /* Inicia o interpretador de linha de comando */
    halt();         /* Na saída, halt */
}

/* Interpretador de linha de comando */
void shell(void) {
    int i;
    clear();
    kwrite("BratOS 1.0\n");

    while (go_on) {
        /* Lê a entrada do usuário. Comandos são tokens ASCII de palavra única sem espaços. */
        do {
            kwrite(PROMPT);
            kread(buffer);
        } while (!buffer[0]);

        /* Verifica comandos embutidos correspondentes */
        i = 0;
        while (cmds[i].funct) {
            if (!strcmp(buffer, cmds[i].name)) {
                cmds[i].funct();
                break;
            }
            i++;
        }

        if (!cmds[i].funct) {
            kwrite("Command not found\n");
        }
    }
}

/* Comando embutido: help */
void f_help(void) {
    kwrite("...me, Charli, you're my only hope!\n\n");
    kwrite("   But we can try also some commands:\n");
    kwrite("      quit    (to exit BratOS)\n");
    kwrite("      info    (to display file system information)\n");
    kwrite("      ls      (to list files)\n");
}

/* Comando embutido: quit */
void f_quit(void) {
    kwrite("About to Crash. Bye.");
    go_on = 0;
}

/* Função para exibir informações do sistema de arquivos */
void f_info(void) {
    // Carrega o cabeçalho do sistema de arquivos na memória
    struct fs_header_t *fs_header = (struct fs_header_t *)FS_HEADER_ADDRESS;

    // Imprime algumas informações do cabeçalho do sistema de arquivos (exemplo)
    kwrite("File System Information:\n");
    kwrite("Signature: ");
    for (int i = 0; i < FS_SIGLEN; i++) {
        char sig_char[2] = { fs_header->signature[i], '\0' };
        kwrite(sig_char);
    }
    kwrite("\n");

    // Imprime os demais campos do cabeçalho
    kwrite("Total number of sectors: ");
    char total_sectors_str[16];
    itoa(fs_header->total_number_of_sectors, total_sectors_str, 10);
    kwrite(total_sectors_str);
    kwrite("\n");

    kwrite("Number of boot sectors: ");
    char boot_sectors_str[16];
    itoa(fs_header->number_of_boot_sectors, boot_sectors_str, 10);
    kwrite(boot_sectors_str);
    kwrite("\n");

    kwrite("Number of file entries: ");
    char file_entries_str[16];
    itoa(fs_header->number_of_file_entries, file_entries_str, 10);
    kwrite(file_entries_str);
    kwrite("\n");

    kwrite("Max file size: ");
    char max_file_size_str[16];
    itoa(fs_header->max_file_size, max_file_size_str, 10);
    kwrite(max_file_size_str);
    kwrite("\n");

    kwrite("Unused space: ");
    char unused_space_str[16];
    itoa(fs_header->unused_space, unused_space_str, 10);
    kwrite(unused_space_str);
    kwrite("\n");
}

/* Função para listar arquivos */
void list_files(void) {
    struct fs_header_t *fs_header = (struct fs_header_t *)FS_HEADER_ADDRESS;
    directory_entry_t *dir_entry = (directory_entry_t *)DIR_BUFFER_ADDRESS;

    kwrite("Listing files:\n");

    for (int i = 0; i < fs_header->number_of_file_entries; i++) {
        if (dir_entry[i].filename[0] != '\0') {
            kwrite("File: ");
            kwrite(dir_entry[i].filename);
            kwrite("\n");
        }
    }
}

/* Função para converter um número inteiro em uma string */
void itoa(int num, char *str, int base) {
    int i = 0;
    int is_negative = 0;

    /* Lida com números negativos apenas para a base decimal */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0 && base == 10) {
        is_negative = 1;
        num = -num;
    }

    /* Processo de conversão */
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    /* Adiciona sinal negativo, se necessário */
    if (is_negative)
        str[i++] = '-';

    str[i] = '\0';

    /* Inverte a string */
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

