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

/* $Log: debugger.h,v $
 * Revision 1.4  1995/01/11  18:20:01  ecd
 * major update to support HP48 G/GX
 *
 * Revision 1.3  1994/11/28  02:19:22  ecd
 * catch TRAP instructions
 *
 * Revision 1.3  1994/11/28  02:19:22  ecd
 * catch TRAP instructions
 *
 * Revision 1.2  1994/11/02  14:51:27  ecd
 * added breakpoint related stuff
 *
 * Revision 1.1  1994/10/04  15:12:38  ecd
 * Initial revision
 *
 *
 * $Id: debugger.h,v 1.4 1995/01/11 18:20:01 ecd Exp ecd $
 */

#ifndef _DEBUGGER_H
#define _DEBUGGER_H	1

#include "global.h"
#include "hp48.h"

#define	USER_INTERRUPT		1
#define	ILLEGAL_INSTRUCTION	2
#define	BREAKPOINT_HIT		4
#define	TRAP_INSTRUCTION	8

/*
 * exec_flags values
 */
#define EXEC_BKPT		1

/*
 * BreakPoint Flags 
 */
#define BP_EXEC         1
#define BP_READ         2
#define BP_WRITE        4
#define BP_RANGE        8
#define BP_REGISTER    16
#define BP_RPL         32
#define BP_TEMP        64

typedef struct bkreg_t {
  char    *name;
  int     len;
  unsigned char   *reg0;
  word_20         *reg1;
} bkreg_t; 

extern bkreg_t	bkreg[];

extern int	enter_debugger;
extern int	in_debugger;
extern int	exec_flags;
extern int      continue_flag;

extern void	init_debugger	__ProtoType__((void));
extern int	debug		__ProtoType__((void));
extern int	emulate_debug	__ProtoType__((int num));
extern int      emulate_over    __ProtoType__((void));
extern int      break_address   __ProtoType__((word_20 addr));
extern int      break_range     __ProtoType__((word_20 addrl, word_20 addrh));
extern int      break_rpl       __ProtoType__((word_20 a, word_20 d));
extern int      break_register  __ProtoType__((word_20 num, word_64 value,
                                                  word_64 mask));

extern char    *str_nibbles	__ProtoType__((word_20 addr, int n));
extern void     dump_reg        __ProtoType__((const char *reg, int n, 
                                                unsigned char *r, char *out));
extern void     read_reg        __ProtoType__((word_64 * val, 
                                                           unsigned char *r));
extern word_20  where_start     __ProtoType__((word_20 middle));

#endif /* !_DEBUGGER_H */
