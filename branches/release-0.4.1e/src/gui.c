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

/* 
 * $Id: debugger.c,v 1.8 1995/01/11 18:20:01 ecd Exp ecd $
 */

#include "global.h"

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
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

extern int Register;
extern int Stack;
extern int Disassembly;
extern int Memory;
extern int Object;

word_20  last_PC;
char     last_intruct[100];

static void
#ifdef __FunctionProto__
gui_stack (void)
#else
gui_stack ()
#endif
{
  word_20 dsktop, dskbot;
  word_20 sp = 0, end = 0, ent = 0;
  word_20 ram_base, ram_mask;
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

  update_stack( stack, n);

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

int
#ifdef __FunctionProto__
up_objaddr( )
#else
up_objaddr( )
#endif
{
  struct objaddr_t *new, *last, *second;

  second = last = (struct objaddr_t *)0;
  new = ObjectAddress;
  while(new) {
    second = last;
    last = new;
    new = new->obj_next;
  }
  second->obj_next = (struct objaddr_t *)0;
  free(last);

  return 1;
}

word_20
#ifdef __FunctionProto__
last_objaddr( )
#else
last_objaddr()
#endif
{
  struct objaddr_t *new, *last;

  last = (struct objaddr_t *)0;
  new = ObjectAddress;
  while(new) {
    last = new;
    new = new->obj_next;
  }

  return last->addr;

}

int
#ifdef __FunctionProto__
add_objaddr(word_20 addr )
#else
add_objaddr(addr)
  word_20  addr;
#endif
{
  struct objaddr_t *new, *last;

  last = (struct objaddr_t *)0;
  new = ObjectAddress;
  while(new) {
    last = new;
    new = new->obj_next;
  }

  new = (struct objaddr_t *)malloc(sizeof(struct objaddr_t));
  new->addr = addr;
  new->obj_next = (struct objaddr_t *)0;
  last->obj_next = new;
  
  return 1;

}

int
#ifdef __FunctionProto__
disp_dir(int y, struct keypad_t d, char *path )
#else
disp_dir(y, d, path);
  int y;
  struct keypad_t  d;
  char *path;
#endif
{
  struct         stat st;
  FILE           *fp;
  DIR            *directory;
  struct dirent  *nth_file;
  unsigned char  in[16], file[1024], buf[1024];
  unsigned long  size;
  int            line = 4, endline;
  word_20        prolog;
  struct objfunc *op;

  directory = (DIR *)0;
  nth_file = (struct dirent *)0;
  endline = XWinLines(d) ;

  if (verbose)
    fprintf(stderr, "opening directory %s\n", path); 

  if (NULL== (directory = opendir(path)) ) {
    fprintf(stderr, "can\'t open directory %s\n", path);
    fprintf(stderr, "%s\n", strerror(errno));
    return 1 ;
  }
  while (NULL != (nth_file = readdir(directory)) && line <= endline ) {
    strcpy(file, path);
    if ( !strcmp(nth_file->d_name, ".") || !strcmp(nth_file->d_name, ".."))
      continue;

    strcat(file, nth_file->d_name);

    if ( NULL == (fp = fopen( file, "r"))) {
      if (verbose) {
        fprintf(stderr, "can\'t open file %s\n", file);
        fprintf(stderr, "%s\n", strerror(errno));
      }
      continue;
    }
    if (stat(file, &st) != 0) {
      if (verbose) 
        fprintf(stderr, "can\'t stat %s \n", file);
      fclose(fp);
      continue;
    }
    if( (size = st.st_size) >= 0x20000 ) {   /* 128 kbs */ 
      fclose( fp );
      continue;
    }
    if ( S_ISDIR(st.st_mode) ) {
      if ( !y ) {
        sprintf(buf, "<  DIR > %s", nth_file->d_name); 
        XDrawStr(d, line, buf, NORM);
      } else if ( y == line) {
        sprintf(buf, "%s/", nth_file->d_name);
        strcat ( saturn.lwd,  buf);
        return 0;
      }
      line++;
      fclose(fp);
      continue;
    }
    if (fread(in, 1, 7, fp) != 7) {
      if (verbose)
        fprintf(stderr, "can\'t read header from %s\n", file); 
      fclose(fp);
      continue;
    }
    if ( strncmp(in,"HPHP48-",7) ) {
      fclose(fp);
      if (verbose)
        fprintf(stderr, "%s is not a HP48 file\n", file);
      continue;
    }
    fread(in, 1, 6, fp);
    fclose(fp);
    if ( !y ) {
      prolog = (word_20)(in[1] + (in[2] << 8) + ((in[3] & 0xf) << 16 ) );
      for (op = objects ; op->prolog != 0; op++) {
        if (op->prolog == prolog)
          break;
      }
      if ( op->prolog)
        sprintf(buf, "<%s %li> %s \n", op->sname, (size-8), nth_file->d_name);
      else 
        sprintf(buf, "<%.5lx %li> %s \n",prolog, (size-8), nth_file->d_name);
      XDrawStr(d, line, buf, NORM);
    } else if ( y == line ) {
      sprintf(buf, "%s%s", path, nth_file->d_name);
      printf("Loading %s\n", buf);
      LoadObject(buf);
    }
    line++;

  }
  XClearBot(d, line);
  return 1;
}

int
#ifdef __FunctionProto__
disp_DORRP(int y, struct keypad_t d, word_20 addr )
#else
disp_DORRP(y, d, addr)
  int y;
  struct keypad_t  d;
  word_20  addr;
#endif
{

  word_20     start, len;
  int         endline, line=0, home=0,leave,i, val, str; 
  char        buf[100], buf1[65536];

  if (addr ==  read_nibbles(0x70592,5) ) {
    home = 1;
    line = 2; 
  } else
    line = 3;

  endline = XWinLines(d) ;
  addr += 5;
  start = read_nibbles(addr,3);
  addr += 3;
  if ( home ) {
    addr += 13*start;               /* Skip over attached libs */
  }
  if ( read_nibbles(addr,5) != 0 ) {
    addr += read_nibbles(addr,5);
    start = addr-5;
    leave = 0;
    while (! leave ) {
      sprintf(buf, "<12345> ");
      len = read_name(addr, buf);
      addr += len;
      if ( len != 2 ) {
        for (i = len/2; i <=13 ; i++)
          sprintf(&buf[strlen(buf)]," ");
      } else
        sprintf(&buf[8],"NULL        " );
      if ( y == 0) {
        sprintf(&buf[1],"%.5lX", addr);
        buf[6]='>';
        decode_rpl_obj(addr, &len, buf1);
        val = strlen(buf);
        str = 17;
        if (strlen(buf1) > str+3) {
          strncpy(&buf[val], buf1, str+3);
          sprintf(&buf[val+str], "...");
          buf[val + str+3] = '\0';
        } else
          strcpy(&buf[val], buf1);
        XDrawStr(d, line, buf, NORM);
      } else {
        if (y == line ) {
          len = read_nibbles(addr,5 );
          if ( len == DORRP || len == DOCOL) {
            add_objaddr(addr);
            return 0;
          } else
            printf("Dis %.5lx:%.5lx\n", addr, len);
        }
      }

      line++;

      if ( read_nibbles(start,5) == 0 || line == endline) {
        leave = 1;
      } else {
        addr = start - read_nibbles(start, 5);
        start = addr - 5;
      }
    }
    if (read_nibbles(start,5) != 0 ) {
      sprintf(buf, "MORE");
      XDrawStr(d, line, buf, NORM);
      line++;
    }
  }
  XClearBot(d, line);
  return 1;
}

int
#ifdef __FunctionProto__
disp_DOCOL(int y, struct keypad_t d, word_20 addr )
#else
disp_DOCOL(y, d, addr)
  int y;
  struct keypad d;
  word_20  addr;
#endif
{
  int      j, level,line;
  word_20  addr1,prolog, prolog2 ;
  char     buf[100], buf1[65000];
  struct objfunc *op;

  line = 3; 
  level = 0;
  addr1 = addr;
  while ( ( read_nibbles(addr1,5) != SEMI || level != 0 ) &&
                line <= 15 ) {
    prolog = read_nibbles(addr,5);
    if ( prolog == USERRPLSTART )
      level++;
    prolog2 = read_nibbles(prolog,5);
    sprintf(buf,"<%.5lX> ",addr);
    addr1 = addr;
    if (prolog == DOCOL) {
      level ++;
      addr +=5;
      sprintf(&buf[strlen(buf)], "PRM%.2i : Y -", level);
      for (j = 0; j < level ; j++)
        sprintf(&buf[strlen(buf)]," ");
      sprintf(&buf[strlen(buf)],"::");
    } else if (prolog == SEMI ) {
      sprintf(&buf[strlen(buf)], "PRM%.2i : Y -", level);
      for (j = 0; j < level ; j++)
        sprintf(&buf[strlen(buf)]," ");
      sprintf(&buf[strlen(buf)],";");
      level--;
      addr +=5;
    } else {
      dec_rpl_obj(&addr, buf1);
      for (op = objects ; op->prolog != 0; op++) {
        if (op->prolog == prolog)
          break;
      }
      if (op->prolog || prolog == addr + 5) {
        sprintf(&buf[strlen(buf)],"%s", op->sname);
      } else {
        for (op = objects ; op->prolog != 0; op++) {
          if (op->prolog == prolog2)
            break;
        }
        if (op->prolog) {
          sprintf(&buf[strlen(buf)],"%s", op->sname);
        } else {
          sprintf(&buf[strlen(buf)],"%.5lX", prolog2);
        }
      }
      sprintf(&buf[strlen(buf)]," : Y -");
      for (j = 0; j < level ; j++)
        sprintf(&buf[strlen(buf)]," ");
      sprintf(&buf[strlen(buf)],"%s", buf1);
    }
    if (y == 0) {
      XDrawStr(d, line, buf, NORM);
    }
    else if ( y == line ) {
      printf("Break at D0:%.5lx : A:%.5lx\n",addr1+5, prolog);
      where_start(addr1+5);
      break_rpl (prolog, addr1+5);
    }
    line++;
    if (prolog == 0x23639 || prolog == 0x235FE )
      level -= 1;

  }
  XClearBot(d, line);
  return 1;
}

int
#ifdef __FunctionProto__
disp_DOLIB(int y, struct keypad_t d, word_20 addr )
#else
disp_DOLIB(y, d, addr)
  int y;
  struct keypad_t d;
  word_20  addr;
#endif
{
  int           endline, k , line = 3, i=0 ;
  word_20       addr1, hash, names;
  char          buf[100] ;
  word_20       len, prolog;
  word_20       libsize, libidsize, libnum;
  struct objfunc *op;

  endline = XWinLines(d) ; 
  addr += 5;
  libsize = read_nibbles(addr,5);
  libidsize = read_nibbles(addr + 5, 2);
  if (libidsize !=0 )
    addr +=  2 * libidsize + 12;
  else
    addr += 15;
  libnum = read_nibbles(addr -3 , 3);
  hash = addr + read_nibbles(addr, 5);
  names = read_nibbles(hash + (5*18), 5) + hash + (5*18);

  addr += 10;
  addr1 = read_nibbles(addr, 5) + addr;
  k = (int) (read_nibbles(addr1 + 5, 5) / 5);
  addr1 +=10;

  i = 0;
  while (i <=k  && line <= endline ) {
    sprintf(buf, "%.3lX:%.3i ", libnum, i);
    addr = read_nibbles(addr1,5);
    if ( addr & 0x80000)
      addr = addr - 0xFFFFF - 1 ;
    prolog = read_nibbles(addr + addr1, 5);

    if ( y == 0) {
      if (read_nibbles(addr + addr1 - 6, 3) == libnum) {
        hash = names - read_nibbles(names,5);
        len = read_name(hash, buf);
      }
      while (strlen(buf) <= 20 )
        sprintf(&buf[strlen(buf)]," ");
      for (op = objects ; op->prolog != 0; op++) {
        if (op->prolog == prolog)
          break;
      }
      if ( op->prolog) {
        sprintf(&buf[strlen(buf)]," %s", op->sname);
      }
      XDrawStr(d, line, buf,NORM);
    } else if ( y == line ) {
      add_objaddr(addr+addr1);
      return 0;
    } 

    line++;
    names += 5;
    addr1 +=5;
    i++;
  }

  XClearBot(d, line);
  return 1;
}

int
#ifdef __FunctionProto__
disp_PORTn(int y, struct keypad_t d, word_20 addr )
#else
disp_PORTn(y, d, addr)
  int y;
  keypad_t  d;
  word_20  addr;
#endif
{
  word_20   prolog, len;
  char      buf[100], buf1[65000];
  int       line = 2, str, val; 

  addr += 5;
  while( (prolog = read_nibbles(addr,5)) !=0) {
    sprintf(buf, "<%.5lx> ", addr);
    decode_rpl_obj(addr, &len, buf1);
    if ( y == 0 ) {
      strcpy(buf1, &buf1[16]);
      if ( prolog == DOBAK ) {
        read_name(addr+10, buf);
      }
      val = strlen(buf);
      str = 28;
      if (strlen(buf1) > str+3) {
        strncpy(&buf[val], buf1, str+3);
        sprintf(&buf[val+str], "...");
        buf[val + str+3] = '\0';
      } else
        strcpy(&buf[val], buf1);
      XDrawStr(d, line, buf,NORM);

    } else if ( y == line && prolog == DOLIB ) {
      add_objaddr(addr);
      return 0;
    } else if (y == line && prolog == DOBAK ) {
      if( read_nibbles( addr+(read_nibbles(addr+10,2)+1)*2+12,5) == DOCOL ){
        add_objaddr( addr+(read_nibbles(addr+10,2)+1)*2+12 );
        return 0;
      }
    } else if (y == line )
      printf (" Huh? addr:%.5lx prolog:%.5lx\n", addr, prolog);

    line++;

    addr += len;
  }

  XClearBot(d, line);
  return 1;
}

int
#ifdef __FunctionProto__
update_regs(void)
#else
update_regs()
#endif
{

  int           line = 1,  i, val;
  word_20       addr, naddr;
  char          instr[100], buf[100];

  line = 1;

  if (buttons_menu[MBUTTON_REGPC].code) {
    naddr = disassemble (saturn.PC, buf, DIS_INSTR);
    addr = read_nibbles(saturn.PC, naddr - saturn.PC);
    sprintf (instr, "   PC: %.5lX  OPCODE: %lX", saturn.PC, addr);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGA].code) {
    dump_reg ("    A: ", 16, saturn.A, instr);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGB].code) {
    dump_reg ("    B: ", 16, saturn.B, instr);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGC].code) {
    dump_reg ("    C: ", 16, saturn.C, instr);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGD].code) {
    dump_reg ("    D: ", 16, saturn.D, instr);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGR0].code) {
    dump_reg ("   R0: ", 16, saturn.R0, instr);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGR1].code) {
    dump_reg ("   R1: ", 16, saturn.R1, instr);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGR2].code) {
    dump_reg ("   R2: ", 16, saturn.R2, instr);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGR3].code) {
    dump_reg ("   R3: ", 16, saturn.R3, instr);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGR4].code) {
    dump_reg ("   R4: ", 16, saturn.R4, instr);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGD0].code) {
    sprintf (instr, "   D0: %.5lX ->", saturn.D0);
    for (i = 0; i < 20; i += 5)
      {
        val = strlen(instr);
        sprintf (&instr[val], " %.5lX", read_nibbles (saturn.D0 + i, 5));
      }
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGD1].code) {
    sprintf (instr, "   D1: %.5lX ->", saturn.D1);
    for (i = 0; i < 20; i += 5)
      {
        val = strlen(instr);
        sprintf (&instr[val], " %.5lX", read_nibbles (saturn.D1 + i, 5));
      }
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGIN].code) {
    dump_reg ("   IN: ", 4, saturn.IN, instr);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGOUT].code) {
    dump_reg ("  OUT: ", 3, saturn.OUT, instr);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGCARY].code) {
    sprintf(instr, "CARRY: %.1d",saturn.CARRY);
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGST].code) {
    val = 0;
    for (i = NR_PSTAT - 1; i >= 0; i--) {
      val <<= 1;
      val |= saturn.PSTAT[i] ? 1 : 0;
    }
    sprintf (instr,"   ST: %.4X ( ", val);
    for (i = NR_PSTAT - 1; i >= 0; i--) {
      val = strlen(instr);
      if (saturn.PSTAT[i])
          sprintf (&instr[val], "%.1X", i);
      else
          sprintf (&instr[val], "-");
      if ((float)i/4 == i/4)
         sprintf (&instr[val+1], " ");
    }
    val = strlen(instr);
    sprintf(&instr[val], ")" );
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  if (buttons_menu[MBUTTON_REGHST].code) {
    val = 0;
    if (saturn.XM != 0)
      val |= 1;
    if (saturn.SB != 0)
      val |= 2;
    if (saturn.SR != 0)
      val |= 3;
    if (saturn.MP != 0)
      val |= 4;
    sprintf (instr, "  HST: %.1X    (%s%s%s%s)", val,
          saturn.MP ? "MP " : "-- ", saturn.SR ? "SR " : "-- ",
          saturn.SB ? "SB " : "-- ", saturn.XM ? "XM" : "--");
    XDrawStr(regpad, line, instr, NORM);
    line++;
  }

  XClearBot(regpad, line);

  DrawKeypad( regpad, regWin);

  return 1;
}

void
#ifdef __FunctionProto__
get_PATH(char *buf)
#else
get_PATH(buf)
  char *buf;
#endif
{
  int           i=0, k;
  word_20       addr, prolog,hash, names, addr1;
  word_20       len ;
  word_20       libnum=0;
  struct  objaddr_t   *objaddr, *lastaddr;
  
  lastaddr = objaddr = (struct objaddr_t *)0;
  objaddr = ObjectAddress;
  if (objaddr->addr ==  read_nibbles(0x70592,5))
    sprintf(buf, "{ HOME ");
  else if (objaddr->addr ==  read_nibbles(0x70597,5) - 5)
    sprintf(buf, "{ PORT0 ");

  objaddr = objaddr->obj_next;

  while(objaddr) {
    prolog = read_nibbles(objaddr->addr, 5);
    if (prolog == DORRP) {
      addr = objaddr->addr - 2;
      len = read_nibbles(addr, 2);
      if ( len ) {
        read_name (addr-(len+1)*2 , buf);
      } else
        sprintf(&buf[strlen(buf)],"Hidden");
      sprintf(&buf[strlen(buf)]," ");
    } else if (prolog == DOLIB) {
      len = read_nibbles(objaddr->addr+10,2);
      if (len)
        libnum = read_nibbles(objaddr->addr+10 + ((len+2) * 2),3);
      else
        libnum = read_nibbles(objaddr->addr+12,3);
      sprintf(&buf[strlen(buf)],"LIB%ld ", libnum);
      libnum = objaddr->addr;
    } else if (prolog == DOCOL && !libnum ) {
      addr = objaddr->addr - 2;
      len = read_nibbles(addr, 2);
      if ( len ) {
        read_name (addr-(len+1)*2 , buf);
      } else
        sprintf(&buf[strlen(buf)],"NULLVAR");
      sprintf(&buf[strlen(buf)]," ");
    } else if ( prolog == DOCOL && libnum ) {

      addr = libnum + 5;
      if (read_nibbles(addr + 5, 2) !=0 )
        addr +=  2 * read_nibbles(addr + 5, 2) + 12;
      else
        addr += 15;
      hash = addr + read_nibbles(addr, 5);
      names = read_nibbles(hash + (5*18), 5) + hash + (5*18);

      addr += 10;
      addr1 = read_nibbles(addr, 5) + addr;
      k = (int) (read_nibbles(addr1 + 5, 5) / 5);
      addr1 +=10;
      i = 0;
      while (i <=k ) {
        addr = read_nibbles(addr1,5);
        if ( addr & 0x80000)
          addr = addr - 0xFFFFF - 1 ;

        if ( (addr + addr1) == objaddr->addr ){
          hash = names - read_nibbles(names,5);
          len = read_name(hash, buf);
          if (len == 2) {
            sprintf(&buf[strlen(buf)],"XLIB%.3i ",i);
          }
        }

        names += 5;
        addr1 +=5;
        i++;
      }

    }

    lastaddr = objaddr;
    objaddr = objaddr->obj_next;
  }
  sprintf(&buf[strlen(buf)],"}");

}

int
#ifdef __FunctionProto__
update_object(int y)
#else
update_object(y)
  int y;
#endif
{
  int           line = 1, offset, i;
  word_20       addr;
  char          buf[100] ;
  word_20       len;
  int           home = 0;

  offset = objpad.offset;

  if (y != 0 && y >= offset) {
    y -= offset;
    y = y/font_height;
    y++;
  }

  if(buttons_menu[MBUTTON_OBJHOME].code) {
    get_PATH(buf);
    XDrawStr(objpad, line, buf, NORM);
    line++;

    addr = last_objaddr();
    if (addr == read_nibbles(0x70592,5 ))
      home = 1;
    else {
      home =0;
      if ( y == 0 ) {
        sprintf(buf,"..");
        XDrawStr(objpad, line, buf, NORM);
       }
      if ( y == line ) {
         up_objaddr();
         update_object(0);
         return 0;
      }
      line++;
    }

    len = read_nibbles(addr, 5);
    if ( len == DORRP ) {
      if (! disp_DORRP(y, objpad, addr)) {
        update_object(0);
        return 0;
      }
       
    } else if ( len == DOCOL ) {
      disp_DOCOL(y, objpad, addr);
    }

  } else if (buttons_menu[MBUTTON_OBJPORT0].code) {

    get_PATH(buf);

    XDrawStr(objpad, line, buf, NORM);
    line++;

    addr = last_objaddr();
    if (addr != read_nibbles(0x70597,5) - 5) {
      if ( y == 0 ) {
        sprintf(buf, "..");
        XDrawStr(objpad, line, buf, NORM);
      } else if (y == line ) {
        up_objaddr();
        update_object(0);
        return 0;
      }
      line++;
      if (read_nibbles(addr,5) == DOLIB)
        if(!disp_DOLIB(y, objpad, addr)) {
          update_object(0);
          return 0;
        }
      if (read_nibbles(addr,5) == DOCOL)
        disp_DOCOL(y, objpad,addr);

    } else {
      if (!disp_PORTn(y, objpad, addr) ) {
        update_object(0);
        return 0;
      }
    }

  } else if (buttons_menu[MBUTTON_OBJRECV].code) {
    sprintf(buf, "HP48 : { STACK %li }", free_mem() );
    XDrawStr(objpad, line, buf, NORM);
    line++;
    sprintf(buf, " X48 : { %s }", saturn.lwd);
    XDrawStr(objpad, line, buf, NORM);
    line++;
    if (strlen(saturn.lwd) >= 2) {
      if ( !y ){
        sprintf(buf, "..");
        XDrawStr(objpad, line, buf, NORM);
      } else if ( y == line ) {
        
        for (i = (strlen(saturn.lwd) - 2) ; i >= 0 ; i --) {
          if (saturn.lwd[i] == '/' ) {
            break;
          }
        }
        saturn.lwd[i+1]=0;
        update_object(0);
        return 0;
      }
      line++;
    }

    if (!disp_dir(y, objpad, saturn.lwd)) {
      update_object(0);
      return 0;
    }

  }
  if (line == 1)
    XClearBot(objpad, line); 

  DrawKeypad( objpad, objWin);
  return 0;
}

int
#ifdef __FunctionProto__
update_memory(void )
#else
update_memory()
#endif
{
  word_20 addr, code, prolog;
  char    buf [100], buf1[100];
  int     endline, i ;
  int     indent, pos ;
  int  line = 0, j ;
  struct objfunc *op;

  addr = MemoryAddress;

  if (buttons_menu[MBUTTON_RAMREGA].code)
    addr =  (saturn.A[4] << 16 ) + (saturn.A[3] << 12) + (saturn.A[2] << 8) +
            (saturn.A[1] << 4 ) + saturn.A[0];
  if (buttons_menu[MBUTTON_RAMREGB].code)
    addr =  (saturn.B[4] << 16 ) + (saturn.B[3] << 12) + (saturn.B[2] << 8) +
            (saturn.B[1] << 4 ) + saturn.B[0];
  if (buttons_menu[MBUTTON_RAMREGC].code)
    addr =  (saturn.C[4] << 16 ) + (saturn.C[3] << 12) + (saturn.C[2] << 8) +
            (saturn.C[1] << 4 ) + saturn.C[0];
  if (buttons_menu[MBUTTON_RAMREGD].code)
    addr =  (saturn.D[4] << 16 ) + (saturn.D[3] << 12) + (saturn.D[2] << 8) +
            (saturn.D[1] << 4 ) + saturn.D[0];
  if (buttons_menu[MBUTTON_RAMREGD0].code)
    addr = saturn.D0;
  if (buttons_menu[MBUTTON_RAMREGD1].code)
    addr = saturn.D1;

  endline = XWinLines(rampad);
  for (line = 1; line <= endline ; line ++) {

    sprintf(buf,"%.5lX: ", addr);
    for (i=0; i<16; i+=2 ) {
      code = read_nibbles(addr, 2);
      sprintf(&buf[7+i],"%lX", code & 0x0f);
      sprintf(&buf[8+i],"%lX", code & 0xf0);
      if ( code >= 0x20 && code < 0x7F )
        sprintf(&buf1[i]," %c", (int)code);
      else
        sprintf(&buf1[i]," .");

      addr +=2;
    }

    sprintf(&buf[23], " %s", buf1);

    if (buttons_menu[MBUTTON_RAMPROLOG].code) {

      for (i=20; i >=1 ; i--) {
        prolog = read_nibbles(addr-i, 5);
        for (op = objects ; op->prolog != 0; op++) {
          if (op->prolog == prolog)
            break;
        }
        if (op->prolog || prolog == addr - i + 5) {

          pos = 0;
          if ( i > 16 ) { 
            pos = 25;
            j = 20 - i;
            memcpy(&buf[25], &op->sname[4-j], j);
          } else {
            pos= 25 + (16 - i) ;
            if (i <= 5)
              j = i-1;
            else
              j = 5;
            memcpy(&buf[pos], op->sname, j);
          }
        }
      }
    }
    XDrawStr(rampad, line, buf, NORM);

/*
    if (buttons_menu[MBUTTON_RAMADDR].code) {
      j = closest_entry(addr-16);
      code = saturn.entries[j].rom;
      while (code < addr && line != 15) {
        line++;
        sprintf(buf1, " %.5lX: %s", code, saturn.entries[j].defn);
        XDrawImageString( dpy, ramWin, gc_stack,
                    indent, line*height + offset, buf1, strlen(buf1));
        clear_from = XTextWidth(DispFont, buf1, strlen(buf1)) + indent;
        XFillRectangle (dpy, ramWin, gc_stack_erase,
                     clear_from, offset + (line-1)*height + coffset,
                     win_width - clear_from + indent, height);
        j = closest_entry(code+1);
        code = saturn.entries[j].rom;
      }
    }
*/

  }
  DrawKeypad(rampad, ramWin);
 
  return 0;
}

int
#ifdef __FunctionProto__
update_instr(void )
#else
update_instr()
#endif
{
  word_20  addr, naddr, opcode;
  char     buf [100], instr[100], buf1[65523];
  int      i;
  int      line = 6, endline;
  word_64  reg;

  endline = XWinLines(asmpad) - 2;
  line = (endline / 2) - 1;

  if ( saturn.PC != last_PC)
  /* List out the last n instructions */
    XMoveWin(asmpad, line-1);

  last_PC = saturn.PC;

  switch ( disassembler_mode ) {
    case HP_MNEMONICS:
    case CLASS_MNEMONICS:
      /* dump the 2nd last instruction executed */
      XDrawStr(asmpad, line, last_intruct, NORM);
      line++;

      /* dump the just executed instruction */
      addr = disassemble (saturn.old_PC[saturn.old_index], instr, DIS_ALL);
      opcode =  read_nibbles(saturn.old_PC[saturn.old_index], 
                       addr-saturn.old_PC[saturn.old_index]);
      sprintf (last_intruct, "%.5lX <%lX>       ", 
                saturn.old_PC[saturn.old_index], opcode );
      sprintf (&last_intruct[16],"%s", instr);
      XDrawStr(asmpad, line, last_intruct, INVR);
      line++;

      addr = saturn.PC;
      /* List the next 5 instructions */
      while ( line <= endline ) {
        naddr = disassemble (addr, instr, DIS_INSTR);
        opcode =  read_nibbles(addr, naddr - addr);
        if (strlen(instr) > 20)
          instr[24] = 0;
        sprintf (buf, "%.5lX <%lX>        ", addr, opcode);
        sprintf (&buf[16],"%s", instr);
        addr = naddr;
        XDrawStr(asmpad, line, buf, NORM);
        line++;
      }

      /* dump the return stack on the botton */
      line++;
      sprintf(buf, " ");
      for (i = 0; i < 8 ; i++) {
        sprintf (&buf[strlen(buf)], "RS%d:", i); 
        if (i <= saturn.rstkp )
          sprintf (&buf[strlen(buf)], "%.5lX ", saturn.rstk[saturn.rstkp-i]);
        else
          sprintf (&buf[strlen(buf)], "----- ");
      }
      XDrawStr(asmpad, line, buf, NORM);
      line++;
      break;
    case SYSRPL_MNEMONICS:
    case USERRPL_MNEMONICS:
      addr = saturn.D0 - 5;
      if (addr >= 0x70000 ) {
        read_reg(&reg,  bkreg[0].reg0);
        for (i = 0; i <= 5 ; i++) {
          sprintf(buf, "<%.5lX> ", addr);
          printf("%s\n", buf);
          dec_rpl_obj(&addr, buf1);
          sprintf(&buf[6],"  %s", buf1);
          XDrawStr(asmpad, line, buf, NORM);
          line++;
        }
      }
    break;
  }

  XClearBot(asmpad, line);
  DrawKeypad(asmpad, asmWin);
  return 1;
}

void
#ifdef __FunctionProto__
update_gui (void)
#else
update_gui ()
#endif
{
  int i;
  i=0;
  if (Stack)
    gui_stack();
  if (Disassembly)
    update_instr(); 
  if (Register)
    update_regs();
  if (Memory)
    update_memory();

}
