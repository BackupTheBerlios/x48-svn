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

/* $Log: x48_x11.h,v $
 * Revision 1.11  1995/01/11  18:20:01  ecd
 * major update to support HP48 G/GX
 *
 * Revision 1.10  1994/12/07  20:16:41  ecd
 * added "refresh_icon"
 *
 * Revision 1.10  1994/12/07  20:16:41  ecd
 * added "refresh_icon"
 *
 * Revision 1.9  1994/11/28  02:19:22  ecd
 * added support for contrast adjustment
 *
 * Revision 1.8  1994/11/04  03:44:47  ecd
 * added support for mono and gray displays
 *
 * Revision 1.7  1994/11/02  14:51:27  ecd
 * minor fix
 *
 * Revision 1.6  1994/10/09  20:26:35  ecd
 * changed disp_t
 *
 * Revision 1.5  1994/10/06  16:29:28  ecd
 * added XShm - Extension stuff
 *
 * Revision 1.4  1994/10/05  08:33:22  ecd
 * changed disp_t: removed Pixmap
 *
 * Revision 1.3  1994/09/30  12:32:49  ecd
 * changed display stuff, added detection of interrupts in GetEvent
 *
 * Revision 1.2  1994/09/18  15:31:58  ecd
 * started Real Time support
 *
 * Revision 1.1  1994/09/13  15:05:11  ecd
 * Initial revision
 *
 *
 * $Id: x48_x11.h,v 1.11 1995/01/11 18:20:01 ecd Exp ecd $
 */

#ifndef _X48_X11_H
#define _X48_X11_H 1

#include "global.h"

#include <X11/Xlib.h>
#ifdef HAVE_XSHM
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#endif

#define WHITE		0
#define LEFT		1
#define RIGHT		2
#define BUT_TOP 	3
#define BUTTON  	4
#define BUT_BOT 	5
#define LCD		6
#define PIXEL		7
#define PAD_TOP 	8
#define PAD		9
#define PAD_BOT		10
#define DISP_PAD_TOP	11
#define DISP_PAD	12
#define DISP_PAD_BOT	13
#define LOGO		14
#define LOGO_BACK	15
#define LABEL		16
#define FRAME		17
#define UNDERLAY	18
#define BLACK		19
#define GREY            20
#define RED             21
#define GREEN           22
#define YELLOW          23
#define BLUE            24

typedef struct color_t {
  char *name;
  int r, g, b;
  int mono_rgb;
  int gray_rgb;
  XColor xcolor;
} color_t;

extern color_t *colors;

#define COLOR(c)        (colors[(c)].xcolor.pixel)

/* buttons for menu/debugger */
#define MBUTTON_START      0
#define MBUTTON_MENUSTART  0

#define MBUTTON_GOC        0
#define MBUTTON_GOR        1
#define MBUTTON_MAINQUIT   2
#define MBUTTON_MAINEXIT   3

#define MBUTTON_DEBUG      4
#define MBUTTON_LOBJ       5
#define MBUTTON_LRAM       6
#define MBUTTON_LREG       7
#define MBUTTON_LSTK       8

#define MBUTTON_MENUDONE   8
#define MBUTTON_REGSTART   9 

#define MBUTTON_REGPC      9
#define MBUTTON_REGA      10
#define MBUTTON_REGB      11
#define MBUTTON_REGC      12
#define MBUTTON_REGD      13
#define MBUTTON_REGR0     14
#define MBUTTON_REGR1     15
#define MBUTTON_REGR2     16
#define MBUTTON_REGR3     17
#define MBUTTON_REGR4     18
#define MBUTTON_REGD0     19
#define MBUTTON_REGD1     20
#define MBUTTON_REGIN     21
#define MBUTTON_REGOUT    22
#define MBUTTON_REGCARY   23
#define MBUTTON_REGST     24
#define MBUTTON_REGHST    25

#define MBUTTON_REGDONE   25
#define MBUTTON_STSTART   26

#define MBUTTON_STADDR    26
#define MBUTTON_STTYPE    27
#define MBUTTON_STMORE    28
#define MBUTTON_STLESS    29

#define MBUTTON_STDONE    29
#define MBUTTON_ASMSTART  30

#define MBUTTON_ASMrplMODE  30 
#define MBUTTON_ASMasmMODE  31
#define MBUTTON_ASMGOC    32
#define MBUTTON_ASMGF     33
#define MBUTTON_ASMGR     34
#define MBUTTON_ASMGS     35
#define MBUTTON_ASMGO     36
#define MBUTTON_ASMGI     37
#define MBUTTON_ASMTE     38
#define MBUTTON_ASMTQ     39

#define MBUTTON_ASMTRAS   40
#define MBUTTON_BADD      41
#define MBUTTON_BRAG      42
#define MBUTTON_BREG      43
#define MBUTTON_BINT      44
#define MBUTTON_BRPL      45
#define MBUTTON_BCLR      46
#define MBUTTON_BNOW      47

#define MBUTTON_ASMDONE   47
#define MBUTTON_RAMSTART  48

#define MBUTTON_RAMUP1       48
#define MBUTTON_RAMUP10      49
#define MBUTTON_RAMUP100     50
#define MBUTTON_RAMUP1000    51
#define MBUTTON_RAMUP10000   52
#define MBUTTON_RAMHOME      53

#define MBUTTON_RAMDN1       54
#define MBUTTON_RAMDN10      55
#define MBUTTON_RAMDN100     56
#define MBUTTON_RAMDN1000    57
#define MBUTTON_RAMDN10000   58

#define MBUTTON_RAMREGA      59
#define MBUTTON_RAMREGB      60
#define MBUTTON_RAMREGC      61
#define MBUTTON_RAMREGD      62
#define MBUTTON_RAMREGD0     63
#define MBUTTON_RAMREGD1     64

#define MBUTTON_RAMPROLOG    65
#define MBUTTON_RAMADDR      66
#define MBUTTON_RAMRPL       67
#define MBUTTON_RAMSRPL      68
#define MBUTTON_RAMFIND      69
#define MBUTTON_RAMGOTO      70

#define MBUTTON_RAMDONE      70
#define MBUTTON_OBJSTART     71

#define MBUTTON_OBJHOME      71
#define MBUTTON_OBJSTACK     72
#define MBUTTON_OBJPORT0     73
#define MBUTTON_OBJPORT1     74
#define MBUTTON_OBJPORT2     75
#define MBUTTON_OBJSEND      76
#define MBUTTON_OBJRECV      77
#define MBUTTON_OBJBREAK     78

#define MBUTTON_OBJDONE      78
#define MBUTTON_DONE         78

#define UPDATE_MENU	1
#define UPDATE_DISP	2

#define NORM            1
#define INVR            0

typedef struct button_t {

  char          *name;
  short         pressed;
  short         key_down;

  int           code;
  int           x, y;
  unsigned int  w, h;

  int           lc;
  char          *label;
  short         font_size;
  unsigned int  lw, lh;
  unsigned char *lb;

  char          *letter;

  char          *left;
  short         is_menu;
  char          *right;
  char          *sub;

  Pixmap        map;
  Pixmap        down;
  Window        xwin;

} button_t;

extern struct button_t buttons_menu[];

typedef struct disp_t {
  unsigned int     w, h;
  Window           win;
  GC               gc;
  short            mapped;
  int		   offset;
  int		   lines;
#ifdef HAVE_XSHM
  int              display_update;
  XShmSegmentInfo  disp_info;
  XImage          *disp_image;
  XShmSegmentInfo  menu_info;
  XImage          *menu_image;
#endif
} disp_t;

extern disp_t   disp;

#ifdef HAVE_XSHM
extern int shm_flag;
#endif

typedef struct objaddr_t {
  word_20    addr;
  struct objaddr_t *obj_next;
} objaddr_t;
extern struct objaddr_t  *ObjectAddress;
extern word_20 MemoryAddress;

typedef struct keypad_t {
  unsigned int  offset;
  unsigned int  width;
  unsigned int  height;
  Pixmap        pixmap;
} keypad_t;

extern keypad_t  menupad;
extern keypad_t  regpad;
extern keypad_t  stackpad;
extern keypad_t  asmpad;
extern keypad_t  objpad;
extern keypad_t  rplpad;
extern keypad_t  rampad;

extern Display *dpy;
extern int	screen;
extern int      Trace;
extern int      font_height;
extern Window   objWin;
extern Window   asmWin;
extern Window   regWin;
extern Window   ramWin;

extern int	InitDisplay	 __ProtoType__((int argc, char **argv));
extern int	CreateWindows    __ProtoType__((int argc, char **argv));
extern int	GetEvent	 __ProtoType__((void));
extern int      update_stack     __ProtoType__((se *stack, int n));
extern int      update_instr     __ProtoType__((void));
extern int      update_regs       __ProtoType__((void));
extern int      update_memory    __ProtoType__((void));
extern void     draw_logo        __ProtoType__((int i));
extern int      XDrawStr         __ProtoType__((struct keypad_t d, int line, 
                                                   char *buf, int normal ));
extern void     XClearBot        __ProtoType__((struct keypad_t d, int line));
extern int      XMoveWin         __ProtoType__((struct keypad_t d, int line ));
extern int      DrawKeypad       __ProtoType__((struct keypad_t src, Drawable dest));
extern int      XWinLines        __ProtoType__((struct keypad_t d));

extern void	adjust_contrast  __ProtoType__((int contrast));
extern void	refresh_icon	 __ProtoType__((void));

extern void	ShowConnections	 __ProtoType__((char *w, char *i));

extern void	exit_x48	 __ProtoType__((int tell_x11));
extern int      on_event         __ProtoType__((void));

#ifdef HAVE_XSHM
extern void	refresh_display  __ProtoType__((void));
#endif

#endif /* !_X48_X11_H */
