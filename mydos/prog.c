/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 Ana Júlia Tagliassachi
 *    SPDX-FileCopyrightText: 2024 Daniel Martins Arrais>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *    This file is part of SYSeg, available at https://gitlab.com/monaco/syseg.
 */

#include "tydos.h"

char name[50];

void main()
{
    puts("Enter your name: ");
    gets(name);
    puts("Hello ");
    puts(name);
}

