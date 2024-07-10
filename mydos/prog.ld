/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 Ana Júlia Tagliassachi
 *    SPDX-FileCopyrightText: 2024 danmartsar <daniel.martins@lonvi.com.br>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *    This file is part of SYSeg, available at https://gitlab.com/monaco/syseg.
 */

/* This is the linker script used to build user programs. */

OUTPUT_FORMAT(binary)          /* Output flat binary (no structure). */
SECTIONS
{
    PRG_LOAD_ADDR = 0xFE00;    /* Program load address.              */
    
    . = PRG_LOAD_ADDR;
    
    .bin :
    {
        crt0.o (.text)         /* Include crt0.o first */
        *.o (.text .data .bss .rodata) /* Command line objects. */
        libtydos.a (.text .data .bss .rodata) /* Runtime library. */
    }       
}
STARTUP(crt0.o)
INPUT(libtydos.a)              /* Link with the TyDOS user library. */

