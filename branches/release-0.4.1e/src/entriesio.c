/*
 *  This file is part of x48, an emulator of the HP-48sx Calculator.
 *  Copyright (C) 1994  Eddie C. Dost  (ecd@dressler.de)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* $Log: entriesio.c,v $
 * Revision 1.0  1999/01/11  18:11:25  ecd
 * Initial revision
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "hp48.h"
#include "global.h"
#include "resources.h"
#include "hp48_emu.h"
#include "entriesio.h"
#include "rpl.h"

#define  CK0     0x18F9D 
#define  CK1     0x18FB2
#define  CK2     0x18FA9
#define  CK1n    0x18ECE
#define  CK2n    0x18EDF
#define  CK3n    0x18EF0
#define  CK4n    0x18F01
#define  CK5n    0x18F12  

int
#ifdef __FunctionProto__
match_rom(word_20 addr)
#else
match_rom(addr)
   word_20 addr;
#endif
{
  int      j ;

  for ( j = 0; j <= saturn.entries_num; j++ ) {
    if ( saturn.entries[j].rom == addr ) {
      return j;
    }
  }
  return 0;
}

int
#ifdef __FunctionProto__
match_entry(word_20 addr)
#else
match_entry(addr)
   word_20 addr;
#endif
{
  int      j ;

  for ( j = 0; j <= saturn.entries_num; j++ ) {
    if ( saturn.entries[j].brk == addr ) {
      return j;
    }
  }
  return 0;
}

int
#ifdef __FunctionProto__
closest_entry(word_20 addr)
#else
closest_entry(addr)
   word_20 addr;
#endif
{
  word_20  low_addr;
  int      j, k=0;

  low_addr = 0xfffff;
  for ( j = 0; j <= saturn.entries_num; j++ ) {
    if ( saturn.entries[j].rom - addr <= low_addr - addr &&
       (labs(saturn.entries[j].rom - addr) == saturn.entries[j].rom - addr)) {
      low_addr = saturn.entries[j].rom;
      k = j;
    }
  }
  return k;
}

int
#ifdef __FunctionProto__
get_prolog(char *name)
#else
read_entries_file(name, table)
   char *name;
#endif
{
  struct stat st;
  FILE *fpin, *fpout;
  unsigned char four[5], label[15];
  char in[1024], out[1024];
  int i;
  word_20  prolog, addr; 

  strcpy(in, name);
  strcat(in, "entries.in");

  strcpy(out, name);
  strcat(out, "entries.out");

  printf("Finding prologs from %s into %s\n", in, out);
  if (NULL == (fpin = fopen(in, "r")))
    {
      fprintf(stderr, "can\'t open %s\n", in);
      return 0;
    }

  if (stat(in, &st) < 0)
    {
      fprintf(stderr, "can\'t stat %s\n", in);
      fclose(fpin);
      return 0;
    }

  if (NULL == (fpout = fopen(out, "w")))
    {
      fprintf(stderr, "can\'t open %s\n", out);
      return 0;
    }

  if (stat(out, &st) < 0)
    {
      fprintf(stderr, "can\'t stat %s\n", out);
      fclose(fpout);
      return 0;
    }

while(fread(four,1,1,fpin)) {
   fseek(fpin, -1, SEEK_CUR);
  while ( fread(four, 1, 1, fpin) && strncmp( "\t", four ,1 ) )
    fwrite( four, 1, 1, fpout);

  fwrite( four, 1, 1, fpout);
  fread(four, 1, 1, fpin);
  fwrite( four, 1, 1, fpout);   /* # */

  fread(four, 1, 5, fpin);
  fwrite( four, 1, 5, fpout);  

  addr = 0;
  for (i = 0; i < 5; i++) {
      addr <<= 4;
      if ('0' <= four[i] && four[i] <= '9')
        {
          addr |= ((int) four[i] - (int) '0');
        }
      else if ('A' <= four[i] && four[i] <= 'F')
        {
          addr |= ((int) four[i] - (int) 'A' + 10);
        }

     }

  sprintf(label, "\t%.5lx\t",addr);
  fwrite(label, 1, 7, fpout);
  
  prolog = read_nibbles(addr, 5);

  if (addr + 5 == prolog)
    sprintf(label,"PRIMI\t");
  else
    sprintf(label, "%.5lx\t",prolog);
  fwrite(label, 1, 6, fpout);

  while ( fread(four, 1, 1, fpin) && strncmp( "\n", four ,1 ) )
    fwrite( four, 1, 1, fpout);

  fwrite( four, 1, 1, fpout);
 
} 
  fclose(fpout);
  fclose(fpin);

  return 0;
}

int
#ifdef __FunctionProto__
read_file(char *name, entries_t *table, int type)
#else
read_entries_file(name, table)
   char *name;
   entries_t *table,
   int type;
#endif
{
  struct stat st;
  struct entries_t new;
  FILE *fp;
  unsigned char four[4], label[15];
  int i, k, m ;
  word_20  j ;
  /* word_20 jumpaddr; */

  /* the var that is pasted is not the file name anymore, it is the
     path, so we can read /path/supported.o & /path/entries.o
   */

  if (NULL == (fp = fopen(name, "r")))
    {
      fprintf(stderr, "can\'t open %s\n", name);
      return 0;
    }

  if (stat(name, &st) < 0)
    {
      fprintf(stderr, "can\'t stat %s\n", name);
      fclose(fp);
      return 0;
    }

  if (fread(four, 1, 4, fp) != 4)
    {
      fprintf(stderr, "can\'t read first 4 bytes of %s\n", name);
      fclose(fp);
      return 0;
    }

  if (fseek(fp, 0, 0) < 0)
    {
      fprintf(stderr, "can\'t fseek to position 0 in %s\n", name);
      fclose(fp);
      return 0;
    }

  k = saturn.entries_num ;
  /* Get to the beginning of the Symbol File */ 
  while ( fread(four, 1, 4, fp) && strncmp( "Symb", four ,4 ) )
    fseek(fp, -3, SEEK_CUR);

  /* read label then address */
  while ( fread ( label, 1, 12, fp ) >= 10 )
  { 
    if (!strncmp( "Symb", label ,4 ))
      {
        fseek(fp, -8, SEEK_CUR);
      }
    else
      {

        j = 0;
        for (i = 0; i < 6; i++)
         {
            fread ( four,  1, 1, fp );
            j = (j << 8) + four[0];
          }

        i = 0;
        while (label[i] != ' ' && i <= 11)
          i++;
          
        strncpy(new.defn, label, i);
        new.defn[i]= '\0';

        /* Look for an existing one */
        m = 0;
        while ( (m < k) && table[m].rom != j )
          {
            m++;
          }

        if (k == 0)
          m = 0;

        if ( m == k )
        {
          new.rom = j;
          new.flags = type;
          if (read_nibbles(j,5) == j+5)
            new.brk = j+5;
          else
            new.brk = j;


          if (verbose)
            printf("added %i : %.5lX : \'%s\'\n",k, j, new.defn);

          table[ k ] = new ;
          k++;
        }
        else
        {
           if (verbose)
             printf(" Dup supported entry at %.5lx : %s, & %s\n m:%i k:%i\n",
               j, new.defn, table[m].defn , m, k);
        }
      }
  }
  fclose(fp);

 /*  if (verbose) */
  printf("%s: read %i entries from %s\n", 
          progname, k - saturn.entries_num, name);
  saturn.entries_num = k +1;

  return 1;
}

int
#ifdef __FunctionProto__
search_for_dispatch(int num, word_32 first)
#else
search_for_dispatch(num, first)
  int num;
  word_32  first;
#endif
{
  word_32  prolog,i, ptr, ptr2;
  word_32  value[6], value1[6];
  int      j, m;

  for (i = 5; i <=0x70000 ; i++) {
    prolog = read_nibbles(i,5);
    if( prolog == first  && read_nibbles(i-5,5) == DOCOL ){
      ptr = i+5;
      for ( m = 0; m < num ; m++)
        value1[m] = 0;
      while ( read_nibbles(ptr, 5) != SEMI) {
        prolog = read_nibbles(ptr,5);
          ptr2 = read_nibbles(prolog,5);
          if (ptr2 == DOBINT) {
            prolog = read_nibbles(prolog+5,5);

            ptr2 = read_nibbles(ptr+5,5);           
            printf("%.5lx : %i : ", ptr2, num);
            for ( m = 0; m < num ; m++){
              ptr2 = prolog & (0xf << (4*m ));
              ptr2 = ptr2 >> (4*m);
              if (ptr2 == 0xF)
                printf("error ********************************** \n");
              if (ptr2 != 0) {
                ptr2--;
                value[m] = 1 << ptr2;
                value1[m] |= value[m] ;
              } else {
                value[m] = 0;
              }
              printf("%.8lx ", value[m]);
            }
            printf("\n");
          }
        ptr += 10;
      }
      printf("%.5lX : %i :", i - 5, num);
      for ( m = 0; m < num ; m++)
        printf(" %.8lX", value1[m] );
      printf("\n");
      
    }
  }
  return 0;
}

int
#ifdef __FunctionProto__
read_entries_file(char *name, entries_t *table)
#else
read_entries_file(name, table)
   char *name;
   entries_t *table,
#endif
{
  char           fnam[1024];
  int            type;

  strcpy(fnam, name);
  strcat(fnam, "supported.o");
  type = GX_ENTRY | SX_ENTRY | SUPPORTED_ENTRY;
  read_file(fnam, table, type);

  strcpy(fnam, name);
  strcat(fnam, "unsupported.o");
  type = GX_ENTRY | SX_ENTRY ;
  read_file(fnam, table, type);

  strcpy(fnam, name);
  strcat(fnam, "sxonly.o");
  type = SX_ENTRY ;
  read_file(fnam, table, type);

  strcpy(fnam, name);
  strcat(fnam, "gxonly.o");
  type = GX_ENTRY ;
  read_file(fnam, table, type);

/*   get_prolog(name); */
/*  search_for_dispatch(1, CK1);
  search_for_dispatch(1, CK1n);
  search_for_dispatch(2, CK2);
  search_for_dispatch(2, CK2n);
  search_for_dispatch(3, CK3n);
  search_for_dispatch(4, CK4n);
  search_for_dispatch(5, CK5n);
*/
  return 1;

}

