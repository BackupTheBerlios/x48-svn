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

/* $Log: debugger.c,v $
 * Revision 1.8  1995/01/11  18:20:01  ecd
 * major update to support HP48 G/GX
 *
 * Revision 1.7  1994/12/07  20:20:50  ecd
 * more functions
 *
 * Revision 1.7  1994/12/07  20:20:50  ecd
 * more functions
 *
 * Revision 1.6  1994/11/28  02:00:51  ecd
 * new functions: do_ram, do_stack
 *
 * Revision 1.5  1994/11/02  14:40:38  ecd
 * more functions
 *
 * Revision 1.4  1994/10/09  20:29:47  ecd
 * start of disassembler implementation.
 *
 * Revision 1.3  1994/10/06  16:30:05  ecd
 * added refresh_display()
 *
 * Revision 1.2  1994/10/05  08:36:44  ecd
 * more functions
 *
 * Revision 1.1  1994/10/04  15:12:21  ecd
 * Initial revision
 *
 *
 * $Id: debugger.c,v 1.8 1995/01/11 18:20:01 ecd Exp ecd $
 */

#include "global.h"

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#ifdef SUNOS
#include <memory.h>
#endif
#include <X11/Xlib.h>

#include "hp48.h"
#include "hp48_emu.h"
#include "device.h"
#include "timer.h"
#include "x48_x11.h"
#include "debugger.h"
#include "disasm.h"
#include "rpl.h"
#include "romio.h"
#include "resources.h"
#include "init.h"
#include "hp48char.h"
#include "append.h"
#include "gui.h"
#include "entriesio.h"


extern char *readline __ProtoType__ ((const char *));
extern void add_history __ProtoType__ ((char *));

#define MAX_ARGS 16
#define BIN 1
#define ASCII 0

int send_mode = BIN;

char *send_name[] =
{
  "ASCII",
  "Binary"
};

bkreg_t bkreg[] = {
 { "A",  16, saturn.A ,         0 },
 { "B",  16, saturn.B ,         0 },
 { "C",  16, saturn.C ,         0 },
 { "D",  16, saturn.D ,         0 },
 { "R0", 16, saturn.R0 ,        0 },
 { "R1", 16, saturn.R1 ,        0 },
 { "R2", 16, saturn.R2 ,        0 },
 { "R3", 16, saturn.R3 ,        0 },
 { "R4", 16, saturn.R4 ,        0 },
 { "D0",  5,         0 , saturn.d },
 { "D1",  5,         0 , saturn.d },
 { "IN",  4, saturn.IN ,        0 },
 { "OUT", 3, saturn.OUT,        0 }
};

int enter_debugger = 0;
int in_debugger = 0;
int exec_flags = 0;

int continue_flag;
static char instr[100];

#define MAX_BREAKPOINTS	32
int num_bkpts;

struct breakpoint
  {
    word_20 addr;
    word_20 end_addr;
    word_64 mask;
    word_64 value;
    int flags;
  }

bkpt_tbl[MAX_BREAKPOINTS + 1];

/*
 * command functions
 */
static void do_break __ProtoType__ ((int, char **));
static void do_continue __ProtoType__ ((int, char **));
static void do_crc __ProtoType__ ((int, char **));
static void do_delete __ProtoType__ ((int, char **));
static void do_dis __ProtoType__ ((int, char **));
static void do_diso __ProtoType__ ((int, char **));
static void do_diss __ProtoType__ ((int, char **));
static void do_exit __ProtoType__ ((int, char **));
static void do_find __ProtoType__ ((int, char **));
static void do_finish __ProtoType__ ((int,char **));
static void do_go __ProtoType__ ((int, char **));
static void do_help __ProtoType__ ((int, char **));
static void do_load __ProtoType__ ((int, char **));
static void do_lib  __ProtoType__ ((int, char **));
static void do_lcd __ProtoType__ ((int, char **));
static void do_list __ProtoType__ ((int, char **));
static void do_ls __ProtoType__ ((int, char **));
static void do_mode __ProtoType__ ((int, char **));
static void do_quit __ProtoType__ ((int, char **));
static void do_regs __ProtoType__ ((int, char **));
static void do_receive __ProtoType__ ((int, char **));
static void do_save __ProtoType__ ((int, char **));
static void do_send __ProtoType__ ((int, char **));
static void do_stack __ProtoType__ ((int, char **));
static void do_stat __ProtoType__ ((int, char **));
static void do_step __ProtoType__ ((int, char **));
static void do_trace __ProtoType__ ((int, char **));
static void do_ram __ProtoType__ ((int, char **));
static void do_reset_calc __ProtoType__ ((int, char **));
static void do_rstk __ProtoType__ ((int, char **));

struct cmd
  {
    char *name;
    void (*func) __ProtoType__ ((int, char **));
    char *help;
  }

cmd_tbl[] =
{
  {
    "break", do_break,
    "break [address ]            Set breakpoint at `address\' or show breakpoints,\n      [addr_low addr_high]  Set breakpoint range, from low to high,\n      [sysRPL_keyword]      Set breakpoint at System RPL keyword "
  }
  ,
  {
    "b", do_break, 0
  }
  ,

  {
    "cont", do_continue,
    "cont                       Continue execution"
  }
  ,
  {
    "crc", do_crc,
    "crc addr len               Calc crc of memory at addr to len"
  }
  ,
  {
    "c", do_continue, 0
  }
  ,
  {
    "dis", do_dis,
    "dis [addr] [addr n]        Disassemble starting at PC or addr, n times"
  }
  ,
  {
    "diso", do_diso,
    "diso [addr]                Disassemble object at addr"
  }
  ,
  {
    "diss", do_diss,
    "diss [addr]                Dissassemble secondary at addr"
  }
  ,
  {
    "delete", do_delete,
    "delete [all | n]           Delete breakpoint or watchpoint number `n\',\n                           all breakpoints, or current breakpoint"
  }
  ,
  {
    "exit", do_exit,
    "exit                       Exit the emulator without saving"
  }
  ,
  {
    "finish", do_finish,
    "finish                     Finish current assembly subroutine"
  }
  ,
  {
    "find", do_find,
    "find addr obj              Search for obj starting at addr"
  }
  ,

  {
    "go", do_go,
    "go address                 Set PC to `address\'"
  }
  ,

  {
    "help", do_help,
    "help                       Display this information"
  }
  ,
  {
    "h", do_help, 0
  }
  ,
  {
    "?", do_help, 0
  }
  ,
  {
    "list", do_list,
    "list [addr] [addr n]       List data starting at addr, n times "
  }
  ,
  {
    "lib", do_lib,
    "lib [addr]                 Disassemble Library at addr"
  }
  ,
  {
    "lcd", do_lcd,
    "lcd /path                  Change local working directory"
  }
  ,
  {
    "load", do_load,
    "load                       Load emulator-state from files"
  }
  ,
  {
    "ls", do_ls,
    "ls addr                    list directory contents"
  }
  ,

  {
    "mode", do_mode,
    "mode [hp | class | sysRPL]          Show or set disassembler mode"
  }
  ,

  {
    "quit", do_quit,
    "quit                       Exit the emulator after saving its state"
  }
  ,
  {
    "q", do_quit, 0
  }
  ,

  {
    "ram", do_ram,
    "ram                        Show RAM layout"
  }
  ,

  {
    "reg", do_regs,
    "reg [register [hexvalue]]  Display or set register value"
  }
  ,
  {
    "r", do_regs, 0
  }
  ,
  {
    "receive", do_receive,
    "receive file               receive file into HP48"
  }
  ,
  {
    "reset", do_reset_calc,
    "reset                      Set the HP48\'s PC to ZERO"
  }
  ,

  {
    "save", do_save,
    "save                       Save emulator-state to files"
  }
  ,
  {
    "send", do_send,
    "send ob file_name          Send ob to file, ob can be name or addr"
  }
  ,
  {
    "stack", do_stack,
    "stack                      Display RPL stack"
  }
  ,

  {
    "stat", do_stat,
    "stat                       Display statistics for the emulator"
  }
  ,

  {
    "step", do_step,
    "step [n]                   Step one or n Instruction(s)"
  }
  ,
  {
    "s", do_step, 0
  }
  ,
  {
   "trace", do_trace, 0
  }
  ,

  {
    "where", do_rstk,
    "where                      Show ML return stack"
  }
  ,

  {
    0, 0, 0
  }
};

void
#ifdef __FunctionProto__
init_debugger (void)
#else
init_debugger ()
#endif
{
  int i;

  num_bkpts = 0;
  for (i = 0; i < MAX_BREAKPOINTS; i++)
    bkpt_tbl[i].flags = 0;
  exec_flags = 0;
}

static int
#ifdef __FunctionProto__
decode_64 (word_64 * addr, char *arg)
#else
decode_64 (addr, arg)
     word_64 *addr;
     char *arg;
#endif
{
  int i;

  if (arg == (char *) 0)
    {
      printf ("Command requires an argument.\n");
      return 0;
    }

  addr->lo = addr->hi = 0;
  for (i = 0; i < strlen (arg); i++)
    {
      addr->hi <<= 4;
      addr->hi |= ((addr->lo >> 28) & 0x0f);
      addr->lo <<= 4;
      if ('0' <= arg[i] && arg[i] <= '9')
        {
          addr->lo |= ((int) arg[i] - (int) '0');
        }
      else if ('A' <= arg[i] && arg[i] <= 'F')
        {
          addr->lo |= ((int) arg[i] - (int) 'A' + 10);
        }
      else
        {
          addr->hi = addr->lo = 0;
          printf ("Not a number: %s.\n", arg);
          return 0;
        }
    }
  return 1;
}

void
#ifdef __FunctionProto__
read_reg (word_64 * val, unsigned char *r)
#else
read_reg (val, r)
     word_64 *val;
     unsigned char *r;
#endif
{
  int i;

  val->lo = val->hi = 0;
  for (i = 0; i < 16; i++)
    {
      if (i < 8)
        val->lo += (word_32) r[i] << (4 * i);
      else
        val->hi += (word_32) r[i] << (4 * (i-8));
    }
}

int
#ifdef __FunctionProto__
check_breakpoint (int type, word_20 addr)
#else
check_breakpoint (type, addr)
     int type;
     word_20 addr;
#endif
{
  struct breakpoint *bp;
  int i, n;
  word_64 reg;

  bp = bkpt_tbl;
  n = num_bkpts;
  i = 0;
  for (; n > 0; bp++)
    {
      i++;
      if (bp->flags == 0)
	continue;
      n--;
      if (bp->flags == BP_RPL) {
        /* 0xC808 is the OPcode for jmp (A)  */
        if (read_nibbles(saturn.old_PC[saturn.old_index],4) == 0xC808 ) {
          read_reg(&reg,  bkreg[0].reg0);
          if ( ((reg.lo & 0xFFFFF) == bp->value.lo ) && 
                       saturn.D0 == bp->value.hi ) {
            goto hit_it;
          }
        }
      }
      if (bp->flags & BP_REGISTER) 
        {
           if (bkreg[bp->flags & 0x0f].reg0)
             read_reg(&reg, bkreg[bp->flags & 0x0f].reg0);
           else 
             reg.lo = saturn.d[bkreg[bp->flags & 0x0f].name[1]-48];
           if( (reg.lo & bp->mask.lo) == bp->value.lo && 
               (reg.hi & bp->mask.hi) == bp->value.hi )
             goto hit_it;
        }
      if (bp->flags == BP_RANGE && addr >= bp->addr && addr <= bp->end_addr)
	{
	  goto hit_it;
	}
      if (bp->flags & type && addr == bp->addr)
	{
	hit_it:
          if ( type & BP_TEMP) {
            printf("Temp BreakPoint - erasing\n");
              num_bkpts--;
              bp->addr = 0;
              bp->flags = 0;
          }
	  if (type == BP_READ)
	    {
	      printf ("%.5lX: Read watchpoint %d hit at %.5lX\n", saturn.PC,
		      i, addr);
	    }
	  else if (type == BP_WRITE)
	    {
	      printf ("%.5lX: Write watchpoint %d hit at %.5lX\n", saturn.PC,
		      i, addr);
	    }
	  else if (bp->flags & BP_REGISTER)
            {
              printf ("%.5lX Register breakpoint %d hit at %.5lX Flag:%i\n",
                      saturn.PC, i, addr, bp->flags);
            }
          else
	    {
	      printf ("Breakpoint %d hit at %.5lX\n", i, addr);
	    }
	  return 1;
	}
    }
  return 0;
}

word_20
#ifdef __FunctionProto__
where_start (word_20 middle)
#else
where_start(middle)
  word_20  middle;
#endif
{
  word_20 addr, addr1, start, len;
  int     leave;

  /* check to see if it is in the home directory */
  addr  = read_nibbles(0x70592,5);
  addr1 = read_nibbles(0x70597,5);

  if ( middle >= addr && middle<= addr1 ) {
    addr += 5;
    start = read_nibbles(addr,3);
    addr += 3;
    for ( ; start >= 1 ; start-- )
      addr+=13;


    if ( read_nibbles(addr,5) != 0 ) {
      addr += read_nibbles(addr,5);
      start = addr-5;
      leave = 0;
      while (!leave) {
        if (middle >= addr) {
          len = read_nibbles(addr,2);
          if (len)
            addr += (len+2)*2;
          else
            addr +=2;
          if (read_nibbles(addr, 5) == DOCOL)
            leave = 1;
          else {
            addr += 8;
            addr += read_nibbles(addr,5);
            start = addr - 5;
          }
        } else {
          addr = start - read_nibbles(start, 5);
          start = addr - 5;
        }
      }
      return addr;  /* Sucess */
    }
    return 0;       /* Something wrong */
  }

  /* It's not in the directory.... let try attached libs in Port 0 */

  addr = addr1;
  addr1 = 0xC0000;
  leave = 0;

  if ( middle >= addr && middle<= addr1 ) {
    while (!leave) {
      len = read_nibbles(addr+5, 5);
      if (middle <= addr+len+5) {
        if (read_nibbles(addr, 5) == DOBAK) {
          addr +=10;
          len = read_nibbles(addr,2);
          if (len)
            addr += (len+2)*2;
          else
            addr +=2;
        } else {  /* groan - it is inside a library, go thru the hash tbl */
          printf("Inside Library at %.5lx\n", addr);
        }
        leave = 1;
        break;
      }
      addr += len+5;
    }
    return addr;
  }
  return 0;
}

static word_20
#ifdef __FunctionProto__
where_lib (int lib_num)
#else
where_lib (lib_num)
     int lib_num;
#endif
{
  word_20  num, addr, name_len;

  addr = read_nibbles(0x70597,5);

  while(read_nibbles(addr,5) == DOLIB)
  {
    addr+=5;
    name_len = read_nibbles(addr+5, 2);
    if (name_len == 0)
      num = read_nibbles(addr+7, 3);
    else
      num = read_nibbles(addr + 2 * name_len + 9, 3);
    if ((int)num == lib_num)
      return addr-5;
    addr += read_nibbles(addr,5);

  }
  return 0;
}

static word_20
#ifdef __FunctionProto__
where (char *ls)
#else
where (ls)
     char *ls;
#endif
{

  word_20 addr,len, start;
  unsigned char    name[64];
  int     leave, home ,i ;

  addr = read_nibbles(0x7059C, 5);

  if (addr == read_nibbles(0x70592,5))
    home = 1;
  else
    home =0;

  if ( read_nibbles(addr, 5) == DORRP )
    {
      addr += 5;
      start = read_nibbles(addr,3);
      addr += 3;
      if ( home )
        {
          for ( ; start >= 1 ; start-- )
              addr+=13;
        }

      if ( read_nibbles(addr,5) != 0 )
        {
          addr += read_nibbles(addr,5);
          start = addr-5;
          leave = 0;
          while (! leave ) 
            {
              len = read_nibbles(addr, 2);
              addr +=2;
              if (len !=0 )
               {
                 i = 0;
                 for ( ; len >= 1; len -- )
                   {
                      name[i] = (int)read_nibbles(addr, 2);
                      i++;
                      addr +=2;
                   }
                 name[i]=0;
                 addr +=2;
               }
             
               if( ! strncmp(name, ls, strlen(ls) ))
                  return addr;
    
              if ( read_nibbles(start,5) == 0)
                leave = 1;
              else
                {
                  addr = start - read_nibbles(start, 5);
                  start = addr - 5;
                }
            } 

        }
      }
  /* Wasn't a var, lets see if it was a sysRPL keyword */
  i = 0;
  while ( i < saturn.entries_num && strcmp(saturn.entries[i].defn, ls))
    i++;
  if ( i < saturn.entries_num )
    {
      return saturn.entries[i].rom;
    }

  
  return 0;
}

char *
#ifdef __FunctionProto__
read_str(char *str, int n, int fp)
#else
read_str(str, n, fp)
	char *str;
	int n;
	int fp;
#endif
{
  int cc;
  int flags;

  while (1)
    {
      cc = read(fp, str, n);
      if (cc > 0)
        return str;
      if (cc == 0)
        return NULL;

      if (errno == EINTR)
        continue;

      if (errno == EAGAIN)
        {
          flags = fcntl(fp, F_GETFL, 0);
          flags &= ~ O_NONBLOCK;
          fcntl(fp, F_SETFL, flags);
          continue;
        }
 
      return NULL;
    }
}


static inline void
#ifdef __FunctionProto__
str_to_upper (char *arg)
#else
str_to_upper (arg)
     char *arg;
#endif
{
  int i;

  for (i = 0; i < strlen (arg); i++)
    {
      if ('a' <= arg[i] && arg[i] <= 'z')
	{
	  arg[i] = (char) ((int) arg[i] - (int) 'a' + (int) 'A');
	}
    }
}

static int
#ifdef __FunctionProto__
decode_dec (int *num, char *arg)
#else
decode_dec (num, arg)
     int *num;
     char *arg;
#endif
{
  int i;
  int sign=1;

  
  if (arg == (char *) 0)
    {
      printf ("Command requires an argument.\n");
      return 0;
    }

  *num = 0;
  for (i = 0; i < strlen (arg); i++)
    {
      if ('-' == arg[0] && i ==0)
        sign = -1;
      else
        {
          *num *= 10;
          if ('0' <= arg[i] && arg[i] <= '9')
            {
	      *num += ((int) arg[i] - (int) '0');
	    }
          else
	    {
	      *num = 0;
	      printf ("Not a number: %s.\n", arg);
	      return 0;
	    }
        }
    }
    *num *= sign;
  return 1;
}

static int
#ifdef __FunctionProto__
decode_20 (word_20 * addr, char *arg)
#else
decode_20 (addr, arg)
     word_20 *addr;
     char *arg;
#endif
{
  int i;
  
  if (arg == (char *) 0)
    {
      printf ("Command requires an argument.\n");
      return 0;
    }


  *addr = 0;
  for (i = 0; i < strlen (arg); i++)
    {
      *addr <<= 4;
      if ('0' <= arg[i] && arg[i] <= '9')
	{
	  *addr |= ((int) arg[i] - (int) '0');
	}
      else if ('A' <= arg[i] && arg[i] <= 'F')
	{
	  *addr |= ((int) arg[i] - (int) 'A' + 10);
	}
      else
	{
	  *addr = 0;
	  printf ("Not a number: %s.\n", arg);
	  return 0;
	}
      *addr &= 0xfffff;
    }
  return 1;
}

static int
#ifdef __FunctionProto__
decode_32 (word_32 * addr, char *arg)
#else
decode_32 (addr, arg)
     word_32 *addr;
     char *arg;
#endif
{
  int i;
  int sign;

  if (arg == (char *) 0)
    {
      printf ("Command requires an argument.\n");
      return 0;
    }

  *addr = 0;
  sign = 1;
  for (i = 0; i < strlen (arg); i++)
    {
      if ( i == 0 && arg[0] == (int) '-' )
        sign = -1;
      else
      {
        *addr <<= 4;
        if ('0' <= arg[i] && arg[i] <= '9')
    	  {
	    *addr |= ((int) arg[i] - (int) '0');
  	  }
        else if ('A' <= arg[i] && arg[i] <= 'F')
	  {
	    *addr |= ((int) arg[i] - (int) 'A' + 10);
	  }
        else
	  {
	    *addr = 0;
	    printf ("Not a number: %s.\n", arg);
	    return 0;
	  }
       }
    }
  *addr = *addr * sign;
  return 1;
}


char *
#ifdef __FunctionProto__
str_nibbles (word_20 addr, int n)
#else
str_nibbles (addr, n)
     word_20 addr;
     int n;
#endif
{
  static char str[1025];
  char *cp;
  int i;

  if (n > 1024)
    {
      str[0] = '\0';
      return str;
    }

  for (cp = str, i = 0; i < n; i++)
    {
      sprintf (cp, "%.1X", read_nibble (addr + i));
      cp++;
    }
  *cp = '\0';

  return str;
}

static int
#ifdef __FunctionProto__
confirm (const char *prompt)
#else
confirm (prompt)
     const char *prompt;
#endif
{
  char ans[80];

  printf ("%s (y or n) ", prompt);
  fflush (stdout);
  read_str (ans, sizeof (ans), 0);
  while (ans[0] != 'y' && ans[0] != 'Y' && ans[0] != 'n' && ans[0] != 'N')
    {
      printf ("Please answer y or n.\n");
      printf ("%s (y or n) ", prompt);
      fflush (stdout);
      read_str (ans, sizeof (ans), 0);
    }
  if (ans[0] == 'y' || ans[0] == 'Y')
    {
      return 1;
    }
  else
    {
      printf ("Not confirmed.\n");
      return 0;
    }
}
 
static void
#ifdef __FunctionProto__
disp_obj (word_20 *addr,int line_len, int full)
#else
disp_obj (addr, line_len full)
     word_20 *addr;
     int line_len;
     int full;
#endif
{
  word_20 end_addr, len;
  char    buf[65536], line[80];
  int     leave, point;

  leave = 1;
  point = 0;
  end_addr = *addr;
  decode_rpl_obj(*addr, &len, buf);

  printf("%.5lX -> %.5lX : ",*addr, *addr+len);

  while (leave)
    {
      if (strlen(buf) > (line_len+3) && !full)
         {
           sprintf(&buf[line_len], "...");
           buf[line_len+3] = '\0';
           strcpy(line, buf);
         }
      else if (strlen(buf) > (line_len + 3) )
         {
           strncpy(line, &buf[point], line_len);
           point +=line_len;
           sprintf(&line[line_len],"\n\t\t\t\t");
           line[line_len+3]='\0';
         }
      else
         strcpy (line, buf );

      printf("%s", line);
      if ( strlen(buf) <= 0 || (!full) || point > 60)
        leave =0;

    }
  printf("\n");

  *addr+=len;

}

int
#ifdef __FunctionProto__
break_range (word_20 addrl, word_20 addrh)
#else
break_range (addrl, addrh)
     word_20 addrl;
     word_20 addrh;
#endif
{
  int i;
  word_20 addr=0;;

  if (addrl > addrh ) {
    addr = addrl;
    addrl = addrh;
    addrh = addr;
  }
  for (i = 0; i < MAX_BREAKPOINTS; i++) {
    if (bkpt_tbl[i].flags == 0) {
      if ( addrl == addrh ) {
        bkpt_tbl[i].flags = BP_EXEC;
        bkpt_tbl[i].addr = addrl;
      } else {
        bkpt_tbl[i].flags = BP_RANGE;
        bkpt_tbl[i].addr = addrl;
        bkpt_tbl[i].end_addr = addrh;
      }
      num_bkpts++;
      return 1;
    }
  }
  return 0;
}

int
#ifdef __FunctionProto__
break_register (word_20 num, word_64 value, word_64 mask)
#else
break_register (num, value, mask)
     word_20  num;
     word_64  value;
     word_64  mask;
#endif
{
  int i;
 
  value.lo &= mask.lo;
  value.hi &= mask.hi;
 
  for (i = 0; i < MAX_BREAKPOINTS; i++) {
    if (bkpt_tbl[i].flags == 0) {
      bkpt_tbl[i].flags = BP_REGISTER | num;
      bkpt_tbl[i].mask.lo = mask.lo;
      bkpt_tbl[i].mask.hi = mask.hi;
      bkpt_tbl[i].value.lo = value.lo;
      bkpt_tbl[i].value.hi = value.hi;
 
      printf("Breakpoint %lx when %s = %.8lx%.8lx && %.8lx%.8lx\n", 
                num,
                bkreg[(int)num].name, value.hi, value.lo, mask.hi, mask.lo);

      num_bkpts++;
      return 1;
    }
  }
  return 0;
}

int
#ifdef __FunctionProto__
break_rpl (word_20 a, word_20 d)
#else
break_rpl (a, d)
     word_20 a;
     word_20 d;
#endif
{
  int i;

  for (i = 0; i < MAX_BREAKPOINTS; i++) {
    if (bkpt_tbl[i].flags == 0) {
      bkpt_tbl[i].flags = BP_RPL;
      bkpt_tbl[i].value.lo = a;
      bkpt_tbl[i].value.hi = d;
      num_bkpts++;
      return 1;
    }
  }
  return 0;
}

int
#ifdef __FunctionProto__
break_address (word_20 addr)
#else
break_addrress (addr)
     word_20 addr;
#endif
{
  int i;

  for (i = 0; i < MAX_BREAKPOINTS; i++) {
    if (bkpt_tbl[i].flags == 0) {
      bkpt_tbl[i].flags = BP_EXEC;
      bkpt_tbl[i].addr = addr;
      num_bkpts++;
      return 1;
    }
  }
  return 0;
}

static void
#ifdef __FunctionProto__
do_break (int argc, char **argv)
#else
do_break (argc, argv)
     int argc;
     char *argv;
#endif
{
  int i,j;
  word_20 addr, addrl, addrh;
  char in[20];

  switch(argc) 
    {
      case 1:
        for (i = 0; i < MAX_BREAKPOINTS; i++)
	{
	  if (bkpt_tbl[i].flags == 0)
	    continue;
	  if (bkpt_tbl[i].flags == BP_EXEC)
	    {
	      printf ("Breakpoint %d at 0x%.5lX", i + 1, bkpt_tbl[i].addr);

              j = 0;
              while ( j <= saturn.entries_num && 
                        saturn.entries[j].rom != bkpt_tbl[i].addr )
                j++;
              if ( j < saturn.entries_num ) 
                printf (" : %s\n",saturn.entries[j].defn); 
              else
                printf ("\n");

	    }
	  else if (bkpt_tbl[i].flags == BP_RANGE)
	    {
	      printf ("Range watchpoint %d at 0x%.5lX - 0x%.5lX\n", i + 1,
		      bkpt_tbl[i].addr, bkpt_tbl[i].end_addr);
	    }
          else
	    {
	      printf ("Watchpoint %d at 0x%.5lX\n", i + 1, bkpt_tbl[i].addr);
	    }
	}
      break;
  case 2:
      i = 0;
      strcpy( in, argv[1]);
      while ( i < saturn.entries_num && strcmp(saturn.entries[i].defn, in))
        i++;
      if ( i < saturn.entries_num )
        {
          addr = saturn.entries[i].rom;
          addrl = read_nibbles(addr, 5);
          if (addr + 5 == addrl )
            addr = addrl;
        }
      else
        {
          str_to_upper (argv[1]);
          if (!decode_20 (&addr, argv[1]))
       	    {
	      return;
	    }
        }
      for (i = 0; i < MAX_BREAKPOINTS; i++)
	{
	  if (bkpt_tbl[i].flags == 0)
	    {
	      bkpt_tbl[i].flags = BP_EXEC;
	      bkpt_tbl[i].addr = addr;
	      printf ("Breakpoint %d at 0x%.5lX", i + 1, bkpt_tbl[i].addr);
 
              j = 0;
              while ( j <= saturn.entries_num
                             && saturn.entries[j].rom != bkpt_tbl[i].addr )
                j++;
              if ( j <= saturn.entries_num )        
                printf (" : %s\n",saturn.entries[j].defn);
              else
                printf ("\n");

	      num_bkpts++;
	      return;
	    }
	}
      printf ("Breakpoint table full\n");
     break;

  case 3:
    str_to_upper (argv[1]);
    str_to_upper (argv[2]);
    addr = addrh = addrl = 0;
    decode_20 (&addrl, argv[1]);
    decode_20 (&addrh, argv[2]);

    if ( addrl < 1 && addrh < 1 )
      {
        return;
      }
    
    if ( addrl == addrh ) 
       return;

    if (addrl > addrh )
      {
        addr = addrl;
        addrl = addrh;
        addrh = addr;
      }
    for (i = 0; i < MAX_BREAKPOINTS; i++)
      {
        if (bkpt_tbl[i].flags == 0)
          {
            bkpt_tbl[i].flags = BP_RANGE;
            bkpt_tbl[i].addr = addrl;
            bkpt_tbl[i].end_addr = addrh;
            printf ("Range watchpoint %d at 0x%.5lX - 0x%.5lX\n", i + 1,
                    bkpt_tbl[i].addr, bkpt_tbl[i].end_addr);
            num_bkpts++;
            return;
          }
      }
    printf ("Breakpoint table full\n");
    break;
  }


}

static void
#ifdef __FunctionProto__
do_crc (int argc, char **argv)
#else
do_crc (argc, argv)
     int argc;
     char *argv;
#endif
{
  word_20 addr, nib, crc, start, len, stop;

  if (argc == 1)
    {
    stop = 0xC003E;
     {
      start = 0xC0005;
        {
           crc = 0;
           addr = start;
           len = 0;
           while (addr != stop )
             {
               nib = read_nibble(addr);
               crc = (crc >> 4) ^ (((crc ^ nib) & 0xf) * 0x1081);
               addr++;
               len++;
             }
              printf("CRC from %.5lX -> %.5lX : %lX\n", start, start+len, crc); 
        }
      }
    }
}

static void
#ifdef __FunctionProto__
do_continue (int argc, char **argv)
#else
do_continue (argc, argv)
     int argc;
     char *argv;
#endif
{
  continue_flag = 1;
}

static void
#ifdef __FunctionProto__
do_diss (int argc, char **argv)
#else
do_diss (argc, argv)
     int argc;     char *argv;
#endif
{
  word_20 addr, prolog, prolog_2 , xlib1, xlib2;
  char    buf[65536];
  int     level, j;

  level = 1;

  if (argc >= 2 )  {
    if ((addr=where( argv[1])) == 0 )
      {
        str_to_upper( argv[1]);
        if( !decode_20(&addr, argv[1]))
          addr = 0;
      }
   }

  if (addr != 0)
  {
    if( read_nibbles(addr, 5) == DOCOL )
    {
       j = 0;
       while ( j <= saturn.entries_num &&
               saturn.entries[j].rom != addr )
         j++;
      if ( j < saturn.entries_num )
        printf ("%.5lX -> %s\n",addr, saturn.entries[j].defn);
 
      printf(" %.5lX->%.5lX : Start #%.2i      : 7 : %.5lx :   ::\n",
                    addr, addr+5, level, (long)DOCOL);
      addr+= 5;
      while ( (prolog = read_nibbles(addr,5)) != SEMI || level != 1 )
      {
        prolog_2 = read_nibbles(prolog, 5);
        if ( prolog == 0x2361E )
          level += 1;

        printf(" %.5lX", addr);
        if (prolog == DOCOL )
          {
            level += 1;
            addr +=5 ;
            printf("->%.5lX : Start #%.2i      :",addr ,level);
            for (j = 1; j < level ; j++)
              printf("   ");
            printf("::\n");
          }
        else if ( prolog == SEMI )
          {
            printf("->%.5lX : End of #%.2i     :",addr ,level);
            for (j = 1; j < level ; j++)
              printf("   ");
            printf(";\n");

            addr += 5;
            level -= 1;
          }
        else 
          {
            dec_rpl_obj(&addr, buf);
            printf ("->%.5lX : " , addr);
            if ( prolog_2 == prolog +5 && prolog != SEMI)
              printf("Primitive Code :");
            if ( prolog_2 == DOCODE )
              printf("Code Object    :");
            if ( prolog_2 == DOCOL )
              printf("Program        :");
            if ( prolog == DOROMP )
              {
                xlib1 = read_nibbles(addr-6,3);
                xlib2 = read_nibbles(addr-3,3);
                printf("XLIB %.4i %.3i  :",(int)xlib1, (int)xlib2);
              }
            if ( prolog_2 == DOBINT || prolog == DOBINT )
              printf("Binary Integer :");
            if ( prolog_2 == DOREAL || prolog == DOREAL )
              printf("Real Number    :");
            if ( prolog_2 == DOEREL || prolog == DOEREL )
              printf("Long Real      :");
            if ( prolog_2 == DOCMP || prolog == DOCMP )
              printf("Complex        :");
            if ( prolog_2 == DOECMP || prolog == DOECMP )
              printf("Long Complex   :");
            if ( prolog_2 == DOCHAR || prolog == DOCHAR )
              printf("Character      :");
            if ( prolog_2 == DOARRY || prolog == DOARRY )
              printf("Array          :");
            if ( prolog_2 == DOLNKARRY || prolog == DOLNKARRY )
              printf("Linked Array   :");
            if ( prolog_2 == DOCSTR || prolog == DOCSTR )
              printf("String         :");
            if ( prolog_2 == DOHSTR || prolog == DOHSTR )
              printf("Hex String     :");
            if ( prolog_2 == DOLIST || prolog == DOLIST )
              printf("List           :");
            if ( prolog_2 == DORRP)
              printf("Directory      :");
            if ( prolog_2 == DOSYMB )
              printf("Symbolic       :");
            if ( prolog_2 == DOLAM || prolog == DOLAM )
              printf("Local Ident    :");
            if ( prolog_2 == DOIDNT || prolog == DOIDNT )
              printf("Global Ident   :");

/*
  { "Unit",             0,      DOEXT,          dec_unit },
  { "Tagged",           0,      DOTAG,          skip_ob },
  { "Graphic",          0,      DOGROB,         dec_grob },
  { "Library",          0,      DOLIB,          dec_library },
  { "Backup",           0,      DOBAK,          dec_back },
  { "Library Data",     0,      DOEXT0,         dec_library_data },
*/
            if ( prolog_2 == DOROMP || prolog_2 == DOCOL ||
                 prolog_2 == DOBINT || prolog_2 == DOLIST ||
                 prolog_2 == prolog + 5)
              {
                j = 0;
                while ( j <= saturn.entries_num && 
                        saturn.entries[j].rom != prolog )
                  j++;
                if ( j < saturn.entries_num ) 
                  printf (" %i ", saturn.entries[j].flags); 
                else
                  printf (" N ");
              }
            else 
              printf( "   ");
           
            printf(": %.5lX :", prolog);

            for (j = 1; j < level ; j++)
              printf("   ");

            printf(" %s\n", buf);
          }
        if (prolog == 0x23639 || prolog == 0x235FE )
          level -= 1;

      }
    printf(" %.5lX->%.5lX : End of #%.2i     :     ;\n", addr, addr+5, level);

    }
  }
}
static void
#ifdef __FunctionProto__
do_diso (int argc, char **argv)
#else
do_diso (argc, argv)
     int argc;     char *argv;
#endif
{
  word_20 addr, len ;
  char    buf[65536];


  str_to_upper (argv[1]);
  if (decode_20 (&addr, argv[1]))
  {

    decode_rpl_obj(addr, &len, buf);
       if (strlen(buf) > 63)
         {
           sprintf(&buf[60], "...");
           buf[63] = '\0';
         }
     printf("%.5lX -> %.5lX : %s\n", addr, addr+len , buf);

   }
}

static void
#ifdef __FunctionProto__
do_dis (int argc, char **argv)
#else
do_dis (argc, argv)
     int argc;     char *argv;
#endif
{
  word_20 addr,naddr,val,low_addr;
  int n, j=0, k=0;


  switch (argc)
  { 
    case 1:
        naddr = disassemble (saturn.PC, instr, DIS_INSTR);
        val = read_nibbles(saturn.PC, naddr - saturn.PC);
        printf ("    PC: %.5lX<%lX> -> %s\n", saturn.PC, val, instr);
        break;
    case 2: 
      str_to_upper (argv[1]);
      if (decode_20 (&addr, argv[1]))
      {
        naddr = disassemble (addr, instr, DIS_INSTR);
        val = read_nibbles(addr, naddr - addr);
        printf ("    PC: %.5lX<%lX> -> %s\n", addr, val, instr);
      }
      break;
    case 3:
      str_to_upper (argv[1]);
      addr = -1;
      if (!strncmp(argv[1],"PC",2))
          addr = saturn.PC;
      else if (!decode_20 (&addr, argv[1]))
        return;
      str_to_upper (argv[2]);
      if (decode_dec(&n, argv[2]) )
      {
        if (n <= 0)
          return;

        for ( ; n >= 1 ; n--)
        {
           naddr = disassemble (addr, instr, DIS_INSTR);
           
           low_addr = 0xfffff;
           for ( j = 0; j <= saturn.entries_num; j++ )
             {
               if ( saturn.entries[j].rom - addr <= low_addr - addr &&
                     (  labs(saturn.entries[j].rom - addr) == 
                        saturn.entries[j].rom - addr)  )
                  {
                    low_addr = saturn.entries[j].rom;
                    k = j;
                  }
             }
           if (low_addr == addr )
             {
               printf("*** PC:\t%.5lX -> %s\n", addr, saturn.entries[k].defn);
               printf("***    \t");
               disp_obj(&addr, 30,0);
             }
           else
             {

               printf ("    PC:\t%.5lX -> %s\n", addr, instr);

               if ( low_addr < naddr && low_addr != addr )
                 {
                   printf(
                     "WARNING: Possible misalignment err during disassembly\n");
                   printf("   PC:\t%.5lX -> %s\n",
                      low_addr, saturn.entries[k].defn);
                   disp_obj(&low_addr, 60, 0);
                   n = 0;
                 }
               else
                 addr = naddr;
             }
         }

      }
      break;
  }

}

static void
#ifdef __FunctionProto__
do_delete (int argc, char **argv)
#else
do_delete (argc, argv)
     int argc;
     char *argv;
#endif
{
  int num;

  if (argc == 1)
    {
      for (num = 0; num < MAX_BREAKPOINTS; num++)
	{
	  if (bkpt_tbl[num].addr == saturn.PC)
            {
	      if (bkpt_tbl[num].flags == BP_EXEC)
		{
		  printf ("Breakpoint %d at 0x%.5lX deleted.\n",
			  num + 1, bkpt_tbl[num].addr);
		}
	      else if (bkpt_tbl[num].flags == BP_RANGE)
		{
		  printf ("Range watchpoint %d at 0x%.5lX - 0x%.5lX deleted.\n",
		       num + 1, bkpt_tbl[num].addr, bkpt_tbl[num].end_addr);
		}
	      else if (bkpt_tbl[num].flags)
		{
		  printf ("Watchpoint %d at 0x%.5lX deleted.\n",
			  num + 1, bkpt_tbl[num].addr);
		}
	      num_bkpts--;
	      bkpt_tbl[num].addr = 0;
	      bkpt_tbl[num].flags = 0;
            }
	}
    }
  else
    {
      str_to_upper (argv[1]);
      if (!strcmp ("ALL", argv[1]))
	{
	  for (num = 0; num < MAX_BREAKPOINTS; num++)
	    {
	      bkpt_tbl[num].addr = 0;
	      bkpt_tbl[num].flags = 0;
	    }
	  num_bkpts = 0;
	  printf ("All breakpoints deleted.\n");
	}
      else
	{
	  if (decode_dec (&num, argv[1]))
	    {
	      if (num < 1 || num > MAX_BREAKPOINTS)
		{
		  printf ("Breakpoint %d out of range.\n", num);
		  return;
		}
	      num -= 1;
	      if (bkpt_tbl[num].flags == BP_EXEC)
		{
		  printf ("Breakpoint %d at 0x%.5lX deleted.\n",
			  num + 1, bkpt_tbl[num].addr);
		}
	      else if (bkpt_tbl[num].flags == BP_RANGE)
		{
		  printf ("Range watchpoint %d at 0x%.5lX - 0x%.5lX deleted.\n",
		       num + 1, bkpt_tbl[num].addr, bkpt_tbl[num].end_addr);
		}
	      else if (bkpt_tbl[num].flags)
		{
		  printf ("Watchpoint %d at 0x%.5lX deleted.\n",
			  num + 1, bkpt_tbl[num].addr);
		}
	      num_bkpts--;
	      bkpt_tbl[num].addr = 0;
	      bkpt_tbl[num].flags = 0;
	    }
	}
    }
}

static void
#ifdef __FunctionProto__
do_exit (int argc, char **argv)
#else
do_exit (argc, argv)
     int argc;
     char *argv;
#endif
{
  if (confirm ("Exit the emulator WITHOUT saving its state?"))
    {
      printf ("Exit.\n");
      XCloseDisplay(dpy);
      exit (0);
    }
}

static void
#ifdef __FunctionProto__
do_find (int argc, char **argv)
#else
do_find (argc, argv)
     int argc;
     char *argv;
#endif
{
  word_32 n, addr;
 
  if (argc == 3 )
  {
    str_to_upper (argv[1]);
    str_to_upper (argv[2]);
    if (decode_32 (&addr, argv[1]) && decode_32(&n, argv[2]) )
      {
         if (n >= 0 && addr >=0 )
           {
              while ( n != read_nibbles(addr, 5) && addr < 0xffffe )
                {
                 addr++;
                }
           }
      }
    printf ("finished at %.5lX\n", addr);
   }
}
static void
#ifdef __FunctionProto__
do_go (int argc, char **argv)
#else
do_go (argc, argv)
     int argc;
     char *argv;
#endif
{
  word_20 addr;
  
  if (argc == 2 )
  {
    str_to_upper (argv[1]);
    if (decode_20 (&addr, argv[1]))
      {
        saturn.PC = addr;
        enter_debugger &= ~ILLEGAL_INSTRUCTION;
      }
  }
}

static void
#ifdef __FunctionProto__
do_help (int argc, char **argv)
#else
do_help (argc, argv)
     int argc;
     char *argv;
#endif
{
  int i;

  for (i = 0; cmd_tbl[i].name; i++)
    {
      if (cmd_tbl[i].help)
	{
	  printf ("%s.\n", cmd_tbl[i].help);
	}
    }
}


static void
#ifdef __FunctionProto__
do_lcd (int argc, char **argv)
#else
do_lcd (argc, argv)
     int argc;
     char *argv;
#endif
{
  int len;

  if(argc == 1)
    printf("%s\n",saturn.lwd);
  if(argc == 2)
    {
      len=strlen(argv[1]);
      strcpy(saturn.lwd, argv[1]);
      saturn.lwd[len]='\0';
      strcat(saturn.lwd, "/");
    }
}

static void
#ifdef __FunctionProto__
do_ls (int argc, char **argv)
#else
do_ls (argc, argv)
     int argc;
     char *argv;
#endif
{
  word_20 addr,len, start;
  int     leave, home, i;
  unsigned char c;

  if (argc == 1)
    addr = read_nibbles(0x7059C, 5);
  else if (argc == 2)
    {
      str_to_upper (argv[1]);
      if (!decode_20 (&addr, argv[1]))
        return;
    }
  else 
    return;
 
  if (addr == read_nibbles(0x70592,5))
    home = 1;
  else
    home =0;

  if ( read_nibbles(addr, 5) == DORRP )
    {
      printf(" Directory at %.5lX\n", addr); /* DORRP */
      addr += 5;

      start = read_nibbles(addr,3);
      addr += 3;
      if ( home )
        {
          for ( ; start >= 1 ; start-- )
            {
              printf( "  Library attached: %.4i", (int) read_nibbles(addr,3));
              printf("  Hash:%.5lX   Mess:%.5lX\n", 
                  read_nibbles(addr+3,5), read_nibbles(addr+8,5));
              addr+=13;
            }
        }
      else
        {
          if ( start != 2047 )
           {
              printf("  Library Attached: %.4i", (int)start); /* ROMPARTID */
              printf("  %.5lX   %.5lX\n", 
                  read_nibbles(addr+3,5), read_nibbles(addr+8,5));
           }
          else
            printf(" No Libraries attached to this directory\n");
        }
      if ( read_nibbles(addr,5) == 0 )
        printf(" Empty directory\n");
      else
        {
          addr += read_nibbles(addr,5);
          start = addr-5;
          leave = 0;
          while (! leave ) 
            {
              len = read_nibbles(addr,2);
              addr+= 2;
              if (len !=0 )
               {
                 printf("  ");
                 i=len -1;
                 for ( ; len >= 1; len -- )
                   {
                      c = read_nibbles(addr,2) ;
                      if (hp48_trans_tbl[c].trans)
                        printf("%s", hp48_trans_tbl[c].trans);
                      else
                        printf ("%c",(int)read_nibbles(addr, 2));
                      addr +=2;
                   }
                 for (; i <=10 ; i ++)
                    printf(" ");
                 addr +=2;
               }
              else
                printf("  NULL        " );
/*              printf(" : @ %.5lX\n ", addr); */
              len = addr;
              disp_obj (&len, 35, 0);
    
              if ( read_nibbles(start,5) == 0)
                leave = 1;
              else
                {
                  addr = start - read_nibbles(start, 5);
                  start = addr - 5;
                }

            } 

        }
      }
  else
    printf (" No directory structure at %.5lX\n", addr);
}

static void
#ifdef __FunctionProto__
do_list (int argc, char **argv)
#else
do_list (argc, argv)
     int argc;
     char *argv;
#endif
{
  word_20 addr ;
  unsigned int n;
  
  if (argc == 2 )
  {
    str_to_upper (argv[1]);
    if (decode_20 (&addr, argv[1]))
      {
         printf(" %.5lX : %s\n", addr,  str_nibbles(addr, 16));
      }
  }
  if (argc == 3 )
  {
    str_to_upper (argv[1]);
    str_to_upper (argv[2]);

    if ( decode_20(&addr, argv[1] ) && decode_dec(&n, argv[2] ) )
      {
         if (n <= 0)
           return;

         for ( ; n >0 ; n--)
           {
             printf(" %.5lX : %s\n", addr,  str_nibbles(addr, 16));
             addr += 16;
           }
      }
  }

return;


}

static void
#ifdef __FunctionProto__
do_load (int argc, char **argv)
#else
do_load (argc, argv)
     int argc;
     char *argv;
#endif
{
  saturn_t tmp_saturn;
  device_t tmp_device;

  if (confirm ("Load emulator-state from files?"))
    {
      memcpy (&tmp_saturn, &saturn, sizeof (saturn));
      memcpy (&tmp_device, &device, sizeof (device));
      memset (&saturn, 0, sizeof (saturn));
      if (read_files ())
	{
	  printf ("Loading done.\n");
	  enter_debugger &= ~ILLEGAL_INSTRUCTION;
	  if (tmp_saturn.rom)
	    {
	      free (tmp_saturn.rom);
	    }
	  if (tmp_saturn.ram)
	    {
	      free (tmp_saturn.ram);
	    }
	  if (tmp_saturn.port1)
	    {
	      free (tmp_saturn.port1);
	    }
	  if (tmp_saturn.port2)
	    {
	      free (tmp_saturn.port2);
	    }
	  init_display ();
	  update_display ();
          update_gui();
#ifdef HAVE_XSHM
	  if (disp.display_update)
	    refresh_display ();
#endif
	}
      else
	{
	  printf ("Loading emulator-state from files failed.\n");
	  if (saturn.rom)
	    {
	      free (saturn.rom);
	    }
	  if (saturn.ram)
	    {
	      free (saturn.ram);
	    }
	  if (saturn.port1)
	    {
	      free (saturn.port1);
	    }
	  if (saturn.port2)
	    {
	      free (saturn.port2);
	    }
	  memcpy (&saturn, &tmp_saturn, sizeof (saturn));
	  memcpy (&device, &tmp_device, sizeof (device));
	}
    }
}

static void
#ifdef __FunctionProto__
do_mode (int argc, char **argv)
#else
do_mode (argc, argv)
     int argc;
     char *argv;
#endif
{
  if (argc < 2)
    {
      printf ("Disassembler uses %s mnemonics.\n", mode_name[disassembler_mode]);
      printf ("File transfer is %s\n", send_name[send_mode]);
    }
  else
    {
      str_to_upper (argv[1]);
      if (!strcmp ("HP", argv[1]))
	{
	  disassembler_mode = HP_MNEMONICS;
	}
      else if (!strcmp ("CLASS", argv[1]))
	{
	  disassembler_mode = CLASS_MNEMONICS;
	}
      else if (!strcmp ("SYSRPL", argv[1]))
	{
          disassembler_mode = SYSRPL_MNEMONICS;
	}
      else if (!strcmp ("BIN", argv[1]))
       {
          send_mode = BIN;
       }
      else if (!strcmp ("ASCII", argv[1]))
       {
          send_mode = ASCII;
       }
      else
	{
	  printf ("Unknown disassembler mode %s. Try \"help\".\n", argv[1]);
	}
    }
}

static void
#ifdef __FunctionProto__
do_quit (int argc, char **argv)
#else
do_quit (argc, argv)
     int argc;
     char *argv;
#endif
{
  if (confirm ("Quit the emulator and save its state?"))
    {
      printf ("Exit.\n");
      exit_emulator ();
      XCloseDisplay(dpy);
      exit (0);
    }
}

static void
#ifdef __FunctionProto__
set_reg (word_64 val, int n, unsigned char *r)
#else
set_reg (val, n, r)
     word_64 val;
     int n;
     unsigned char *r;
#endif
{
  int i;

  for (i = 0; i < n; i++)
    {
      if (i < 8)
	r[i] = (unsigned char) ((val.lo & (0xf << (4 * i))) >> (4 * i));
      else
	r[i] = (unsigned char) ((val.hi & (0xf << (4 * (i - 8)))) >> (4 * (i - 8)));
    }
}

void
#ifdef __FunctionProto__
dump_reg (const char *reg, int n, unsigned char *r, char *out)
#else
dump_reg (reg, n, r, out)
     const char *reg;
     int n;
     unsigned char *r;
     char *out;
#endif
{
  int i, j;
  char *p = out , name[20];

  strcpy (name, reg);
  p = append_str(out, name);
  for (i = n - 1; i >= 0; i--)
    {
      j = strlen(out);
      sprintf (&out[j],"%.1X", r[i] & 0xf);
/*       if ( ((float)i )/5 == i/5)
        sprintf(&out[j+1]," "); */
    }
}


static void
#ifdef __FunctionProto__
set_st (word_64 val)
#else
set_st (val)
     word_64 val;
#endif
{
  int i;

  for (i = 0; i < 16; i++)
    saturn.PSTAT[i] = (val.lo & (1 << i)) ? 1 : 0;
}

static void
#ifdef __FunctionProto__
dump_st (void)
#else
dump_st ()
#endif
{
  int i;
  int val;

  val = 0;
  for (i = NR_PSTAT - 1; i >= 0; i--)
    {
      val <<= 1;
      val |= saturn.PSTAT[i] ? 1 : 0;
    }
  printf ("    ST:\t%.4X (", val);
  for (i = NR_PSTAT - 1; i > 0; i--)
    {
      if (saturn.PSTAT[i])
	{
	  printf ("%.1X ", i);
	}
      else
	{
	  printf ("- ");
	}
    }
  if (saturn.PSTAT[0])
    {
      printf ("%.1X)\n", 0);
    }
  else
    {
      printf ("-)\n");
    }
}

static void
#ifdef __FunctionProto__
set_hst (word_64 val)
#else
set_hst (val)
     word_64 val;
#endif
{
  saturn.XM = 0;
  saturn.SB = 0;
  saturn.SR = 0;
  saturn.MP = 0;
  if (val.lo & 1)
    saturn.XM = 1;
  if (val.lo & 2)
    saturn.SB = 1;
  if (val.lo & 4)
    saturn.SR = 1;
  if (val.lo & 8)
    saturn.MP = 1;
}

static void
#ifdef __FunctionProto__
dump_hst (void)
#else
dump_hst ()
#endif
{
  short hst = 0;
  if (saturn.XM != 0)
    hst |= 1;
  if (saturn.SB != 0)
    hst |= 2;
  if (saturn.SR != 0)
    hst |= 3;
  if (saturn.MP != 0)
    hst |= 4;
  printf ("   HST:\t%.1X    (%s%s%s%s)\n", hst,
          saturn.MP ? "MP " : "-- ", saturn.SR ? "SR " : "-- ",
          saturn.SB ? "SB " : "-- ", saturn.XM ? "XM" : "--");
}

static char *mctl_str_gx[] = {
  "MMIO       ", 
  "SysRAM     ",
  "Bank Switch",
  "Port 1     ",
  "Port 2     ",
  "SysROM     "
};

static char *mctl_str_sx[] = {
  "MMIO  ", 
  "SysRAM",
  "Port 1",
  "Port 2",
  "Extra ",
  "SysROM"
};

static void
#ifdef __FunctionProto__
do_lib (int argc, char **argv)
#else
do_lib (argc, argv)
     int argc;
     char *argv;
#endif
{
  word_20        addr, start, temp, libnum, offset;
  word_20        libsize, libidsize, names, hash, linkoffset;
  int            i, j, k;
  unsigned char c;

  addr = 0;
  if (argc == 2)
    {
      str_to_upper (argv[1]);
      if (!decode_20 (&addr, argv[1]))
        return;
      if (read_nibbles(addr, 5) != DOLIB)
        {
          if (decode_dec(&i, argv[1]))
            addr = where_lib(i);
          else
            addr = 0;
        }

      if ( addr !=0 )
        {
          start = addr;
          disp_obj(&addr, 50, 0);
          addr = start + 5;
          
          libsize = read_nibbles(addr, 5);
          libidsize = read_nibbles(addr + 5, 2);
          if (libidsize !=0 )
            addr = start + 2 * libidsize + 17;
          else
            addr = start + 15;
          libnum = read_nibbles(addr - 3, 3);

          temp = addr + read_nibbles(addr,5);
          printf("hash = %.5lX\n", temp   );

          hash = temp ;
          names = read_nibbles(temp+ (5 * 18), 5) + temp + (5*18) ;
          printf("names = %.5lx\n", names);

          temp = addr + read_nibbles(addr + 5,5) +5;
          printf("mesg = %.5lX\n", temp );
          
          temp =  addr + read_nibbles(addr + 10, 5) +10;
          printf("addr = %.5lx; link = %.5lX\n", addr,  temp);
          k = (int) (read_nibbles(temp + 5, 5) / 5);
          temp += 10;

          for ( i = 0 ;  i <= k ; i++)
            {
              printf("  %.2i  ",i);
              offset = read_nibbles(temp,5);
              if ( offset & 0x80000)
                offset = offset - 0xFFFFF - 1 ;
              if ( read_nibbles(offset+temp -6,3) == libnum )
                {
                   hash = names - read_nibbles(names,5);
                   j = (int) read_nibbles(hash, 2);
                   hash += 2;
                   for ( ; j >=1 ; j--)
                     {
                        c = read_nibbles(hash,2) ;
                        if (hp48_trans_tbl[c].trans)
                          printf("%s", hp48_trans_tbl[c].trans);
                        else
                          printf ("%c",(int)c );
                        hash +=2;
                     }
             
                }
              printf(" : ");
              linkoffset = temp+offset;
              disp_obj(&linkoffset, 40,0);
              names += 5;
              temp +=5;
            }

          temp = addr + read_nibbles(addr + 15, 5) +15;          
          printf("Conf = %.5lX\n  ", temp );
          disp_obj(&temp, 50 ,0);

          addr += 20;
          printf("%.5lX\n", addr);
        }
      else
        printf("No Library by that number or at that address\n");
    }


}

static void
#ifdef __FunctionProto__
do_ram (int argc, char **argv)
#else
do_ram (argc, argv)
     int argc;
     char *argv;
#endif
{
  int i;
  word_20 home, end_home; 

  home = 0x70713;
  printf("Stack  ");
  disp_obj(&home, 40, 0);

  home = read_nibbles(0x70551, 5);
  printf("Menu   ");
  disp_obj(&home, 40, 0);

  home = read_nibbles(0x7055B, 5);
  printf("Current :  ");
  disp_obj(&home, 40, 0);

  home = read_nibbles(0x70556, 5);
  printf("textgrob:  ");
  disp_obj(&home, 40, 0);

  home = read_nibbles(0x70565, 5);
  printf("graphgrob: ");
  disp_obj(&home, 40, 0);
  
  home     = read_nibbles(0x7057e,5);
  end_home = read_nibbles(0x70579,5);
  printf("%.5lX <-> %.5lX : Stack\n",home, end_home);

  home     = read_nibbles(0x70583,5);
  printf("%.5lX : Local vars\n", home);

  home     = read_nibbles(0x70588,5);
  printf("%.5lX : internal loops\n",home);

  home     = read_nibbles(0x7058D,5);
  printf("%.5lX : menu Keys\n",home);

  home = read_nibbles(0x70592,5);
  end_home = read_nibbles(0x70597,5);
  printf("%.5lX -> %.5lX : Home Directory\n",home, end_home);

  home = end_home;
  while(read_nibbles(home,5) !=0)
    disp_obj(&home,40,0);
  printf("\n");


  

  for (i = 0; i < 5; i++)
    {
      printf("%s ", opt_gx ? mctl_str_gx[i] : mctl_str_sx[i]);
      if (saturn.mem_cntl[i].unconfigured)
        printf("unconfigured\n");
      else
        if (i == 0)
          printf("configured to 0x%.5lx\n", saturn.mem_cntl[i].config[0]);
        else
          {
            printf("configured to 0x%.5lX - 0x%.5lX\n",
                 saturn.mem_cntl[i].config[0],
                 (saturn.mem_cntl[i].config[0] | ~saturn.mem_cntl[i].config[1])
                 & 0xfffff);
          }
    }
  if (opt_gx)
    printf("Port 2      switched to bank %d\n", saturn.bank_switch);

}

static void
#ifdef __FunctionProto__
do_regs (int argc, char **argv)
#else
do_regs (argc, argv)
     int argc;
     char *argv;
#endif
{
  int i;
  word_64 val;
  char reg[100];

  if (argc < 2)
    {
      /*
     * dump all registers
     */
      printf ("CPU is in %s mode. Registers:\n",
	      saturn.hexmode == HEX ? "HEX" : "DEC");
      dump_reg ("A : ", 16, saturn.A, reg);
      printf ("    %s\n", reg);
      dump_reg ("B : ", 16, saturn.B, reg);
      printf("    %s\n", reg);
      dump_reg ("C : ", 16, saturn.C, reg);
      printf("    %s\n", reg);
      dump_reg ("D : ", 16, saturn.D, reg);
      printf("    %s\n",reg);
      printf ("    D0:\t%.5lX ->", saturn.D0);
      for (i = 0; i < 20; i += 5)
	{
	  printf (" %s", str_nibbles (saturn.D0 + i, 5));
	}
      printf ("\n");
      printf ("    D1:\t%.5lX ->", saturn.D1);
      for (i = 0; i < 20; i += 5)
	{
	  printf (" %s", str_nibbles (saturn.D1 + i, 5));
	}
      printf ("\n");
      printf ("     P:\t%.1X\n", saturn.P);
      disassemble (saturn.PC, instr, DIS_INSTR);
      printf ("    PC:\t%.5lX -> %s\n", saturn.PC, instr);
      dump_reg ("R0: ", 16, saturn.R0,reg);
      printf("    %s\n",reg);
      dump_reg ("R1: ", 16, saturn.R1,reg);
      printf("    %s\n",reg);
      dump_reg ("R2: ", 16, saturn.R2,reg);
      printf("    %s\n",reg);
      dump_reg ("R3: ", 16, saturn.R3,reg);
      printf("    %s\n",reg);
      dump_reg ("R4: ", 16, saturn.R4,reg);
      printf("    %s\n",reg);
      dump_reg ("IN: ", 4, saturn.IN, reg);
      printf("    %s\n",reg);
      dump_reg ("OUT: ", 3, saturn.OUT,reg);
      printf("   %s\n",reg);
      printf (" CARRY:\t%.1d\n", saturn.CARRY);
      dump_st ();
      dump_hst ();
    }
  else if (argc == 2)
    {
      /*
     * dump specified register
     */
      str_to_upper (argv[1]);
      if (!strcmp ("A", argv[1]))
	{
          dump_reg ("   A: ", 16, saturn.A, reg);
          printf ("    %s\n", reg);
        }
      else if (!strcmp ("B", argv[1]))
        {
          dump_reg ("   B: ", 16, saturn.B, reg);
          printf ("    %s\n", reg);
        }
      else if (!strcmp ("C", argv[1]))
        {
          dump_reg ("   C: ", 16, saturn.C, reg);
          printf ("    %s\n", reg);
        }
      else if (!strcmp ("D", argv[1]))
        {
          dump_reg ("   D: ", 16, saturn.D, reg);
          printf ("    %s\n", reg);
	}
      else if (!strcmp ("D0", argv[1]))
	{
	  printf ("    D0:\t%.5lX ->", saturn.D0);
	  for (i = 0; i < 20; i += 5)
	    {
	      printf (" %s", str_nibbles (saturn.D0 + i, 5));
	    }
	  printf ("\n");
	}
      else if (!strcmp ("D1", argv[1]))
	{
	  printf ("    D1:\t%.5lX ->", saturn.D1);
	  for (i = 0; i < 20; i += 5)
	    {
	      printf (" %s", str_nibbles (saturn.D1 + i, 5));
	    }
	  printf ("\n");
	}
      else if (!strcmp ("P", argv[1]))
	{
	  printf ("     P:\t%.1X\n", saturn.P);
	}
      else if (!strcmp ("PC", argv[1]))
	{
	  disassemble (saturn.PC, instr,DIS_INSTR);
	  printf ("    PC:\t%.5lX -> %s\n", saturn.PC, instr);
	}
      else if (!strcmp ("R0", argv[1]))
	{
          dump_reg ("    R0", 16, saturn.R0, reg);
          printf ("    %s\n", reg);
        }
      else if (!strcmp ("R1", argv[1]))
        {
          dump_reg ("    R1", 16, saturn.R1,reg);
          printf ("    %s\n", reg);
        }
      else if (!strcmp ("R2", argv[1]))
        {
          dump_reg ("    R2", 16, saturn.R2, reg);
          printf ("    %s\n", reg);
        }
      else if (!strcmp ("R3", argv[1]))
        {
          dump_reg ("    R3", 16, saturn.R3, reg);
          printf ("    %s\n", reg);
        }
      else if (!strcmp ("R4", argv[1]))
        {
          dump_reg ("    R4", 16, saturn.R4, reg);
          printf ("    %s\n", reg);
        }
      else if (!strcmp ("IN", argv[1]))
        {
          dump_reg ("    IN", 4, saturn.IN, reg);
          printf ("    %s\n", reg);
        }
      else if (!strcmp ("OUT", argv[1]))
        {
          dump_reg ("   OUT", 3, saturn.OUT, reg);
          printf ("    %s\n", reg);
	}
      else if (!strcmp ("CARRY", argv[1]))
	{
	  printf (" CARRY:\t%.1d\n", saturn.CARRY);
	}
      else if (!strcmp ("CY", argv[1]))
	{
	  printf (" CARRY:\t%.1d\n", saturn.CARRY);
	}
      else if (!strcmp ("ST", argv[1]))
	{
	  dump_st ();
	}
      else if (!strcmp ("HST", argv[1]))
	{
	  dump_hst ();
	}
      else
	{
	  printf ("No Register %s in CPU.\n", argv[1]);
	}
    }
  else
    {
      /*
     * set specified register
     */
      str_to_upper (argv[1]);
      str_to_upper (argv[2]);
      if (decode_64 (&val, argv[2]))
	{
	  if (!strcmp ("A", argv[1]))
	    {
              set_reg (val, 16, saturn.A);
              dump_reg ("     A", 16, saturn.A, reg);
              printf ("    %s\n", reg);
            }
          else if (!strcmp ("B", argv[1]))
            {
              set_reg (val, 16, saturn.B);
              dump_reg ("     B", 16, saturn.B, reg);
              printf ("    %s\n", reg);
            }
          else if (!strcmp ("C", argv[1]))
            {
              set_reg (val, 16, saturn.C);
              dump_reg ("     C", 16, saturn.C, reg);
              printf ("    %s\n", reg);
            }
          else if (!strcmp ("D", argv[1]))
            {
              set_reg (val, 16, saturn.D);
              dump_reg ("     D", 16, saturn.D, reg);
              printf ("    %s\n", reg);
	    }
	  else if (!strcmp ("D0", argv[1]))
	    {
	      saturn.D0 = (word_20)(val.lo & 0xfffff);
	      printf ("    D0:\t%.5lX ->", saturn.D0);
	      for (i = 0; i < 20; i += 5)
		{
		  printf (" %s", str_nibbles (saturn.D0 + i, 5));
		}
	      printf ("\n");
	    }
	  else if (!strcmp ("D1", argv[1]))
	    {
	      saturn.D1 = (word_20)(val.lo & 0xfffff);
	      printf ("    D1:\t%.5lX ->", saturn.D1);
	      for (i = 0; i < 20; i += 5)
		{
		  printf (" %s", str_nibbles (saturn.D1 + i, 5));
		}
	      printf ("\n");
	    }
	  else if (!strcmp ("P", argv[1]))
	    {
	      saturn.P = (word_4)(val.lo & 0xf);
	      printf ("     P:\t%.1X\n", saturn.P);
	    }
	  else if (!strcmp ("PC", argv[1]))
	    {
	      saturn.PC = (word_20)(val.lo & 0xfffff);
	      disassemble (saturn.PC, instr, DIS_INSTR);
	      printf ("    PC:\t%.5lX -> %s\n", saturn.PC, instr);
	    }
	  else if (!strcmp ("R0", argv[1]))
	    {
              set_reg (val, 16, saturn.R0);
              dump_reg ("    R0", 16, saturn.R0, reg);
              printf ("    %s\n", reg);
            }
          else if (!strcmp ("R1", argv[1]))
            {
              set_reg (val, 16, saturn.R1);
              dump_reg ("    R1", 16, saturn.R1, reg);
              printf ("    %s\n", reg);
            }
          else if (!strcmp ("R2", argv[1]))
            {
              set_reg (val, 16, saturn.R2);
              dump_reg ("    R2", 16, saturn.R2, reg);
              printf ("    %s\n", reg);
            }
          else if (!strcmp ("R3", argv[1]))
            {
              set_reg (val, 16, saturn.R3);
              dump_reg ("    R3", 16, saturn.R3,reg);
              printf ("    %s\n", reg);
            }
          else if (!strcmp ("R4", argv[1]))
            {
              set_reg (val, 16, saturn.R4);
              dump_reg ("    R4", 16, saturn.R4, reg);
              printf ("    %s\n", reg);
            }
          else if (!strcmp ("IN", argv[1]))
            {
              set_reg (val, 4, saturn.IN);
              dump_reg ("    IN", 4, saturn.IN,reg);
              printf ("    %s\n", reg);
            }
          else if (!strcmp ("OUT", argv[1]))
            {
              set_reg (val, 3, saturn.OUT);
              dump_reg ("   OUT", 3, saturn.OUT,reg);
              printf ("    %s\n", reg);
	    }
	  else if (!strcmp ("CARRY", argv[1]))
	    {
	      saturn.CARRY = (word_1)(val.lo & 0x1);
	      printf (" CARRY:\t%.1d\n", saturn.CARRY);
	    }
	  else if (!strcmp ("CY", argv[1]))
	    {
	      saturn.CARRY = (word_1)(val.lo & 0x1);
	      printf (" CARRY:\t%.1d\n", saturn.CARRY);
	    }
	  else if (!strcmp ("ST", argv[1]))
	    {
	      set_st (val);
	      dump_st ();
	    }
	  else if (!strcmp ("HST", argv[1]))
	    {
	      set_hst (val);
	      dump_hst ();
	    }
	  else
	    {
	      printf ("No Register %s in CPU.\n", argv[1]);
	    }
	}
    }
}

static void
#ifdef __FunctionProto__
do_receive (int argc, char **argv)
#else
do_receive (argc, argv)
     int argc;
     char *argv;
#endif
{
   struct stat st;
   FILE *fp;
   DIR  *directory;
   struct dirent *nth_file;
   unsigned char in[16] ;
   char path[1024];
   word_20 k, crc=0, addr=0xC0000, len, start=0xC0000;
   int i, leave, dir ;
   /* int j; */

  directory = (DIR *)0;
  nth_file = (struct dirent *)0;

  if (argc == 2 )
    {
       if (!strcmp(argv[1],"*"))
         {
           dir = 1;
           strcpy(path, saturn.lwd);
           if (NULL== (directory = opendir(path)) )
             {
                fprintf(stderr, "can\'t open directory %s\n", path);
                fprintf(stderr, "%s\n", strerror(errno));
                return ;
             }
         }
       else
         dir = 0;

       leave =0;
       while(!leave)
         {
           if (!dir)
             {
                strcpy(path, saturn.lwd);
                strcat(path, argv[1]);
                leave = 1;
             }
           else
             {
                if (NULL == (nth_file = readdir(directory)) )
                  return;
                strcpy(path, saturn.lwd);
                strcat(path, nth_file->d_name);
             }

           if ( NULL == (fp = fopen(path, "r")))
             {
                fprintf(stderr, "can\'t open file %s\n", path);
                fprintf(stderr, "%s\n", strerror(errno));
                if (leave)
                  return ;
                else
                  continue;
             }

           if (stat(path, &st) < 0)
             {
                fprintf(stderr, "can\'t stat \n");
                fclose(fp);
                if (leave)
                  return ;
                else
                  continue;
             }

           if (fread(in, 1, 6, fp) != 6)
             {
                fclose(fp);
                if (leave)
                  {
                    fprintf(stderr, "can\'t read first 6 bytes of %s\n",path);
                    return;
                  }
                else
                  continue;
             }

           if( strncmp(in,"HPHP48",6) )
             {
                fclose(fp);
                if (leave)
                  {
                    fprintf(stderr, "%s is not a HP file\n",path);
                    return ;
                  }
                else
                  continue;
             }
           fseek(fp, 2, SEEK_CUR);
     
           if (!dir)
             {
               addr = read_nibbles(start,5);
               if (addr != 0)
                 {
                    while (( addr = read_nibbles(start,5)) != 0)
                      {
                         printf("Skipping ");
                         disp_obj (&start, 50,0);
                      }
                 }
             addr = start;
             }
           write_nibbles(addr, 0x02b62 , 5); /* DOBACKUP */
           addr +=5;
           write_nibbles(addr, 0x0, 5);  /* len */
           addr +=5; 

           if (!dir)
             k = strlen(argv[1]);
           else
             k = strlen(nth_file->d_name);
           write_nibbles(addr, k , 2); /* len of name */
           addr +=2;
 
           for (i=0; i < k; i++)
             {
                if (!dir)
                  write_nibbles(addr, argv[1][i], 2);
                else
                  write_nibbles(addr, nth_file->d_name[i], 2);
                addr+=2;
             }

           write_nibbles(addr, k , 2); /* len of name */
           addr +=2;
 
           in[1] = 0;
           k = 0;
           while(fread(&in, 1, 1, fp))
             {

            /* i=(int)in[0] & 0xf0;
               i >>= 4;
               j = (int)in[0] & 0x0f;
               j <<= 4;
               printf( "%.2x", i+j ); */

               write_nibbles(addr, in[0] , 2);
               addr+= 2;
               k++;
             }
           fclose(fp);
     
           len = addr - start +5 ;
           write_nibbles(start+5, len, 5);
           write_nibbles(addr, 0x02911, 5); /* Prologue for BINT */
           addr+=5;

           addr+=1;
           start += 5;
           crc=0;
           for ( ; start < addr ; start++)
             crc = (crc >> 4) ^ (((crc ^ read_nibble(start)) & 0xf) * 0x1081);

           write_nibbles(addr, crc, 5);
           printf (" Recieved %s : 0x%lxh nibbles, 0x%lxh CRC\n",
                           path, len, crc);
           addr+=4;
           start=addr;

         }
      write_nibbles(addr, 0x00000, 5);
    }
}

static void
#ifdef __FunctionProto__
do_send (int argc, char **argv)
#else
do_send (argc, argv)
     int argc;
     char *argv;
#endif
{
   word_20 addr, length , i;
   struct stat st;
   FILE *fp;
   char byte[1], path[1024], buf[65535];
  
  if (argc >= 2 )
  {
    if ((addr=where( argv[1])) == 0 )
      { 
        str_to_upper( argv[1]);
        if( !decode_20(&addr, argv[1]))
          addr = 0;
      }
   }

  strcpy(path, saturn.lwd);

  if (argc == 2)
     strcat(path, argv[1]);
  else if (argc == 3)
     strcat(path, argv[2]);
  
  if ( addr !=0 )
    {
      if ( NULL == (fp = fopen(path, "w")))
        { 
          fprintf(stderr, "can\'t open file : %s\n",path);
          fprintf(stderr, "%s\n", strerror(errno));

          return ;
        }
      if (stat(path, &st) < 0)
        {
           fprintf(stderr, "can\'t stat file : %s\n",path);
           fclose(fp);
           return ;
        }
      i = addr;
      printf(" Sending : ");
      disp_obj(&i, 50, 0);
      length = i - addr;

      if (send_mode == BIN)
      {
        fwrite("HPHP48-E",1, 8, fp);
        for( i =0 ; i <= length ; i+=2)
          {
            if( (length - i) == 1 )
              byte[0] = (int) read_nibbles(addr,1);
            else
              byte[0] = (int)read_nibbles(addr,2);
            fwrite( byte, 1, 1, fp);
            addr += 2;
          }
       }
       if (send_mode == ASCII)
       {
        decode_rpl_obj(addr, &length, buf);
        fwrite(buf, 1, strlen(buf), fp);
       }
      fclose(fp);
      printf(" Wrote file : %s, 0x%lXh nibbles\n", path, length);
    }
}

static void
#ifdef __FunctionProto__
do_save (int argc, char **argv)
#else
do_save (argc, argv)
     int argc;
     char *argv;
#endif
{
  if (write_files ())
    {
      printf ("Saving done.\n");
    }
  else
    {
      printf ("Saving emulator-state failed.\n");
    }
}

static void
#ifdef __FunctionProto__
do_stack (int argc, char **argv)
#else
do_stack (argc, argv)
     int argc;
     char *argv;
#endif
{
  word_20 dsktop, dskbot, len;
  word_20 sp = 0, end = 0, ent = 0;
  word_20 ram_base, ram_mask;
  char    buf[65536];
  struct se *stack, *se;
  int n;

  ram_base = saturn.mem_cntl[1].config[0];
  ram_mask = saturn.mem_cntl[1].config[1];
  if (opt_gx)
    {
      saturn.mem_cntl[1].config[0] = 0x80000;
      saturn.mem_cntl[1].config[1] = 0xc0000;
      dsktop = DSKTOP_GX;
      dskbot = DSKBOT_GX;
    }
  else
    {
      saturn.mem_cntl[1].config[0] = 0x70000;
      saturn.mem_cntl[1].config[1] = 0xf0000;
      dsktop = DSKTOP_SX;
      dskbot = DSKBOT_SX;
    }

  load_addr(&sp, dsktop, 5);
  load_addr(&end, dskbot, 5);

  stack = (struct se *)0;
  n = 0;
  do
    {
      load_addr(&ent, sp, 5);
      if (ent == 0)
        break;
      n++;
      sp += 5;
      se = (struct se *)malloc(sizeof(struct se));
      if (se == 0)
        {
          fprintf(stderr, "Out off memory.\n");
          break;
        }
      se->se_n = n;
      se->se_p = ent;
      se->se_next = stack;
      stack = se;
    }
  while (sp <= end);

  if (n == 0)
    printf("Empty stack.\n");

  se = stack;
  while (se)
    {
      decode_rpl_obj(se->se_p,&len, buf);
      if (se->se_n != 1)
        if (strlen(buf) > 63)
          {
            sprintf(&buf[60], "...");
            buf[63] = '\0';
          }
      printf("%5d: %.5lX -> %s\n", se->se_n, se->se_p, buf);
      se = se->se_next;
    } 

  se = stack;
  while (se)
    {
      stack = se;
      se = se->se_next;
      free(stack);
    }

  saturn.mem_cntl[1].config[0] = ram_base;
  saturn.mem_cntl[1].config[1] = ram_mask;
}

static void
#ifdef __FunctionProto__
do_stat (int argc, char **argv)
#else
do_stat (argc, argv)
     int argc;
     char *argv;
#endif
{
  printf ("Instructions/s: %ld\n", saturn.i_per_s);
  printf ("Timer 1 I/TICK: %d\n", saturn.t1_tick);
  printf ("Timer 2 I/TICK: %d\n", saturn.t2_tick);
}

static void
#ifdef __FunctionProto__
do_finish (int argc, char **argv)
#else
do_finish (argc, argv)
     int argc;
     char *argv;
#endif
{
  int n;

  if (enter_debugger & ILLEGAL_INSTRUCTION)
    {
      printf ("Can\'t step into an illegal instruction.");
      return;
    }

  n = saturn.rstkp;

  in_debugger = 1;

  enter_debugger = 0;
  while ( n != (saturn.rstkp +1) )
    {
      if (enter_debugger)
        break;

      step_instruction ();

      if (exec_flags & EXEC_BKPT)
        {
          if (check_breakpoint (BP_EXEC, saturn.PC))
            {
              enter_debugger |= BREAKPOINT_HIT;
              break;
            }
        }

      schedule ();
    }
}


static void
#ifdef __FunctionProto__
do_step (int argc, char **argv)
#else
do_step (argc, argv)
     int argc;
     char *argv;
#endif
{
  word_20 next_instr;
  int n;
  int leave;

  if (enter_debugger & ILLEGAL_INSTRUCTION)
    {
      printf ("Can\'t step into an illegal instruction.");
      return;
    }

  n = 1;
  if (argc > 1)
    if (!decode_dec(&n, argv[1]))
      return;

  if (n <= 0)
    return;

  in_debugger = 1;

  step_instruction ();
  next_instr = saturn.PC;

  if (exec_flags & EXEC_BKPT)
    {
      if (check_breakpoint (BP_EXEC, saturn.PC))
	{
	  enter_debugger |= BREAKPOINT_HIT;
	  return;
	}
    }

  sched_adjtime = 0;
  schedule ();

  enter_debugger = 0;
  while ( n >= 1)
    {
      if (enter_debugger)
	break;

      leave = 0;

      if (saturn.PC == next_instr)
	{
	  n--;
	  leave = 1;
	  if (n == 0)
	    break;
	}
    if (check_breakpoint (BP_EXEC, saturn.PC) )
    {
       enter_debugger |= BREAKPOINT_HIT;
       return;
    }


      step_instruction ();

      if (exec_flags & EXEC_BKPT)
	{
	  if (check_breakpoint (BP_EXEC, saturn.PC))
	    {
	      enter_debugger |= BREAKPOINT_HIT;
	      break;
	    }
	}

      if (leave)
	next_instr = saturn.PC;

      schedule ();
    }
}

static void
#ifdef __FunctionProto__
do_trace (int argc, char **argv)
#else
do_trace (argc, argv)
     int argc;
     char *argv;
#endif
{
  word_20 next_instr, naddr, opcode;
  int n;
  int leave ;
  int shutup, pos;

  if (enter_debugger & ILLEGAL_INSTRUCTION) {
    printf ("Can\'t step into an illegal instruction.");
    return;
  }

  n = 1;
  if (argc > 1)
    if (!decode_dec(&n, argv[1]))
      return;

  if (n == 0)
    return;

  if ( n<0 ) {
    leave = 1;
    pos = saturn.old_index;
    while (n != 0) {
      disassemble (saturn.old_PC[pos], instr, DIS_INSTR);
      printf ("%i    PC:\t%.5lX -> %s\n", pos, saturn.old_PC[pos], instr);
      if (pos == 0)
         pos = MAX_TRACE+1 ;
      pos--;
      n++;
    }
  }
  else {
    if (argc == 3) {
      str_to_upper(argv[2]);
      if (!strncmp(argv[2],"QUIET",5))
        shutup = 1;
      else
        shutup = 0;
    }
    else
      shutup = 0;

    in_debugger = 1;

    step_instruction ();

      /*
       * print current instruction
       */

    if(!shutup) {
      naddr = disassemble (saturn.PC, instr, DIS_ALL);
      opcode = read_nibbles(saturn.PC, naddr - saturn.PC);

      if (check_breakpoint (BP_EXEC, saturn.PC))
        printf("BREAK: ");
      if( strncmp(instr, "nope",4) != 0 )
        printf ("%.5lX <%7lX>: %s\n", saturn.PC, opcode, instr);
      else
        n++;
     }

    next_instr = saturn.PC;

    sched_adjtime = 0;
    schedule ();

    enter_debugger = 0;
    while (1) {
      if (enter_debugger)
        break;

      leave = 0;
      if (saturn.PC == next_instr) {
        n--;
        leave = 1;
        if (n == 0)
          break;
      }

      if (check_breakpoint (BP_EXEC, saturn.PC) )
        printf("BREAK: ");

      step_instruction ();

      /*
       * print the last instruction
       */
      if (!shutup) {
        naddr = disassemble (next_instr, instr, DIS_ALL);
        opcode = read_nibbles(next_instr, naddr - next_instr);

        if( strncmp(instr, "nope",4) !=0 )
          printf ("%.5lX <%7lX>: %s\n", next_instr, opcode, instr);
        else
          n++; 
      }

      if (leave)
        next_instr = saturn.PC;
      schedule ();
    }
  }

}
static void
#ifdef __FunctionProto__
do_reset_calc (int argc, char **argv)
#else
do_reset_calc (argc, argv)
     int argc;
     char *argv;
#endif
{
  if (confirm ("Do a RESET (PC = 00000)?"))
    {
      saturn.PC = 0;
      enter_debugger &= ~ILLEGAL_INSTRUCTION;
    }
}

static void
#ifdef __FunctionProto__
do_rstk (int argc, char **argv)
#else
do_rstk (argc, argv)
     int argc;
     char *argv;
#endif
{
  int i, j;

  disassemble (saturn.PC, instr, DIS_INSTR);
  printf ("PC: %.5lX: %s\n", saturn.PC, instr);
  if (saturn.rstkp < 0)
    {
      printf ("Empty return stack.\n");
    }
  else
    {
      j = 0;
      for (i = saturn.rstkp; i >= 0; i--)
	{
	  disassemble (saturn.rstk[i], instr, DIS_INSTR);
	  printf ("%2d: %.5lX: %s\n", j, saturn.rstk[i], instr);
	  j++;
	}
    }
}

int
#ifdef __FunctionProto__
debug (void)
#else
debug ()
#endif
{
  t1_t2_ticks ticks;
  struct cmd *cmdp;
  char *cp;
  int argc;
  char *argv[MAX_ARGS];
  char *rl = NULL;
  static char *cl = (char *) 0;
  static char *old_line = (char *) 0;
  int i;

  /*
   * do we want to debug ???
   */
  if (!useDebugger)
    {
      if (enter_debugger & ILLEGAL_INSTRUCTION)
        {
          if (!quiet)
            fprintf (stderr, "%s: reset (illegal instruction at 0x%.5lX)\n",
                     progname, saturn.PC);
          saturn.PC = 0;
        }
      if (enter_debugger & USER_INTERRUPT)
        if (verbose)
          printf ("%s: user interrupt (SIGINT) ignored\n", progname);
      if (enter_debugger & BREAKPOINT_HIT)
        if (verbose)
          printf ("%s: breakpoint hit at 0x%.5lX ignored\n",
                  progname, saturn.PC);
      if (enter_debugger & TRAP_INSTRUCTION)
        if (verbose)
          printf ("%s: trap instruction at 0x%.5lX ignored\n",
                  progname, saturn.PC);
      enter_debugger = 0;
      return 0;
    }

  /*
   * update the lcd if necessary
   */
  if (device.display_touched)
    {
      device.display_touched = 0;
      update_display ();
      update_gui();
#ifdef HAVE_XSHM
      if (disp.display_update)
	refresh_display ();
#endif
    }

  /*
   * debugging is counted as idle time
   */
  stop_timer (RUN_TIMER);
  start_timer (IDLE_TIMER);

  continue_flag = 0;

  if (enter_debugger & ILLEGAL_INSTRUCTION)
    {
      printf ("ILLEGAL INSTRUCTION at %.5lX : %s\n",
	      saturn.PC, str_nibbles (saturn.PC, 16));
    }

  if (enter_debugger & TRAP_INSTRUCTION)
    {
      printf ("TRAP at %.5lX : %s\n",
	      saturn.PC - 5, str_nibbles (saturn.PC - 5, 16));
      enter_debugger &= ~TRAP_INSTRUCTION;
    }

  do
    {
      update_gui();
      /*
       * print current instruction
       */
/*
      disassemble (saturn.PC, instr, DIS_INSTR);
      printf ("%.5lX: %s\n", saturn.PC, instr);
*/
      /*
       * read a command
       */
#ifdef HAVE_READLINE
      rl = readline ("x48-debug> ");
#else
      if (rl == (char *) 0)
        rl = (char *)malloc((size_t)80);
      printf("x48-debug> ");
      fflush(stdout);
      rl = read_str(rl, 80, 0);
#endif

      if (rl == (char *) 0)
	{
	  continue_flag = 1;
	  continue;
	}
      if (*rl == '\0')
	{
	  free (rl);
          rl = (char *) 0;
	  if (cl)
            {
	      free (cl);
              cl = (char *) 0;
            }
	  if (old_line)
	    cl = strcpy ((char *) malloc (strlen (old_line)), old_line);
	  else
	    cl = strcpy ((char *) malloc (strlen ("(null)")), "(null)");
	}
      else
	{
#ifndef HAVE_READLINE
          if (rl[strlen(rl) - 1] == '\n')
            rl[strlen(rl) - 1] = '\0';
#endif
	  if (cl)
	    {
	      free (cl);
              cl = (char *) 0;
	    }
	  if (old_line)
	    {
	      free (old_line);
              old_line = (char *) 0;
	    }
	  cl = strcpy ((char *) malloc (strlen (rl)), rl);
	  old_line = strcpy ((char *) malloc (strlen (rl)), rl);
#ifdef HAVE_READLINE
	  add_history (rl);
#endif
	  free (rl);
          rl = (char *) 0;
	}

      /*
       * decode the commandline
       */
      cp = strtok (cl, " \t");
      for (cmdp = cmd_tbl; cmdp->name; cmdp++)
	{
	  if (strcmp (cp, cmdp->name) == 0)
	    {
	      break;
	    }
	}

      argc = 0;
      argv[argc++] = cp;
      while ((cp = strtok ((char *) 0, " \t")) != (char *) 0)
	{
	  argv[argc++] = cp;
	  if (argc == MAX_ARGS)
	    break;
	}
      for (i = argc; i < MAX_ARGS; i++)
	argv[i] = (char *) NULL;

      /*
       * execute the command, if valid
       */
      if (cmdp->func)
	{
	  (*cmdp->func) (argc, argv);
	}
      else
	{
	  printf ("Undefined command \"%s\". Try \"help\".\n", argv[0]);
	}
      in_debugger = 0;

    }
  while (!continue_flag);

  /*
   * adjust the hp48's timers
   */
  in_debugger = 1;
  ticks = get_t1_t2 ();
  in_debugger = 0;
  
  if (saturn.t2_ctrl & 0x01)
    {
      saturn.timer2 = ticks.t2_ticks;
    }

  saturn.timer1 = (set_t1 - ticks.t1_ticks) & 0xf;

  sched_adjtime = 0;

  /*
   * restart timers
   */
  stop_timer (IDLE_TIMER);
  start_timer (RUN_TIMER);

  set_accesstime();

  if (enter_debugger & ILLEGAL_INSTRUCTION)
    {
      printf ("Reset (ILLEGAL INSTRUCTION)\n");
      saturn.PC = 0;
    }
  else
    {
      printf ("Continue.\n");
    }

  enter_debugger = 0;

  /*
   * Set exec_flags according to breakpoints, etc.
   */
  exec_flags = 0;
  if (num_bkpts)
    exec_flags |= EXEC_BKPT;

  return 0;
}

int
#ifdef __FunctionProto__
emulate_over ()
#else
emulate_debug ()
#endif
{
  saturn.PC = disassemble (saturn.PC, instr, DIS_ALL);
  return 0;
}

int
#ifdef __FunctionProto__
emulate_debug (int num)
#else
emulate_debug (num)
  int  num;
#endif
{
  word_20  naddr, opcode, step=0;
  long     i=0;
  int      start_trace = 0, k, indent=0, l, leave=0 ;
  char     buf[100], path[1024];
  FILE *fp;
  struct stat st;
 

  strcpy(path, saturn.lwd);
  strcat(path, "trace");

  if (num==0)
    draw_logo(YELLOW);

  do {
    if (start_trace && !Trace) {
      sprintf(buf, "***Stepped %ld instructions\n", i);
      fwrite(buf, 1, strlen(buf), fp);
      sprintf(buf, "***Stopped Trace %s-%d.%d.%d\n", progname,
              saturn.version[0], saturn.version[1], saturn.version[2]);
      fwrite(buf, 1, strlen(buf), fp);

      fclose(fp);
      start_trace = 0;
    }
    if (!start_trace && Trace) {
      start_trace = 1;
      if ( NULL == (fp = fopen(path, "a"))) { 
        fprintf(stderr, "can\'t open file : %s\n",path);
        fprintf(stderr, "%s\n", strerror(errno));
        return 0 ;
      }
      if (stat(path, &st) < 0) {
         fprintf(stderr, "can\'t stat file : %s\n",path);
         fclose(fp);
         return 0 ;
      }
      sprintf(buf, "***Started Trace %s-%d.%d.%d\n", progname,
              saturn.version[0], saturn.version[1], saturn.version[2]);
      fwrite(buf, 1, strlen(buf), fp);
    }

    leave = 0;
    while (!leave) {
/*      if ( num && ( read_nibbles(saturn.PC, 2) == 0xD8 ||
          read_nibbles(saturn.PC, 2) == 0xF8 )) {
        printf("D8 : %.5lx\n", saturn.PC );
        step = saturn.PC + 7;
      }
*/
        step_instruction ();
        if (schedule_event-- == 0)
          schedule ();

      if ( disassembler_mode == HP_MNEMONICS || 
           disassembler_mode == CLASS_MNEMONICS )
        leave = 1;
      else {
      naddr = disassemble (saturn.old_PC[saturn.old_index], instr, DIS_INSTR);
      opcode = read_nibbles(saturn.old_PC[saturn.old_index],
                   naddr - saturn.old_PC[saturn.old_index]);
      if (opcode == 0xC808 || opcode == 0x708)
        leave = 1;
      }
    }
    update_gui();

    if (Trace) {
      i++;
      naddr = disassemble (saturn.old_PC[saturn.old_index], instr, DIS_ALL);
      opcode = read_nibbles(saturn.old_PC[saturn.old_index],
                   naddr - saturn.old_PC[saturn.old_index]);
      if ( (k=match_entry(saturn.old_PC[saturn.old_index])) != 0) {
        sprintf(buf,"*** SysRPL : %.5lx - %s\n",
              saturn.entries[k].rom, saturn.entries[k].defn);
        fwrite(buf, 1, strlen(buf), fp);
      }
      if (saturn.old_PC[saturn.old_index] == DOCOL)
        indent ++;
      if (saturn.PC == SEMI+5 || saturn.old_PC[saturn.old_index] == COLA +5 )
        indent --;
      sprintf (buf, "%.8ld - ", i);
      fwrite(buf, 1, strlen(buf), fp);
      for (l = indent; l >=0 ; l--) {
        fwrite(" ", 1, 1, fp);
      }
      sprintf (buf, "%.5lX <%8lX>: %s\n", 
                      saturn.old_PC[saturn.old_index], opcode, instr);
      fwrite(buf, 1, strlen(buf), fp);
    }

    if (exec_flags & EXEC_BKPT) {
      if (check_breakpoint (BP_EXEC, saturn.PC)) {
        enter_debugger |= BREAKPOINT_HIT;
        draw_logo(RED);
        break;
      }
    }

  }
  while (!enter_debugger && exec_flags && !num);

  return 0;
}
