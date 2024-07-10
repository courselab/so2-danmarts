/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 Ana Júlia Tagliassachi
 *    SPDX-FileCopyrightText: 2024 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 danmartsar <daniel.martins@lonvi.com.br>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *    This file is part of SYSeg, available at https://gitlab.com/monaco/syseg.
 */


#include "bios1.h"  
#include "bios2.h"  
#include "kernel.h" 
#include "kaux.h"   
#include <stdio.h>


/* Endereços onde o cabeçalho do sistema de arquivos e o buffer de diretório serão carregados */
#define FS_HEADER_ADDRESS   0x7C00 
#define DIR_BUFFER_ADDRESS  0x9000  


/* Variáveis globais */
char buffer[BUFF_SIZE];
int go_on = 1;

/* Comandos embutidos */
static struct cmd_t cmds[] = {
    {"help", f_help},
    {"quit", f_quit},
    {"ls", f_list},
    {0, 0}
};

struct fs_header_t *get_fs_header() {
  return (struct fs_header_t *)0x7c00;
}

/* Função principal do kernel */
void kmain(void) {
    register_syscall_handler(); /* Registra o manipulador de syscall no int 0x21 */
    //    splash();       /* Tela de splash desnecessária */
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

	/* No maching bilt-in command. */
        if (!cmds[i].funct) {
	    f_exec(buffer);
        }
    }
}

/* Comando embutido: help */
void f_help(void) {
    kwrite("...me, Charli, you're my only hope!\n\n");
    kwrite("   But we can try also some commands:\n");
    kwrite("      quit    (to exit BratOS)\n");
    kwrite("      ls      (to list files)\n");
}

/* Comando embutido: quit */
void f_quit(void) {
    kwrite("About to Crash. Bye.");
    go_on = 0;
}

/* Comando: list */
void f_list() {
  // Obtém o cabeçalho do sistema de arquivos
  struct fs_header_t *header = get_fs_header();

  // Calcula a coordenada inicial do setor da área de diretórios
  int dir_start_sector = 1 + header->number_of_boot_sectors;

  // Calcula o número de setores a serem lidos para carregar todas as entradas de diretório
  int dir_sectors_to_read = header->number_of_file_entries * 32 / 512;

  // Ponteiro para o pool de memória onde os dados do diretório serão carregados
  extern byte _MEM_POOL;
  void *dir_section_memory = (void *)&_MEM_POOL;

  // Carrega os setores do disco para a memória
  LerDisco(dir_start_sector, dir_sectors_to_read, dir_section_memory);

  // Itera sobre cada entrada de diretório
  for (int entry_index = 0; entry_index < header->number_of_file_entries; entry_index++) {
    // Obtém o nome do arquivo na posição atual da entrada de diretório
    char *entry_name = dir_section_memory + entry_index * 32;

    // Se o nome do arquivo não for vazio, escreve-o na saída
    if (entry_name[0]) {
      kwrite(entry_name);
      kwrite("\n");
    }
  }
}


/* Comando: exec */
void f_exec(const char* binary_file_name) {

  struct fs_header_t *fs_header = get_fs_header();

  int directory_sector_coordinate = 1 + fs_header->number_of_boot_sectors;
  int sectors_to_read = fs_header->number_of_file_entries * 32 / 512 + 1;

  int memoryOffset = fs_header->number_of_file_entries * 32 - (sectors_to_read - 1) * 512;

  extern byte _MEM_POOL;
  void *directory_section = (void *)&_MEM_POOL;
  
  LerDisco(directory_sector_coordinate, sectors_to_read, directory_section);
  
  int bin_sector_coordinate = -1;
  for (int i = 0; i < fs_header->number_of_file_entries; i++) {
    char *file_name = directory_section + i * 32;
    if (!strcmp(file_name, binary_file_name)) {
      bin_sector_coordinate = directory_sector_coordinate + sectors_to_read + fs_header->max_file_size * i - 1;
      break;
    }
  }
  if (bin_sector_coordinate == -1) {
    kwrite("Program not found.\n");
    return;
  }

  void *program = (void *)(0xFE00);
  void *program_sector_start = program - memoryOffset;
  
  LerDisco(bin_sector_coordinate, fs_header->max_file_size, program_sector_start);

  exec_prog();

  /* All this was disabled (can be safely removed).
     Function exec_prog implemented in bios2.S. */
  
  /*
  __asm__ volatile(
      "  call get_return_addr_into_ebx \n"  // coloca o return address em ebx

      "original_return_addr: \n"  // será usado para calcular o valor que deve ser adicionado à stack

      "  push %%ebx \n"  // colocar o ebx na stack

      "  jmp *%[progAddr] \n"  // jump pra main
      "get_return_addr_into_ebx: \n"
      "  mov (%%esp), %%ebx \n"  // coloca o topo da stack em ebx

      "  mov $prog_finish, %%ecx \n"           // ecx = endereço de prog_finish
      "  sub $original_return_addr, %%ecx \n"  // ecx -= endereço de original_return_addr

      "  add %%ecx, %%ebx \n"  // soma ecx em ebx, ou seja, faz com que ebx aponte para prog_finish
      "  ret \n"

      "prog_finish:"

      ::[progAddr] "r"(program));

  */
  
}


