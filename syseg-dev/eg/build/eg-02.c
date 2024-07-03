/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *    This file is part of SYSeg, available at https://gitlab.com/monaco/syseg.
 */

#include <stdio.h>

foo_t foo();

int main()
{
  int b;
  b = foo(512);

  printf("%d\n", b);

  return b;
}

foo_t foo(int a)
{
  return a+1;
}


