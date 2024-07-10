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

#include "bios2.h"		/* For udelay().      */
#include "kaux.h"		/* For ROWS and COLS. */

/* Video RAM as 2D matrix: short vram[row][col]. */

short (*vram)[COLS] = (short (*)[COLS])0xb8000;

char character_color = 0x02;	/* Default fore/background character color.*/

/* Write 'string' starting at the position given by 'row' and 'col'.
   Text is wrapped around both horizontally and vertically. 

   The implementation manipulates the video-RAM rather than BIOS services.
*/

void writexy(unsigned char row, unsigned char col, const char* string)
{
  int k=0;
  
  while (string[k])
    {

      col = col % COLS;
      row = row % ROWS;
      
      vram[row][col] = color_char(string[k]);
      col++;
      k++;
    }
}

/* Clear the entire screen

   The implementation manipulates the video-RAM rather than BIOS services.

 */

void clearxy()
{
  int i,j;

  for (j=0; j<ROWS; j++)
    for (i=0; i<COLS; i++)
      vram[j][i] = color_char(' ');
}

/* A not-that-impressive splash screen that is entirely superfluous. */

extern const char logo[];
void splash(void)
{
  int i,j, k;

  clearxy();

  for (i=0; i<COLS; i++)
    {
      for (j=0; j<ROWS; j+=2)
	{
	  vram[j][i] = color_char(logo[j*COLS+i]);
	  vram[j+1][COLS-i] = color_char(logo[(j+1)*COLS+(COLS-i)]);
	  udelay (1);
	}
    }

  udelay (500);
  clearxy();
}

/* Return 0 is string 's1' and 's2' are equal; return non-zero otherwise.*/

int strcmp(const char *s1, const char *s2)
{
  while (*s1 && *s2 && *s1 == *s2) {
    s1++;
    s2++;
  }
  return (*s1 - *s2);
}

void LerDisco(int sector_coordinate, int readSectors, void *target_address) {
    __asm__ volatile(
        "pusha \n"                    // Salva todos os registradores na pilha
        "mov boot_drive, %%dl \n"     // Seleciona o drive de boot (vindo de rt0.o)
        "mov $0x2, %%ah \n"           // Serviço de disco BIOS: operação de ler setor
        "mov %[sectToRead], %%al \n"  // Quantos setores ler
        "mov $0x0, %%ch \n"           // Coordenada do cilindro (começa em 0)
        "mov %[sectCoord], %%cl \n"   // Coordenada do setor (começa em 1)
        "mov $0x0, %%dh \n"           // Coordenada da cabeça (começa em 0)
        "mov %[targetAddr], %%bx \n"  // Onde carregar os dados
        "int $0x13 \n"                // Chama o serviço de disco BIOS 0x13
#if 0
	"mov $error%=, %%cx \n"
	"jc fatal \n"
	"jmp end%=\n"
#endif	
        "popa \n"                     // Restaura os registradores da pilha
#if 0
	"error%=: \n"
	" .string \"Read failed\\n\"  \n"
	"end%=:"
#endif
	
        ::
        [sectCoord] "g"(sector_coordinate),
        [sectToRead] "g"(readSectors),
        [targetAddr] "g"(target_address)
    );
}


