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

/* $Log: buttons.h,v $
 * Revision 1.5  1995/01/11  18:20:01  ecd
 * major update to support HP48 G/GX
 *
 * Revision 1.4  1994/12/07  20:16:41  ecd
 * added some newlines
 *
 * Revision 1.4  1994/12/07  20:16:41  ecd
 * added some newlines
 *
 * Revision 1.3  1994/11/02  14:51:27  ecd
 * minor fix
 *
 * Revision 1.2  1994/10/06  16:28:03  ecd
 * changed char to unsigned
 *
 * Revision 1.1  1994/09/13  15:05:11  ecd
 * Initial revision
 *
 *
 * $Id: buttons.h,v 1.5 1995/01/11 18:20:01 ecd Exp ecd $
 */

#ifndef _BUTTONS_H
#define _BUTTONS_H 1

#define menu_label_width 24
#define menu_label_height 11
static unsigned char menu_label_bits[] = {
   0xfe, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x7f};

#define up_width 11
#define up_height 11
static unsigned char up_bits[] = {
   0x20, 0x00, 0x20, 0x00, 0x70, 0x00, 0x70, 0x00, 0xf8, 0x00, 0xf8, 0x00,
   0xfc, 0x01, 0xfc, 0x01, 0xfe, 0x03, 0xfe, 0x03, 0xff, 0x07};

#define down_width 11
#define down_height 11
static unsigned char down_bits[] = {
   0xff, 0x07, 0xfe, 0x03, 0xfe, 0x03, 0xfc, 0x01, 0xfc, 0x01, 0xf8, 0x00,
   0xf8, 0x00, 0x70, 0x00, 0x70, 0x00, 0x20, 0x00, 0x20, 0x00};

#define left_width 11
#define left_height 11
static unsigned char left_bits[] = {
   0x00, 0x04, 0x00, 0x07, 0xc0, 0x07, 0xf0, 0x07, 0xfc, 0x07, 0xff, 0x07,
   0xfc, 0x07, 0xf0, 0x07, 0xc0, 0x07, 0x00, 0x07, 0x00, 0x04};

#define right_width 11
#define right_height 11
static unsigned char right_bits[] = {
   0x01, 0x00, 0x07, 0x00, 0x1f, 0x00, 0x7f, 0x00, 0xff, 0x01, 0xff, 0x07,
   0xff, 0x01, 0x7f, 0x00, 0x1f, 0x00, 0x07, 0x00, 0x01, 0x00};

#define sqrt_width 20
#define sqrt_height 11
static unsigned char sqrt_bits[] = {
   0x00, 0xff, 0x0f, 0x00, 0x01, 0x08, 0x00, 0x01, 0x08, 0x80, 0x8c, 0x01,
   0x80, 0x58, 0x01, 0x80, 0x38, 0x00, 0x47, 0x30, 0x00, 0x4c, 0x30, 0x00,
   0x58, 0x78, 0x00, 0x30, 0x6a, 0x01, 0x20, 0xc6, 0x00};

#define power_width 17
#define power_height 14
static unsigned char power_bits[] = {
   0x00, 0x8c, 0x01, 0x00, 0x58, 0x01, 0x00, 0x38, 0x00, 0xc8, 0x30, 0x00,
   0x9c, 0x30, 0x00, 0x98, 0x78, 0x00, 0x58, 0x6a, 0x01, 0x58, 0xc6, 0x00,
   0x38, 0x00, 0x00, 0x30, 0x00, 0x00, 0x10, 0x00, 0x00, 0x08, 0x00, 0x00,
   0x05, 0x00, 0x00, 0x03, 0x00, 0x00};

#define inv_width 18
#define inv_height 13
static unsigned char inv_bits[] = {
   0x0c, 0x04, 0x00, 0x0f, 0x06, 0x00, 0x0c, 0x02, 0x00, 0x0c, 0x03, 0x00,
   0x0c, 0x01, 0x00, 0x8c, 0x19, 0x03, 0x8c, 0xb0, 0x02, 0xcc, 0x70, 0x00,
   0x40, 0x60, 0x00, 0x60, 0x60, 0x00, 0x20, 0xf0, 0x00, 0x30, 0xd4, 0x02,
   0x10, 0x8c, 0x01};

#define neg_width 21
#define neg_height 11
static unsigned char neg_bits[] = {
   0x18, 0x00, 0x00, 0x18, 0x30, 0x00, 0x18, 0x30, 0x00, 0xff, 0x18, 0x00,
   0xff, 0x18, 0x00, 0x18, 0x0c, 0x00, 0x18, 0x0c, 0x00, 0x18, 0xc6, 0x1f,
   0x00, 0xc6, 0x1f, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00};

#define bs_width 11
#define bs_height 11
static unsigned char bs_bits[] = {
   0x20, 0x00, 0x30, 0x00, 0x38, 0x00, 0xfc, 0x07, 0xfe, 0x07, 0xff, 0x07,
   0xfe, 0x07, 0xfc, 0x07, 0x38, 0x00, 0x30, 0x00, 0x20, 0x00};

#define alpha_width 12
#define alpha_height 10
static unsigned char alpha_bits[] = {
   0x78, 0x00, 0x84, 0x08, 0x82, 0x09, 0x83, 0x05, 0x83, 0x05, 0x83, 0x03,
   0x83, 0x01, 0x83, 0x01, 0x46, 0x09, 0x3c, 0x06};

#define div_width 10
#define div_height 10
static unsigned char div_bits[] = {
   0x30, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0xff, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x30, 0x00};

#define shl_width 24
#define shl_height 14
static unsigned char shl_bits[] = {
   0xfe, 0xff, 0x7f, 0xff, 0xfc, 0xff, 0x7f, 0xfc, 0xff, 0x3f, 0xfe, 0xff,
   0x1f, 0xff, 0xff, 0x0f, 0x00, 0xfc, 0x07, 0x00, 0xf8, 0x0f, 0x00, 0xf0,
   0x1f, 0xff, 0xf1, 0x3f, 0xfe, 0xf1, 0x7f, 0xfc, 0xf1, 0xff, 0xfc, 0xf1,
   0xff, 0xff, 0xf1, 0xfe, 0xff, 0x71};

#define mul_width 10
#define mul_height 10
static unsigned char mul_bits[] = {
   0x03, 0x03, 0x87, 0x03, 0xce, 0x01, 0xfc, 0x00, 0x78, 0x00, 0x78, 0x00,
   0xfc, 0x00, 0xce, 0x01, 0x87, 0x03, 0x03, 0x03};

#define shr_width 24
#define shr_height 14
static unsigned char shr_bits[] = {
   0xfe, 0xff, 0x7f, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfc,
   0xff, 0xff, 0xf8, 0x3f, 0x00, 0xf0, 0x1f, 0x00, 0xe0, 0x0f, 0x00, 0xf0,
   0x8f, 0xff, 0xf8, 0x8f, 0x7f, 0xfc, 0x8f, 0x3f, 0xfe, 0x8f, 0x3f, 0xff,
   0x8f, 0xff, 0xff, 0x8e, 0xff, 0x7f};

#define minus_width 10
#define minus_height 10
static unsigned char minus_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0xff, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define plus_width 10
#define plus_height 10
static unsigned char plus_bits[] = {
   0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0xff, 0x03, 0xff, 0x03,
   0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00};

#define last_width 120
#define last_height 6
static unsigned char last_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xc6, 0x1c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x29, 0x09,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x11, 0x49, 0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x10, 0x8f, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x29, 0x09, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xc9, 0x08,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x80};

#define colon_width 2
#define colon_height 10
static unsigned char colon_bits[] = {
   0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#ifndef buttons_menu
button_t buttons_menu[] = {
/* main menu */
/* 0*/ { "GoCont"  ,   0, 0, 0,   0,   0, 36, 26, WHITE, "GO", 0, 0, 0, 0, 0, "CONT", 1, 0, 0, 0 },
/* 1*/ { "GoReset" ,   0, 0, 0,  50,   0, 36, 26, WHITE, "GO", 0, 0, 0, 0, 0, "RESET", 1, 0, 0, 0 },
/* 2*/ { "StopQuit",   0, 0, 0, 200,   0, 36, 26, WHITE, "QUIT", 0, 0, 0, 0, 0, "SAVE", 1, 0, 0, 0 },
/* 3*/ { "StopExit",   0, 0, 0, 250,   0, 36, 26, WHITE, "QUIT", 0, 0, 0, 0, 0, "NO SAVE", 1, 0, 0, 0 },

/* 4*/ { "Debug",      0, 0, 0,   0,  50, 36, 26, WHITE, "DBG",  0, 0, 0, 0, 0, "DEBUG", 1, 0, 0, 0 },
/* 5*/ { "LookObject", 0, 0, 0, 100,  50, 36, 26, WHITE, "LOOK", 0, 0, 0, 0, 0, "OBJECT", 1, 0, 0, 0 },
/* 6*/ { "LookRAM",    0, 0, 0, 150,  50, 36, 26, WHITE, "LOOK", 0, 0, 0, 0, 0, "RAM", 1, 0, 0, 0 },
/* 7*/ { "LookREG",    0, 0, 0, 200,  50, 36, 26, WHITE, "LOOK", 0, 0, 0, 0, 0, "REGISTER", 1, 0, 0, 0 },
/* 8*/ { "LookStack",  0, 0, 0, 250,  50, 36, 26, WHITE, "LOOK", 0, 0, 0, 0, 0, "STACK", 1, 0, 0, 0 },

/* Register Menu */
/* 9*/ { "REG PC",     0, 0, 0,   0,   0, 36, 26, WHITE, "PC",   0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*10*/ { "REG A",      0, 0, 0,  50,   0, 36, 26, WHITE, "A",    0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*11*/ { "REG B",      0, 0, 0, 100,   0, 36, 26, WHITE, "B",    0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*12*/ { "REG C",      0, 0, 0, 150,   0, 36, 26, WHITE, "C",    0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*13*/ { "REG D",      0, 0, 0, 200,   0, 36, 26, WHITE, "D",    0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*14*/ { "REG R0",     0, 0, 0, 250,   0, 36, 26, WHITE, "R0",   0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*15*/ { "REG R1",     0, 0, 0,   0,  50, 36, 26, WHITE, "R1",   0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*16*/ { "REG R2",     0, 0, 0,  50,  50, 36, 26, WHITE, "R2",   0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*17*/ { "REG R3",     0, 0, 0, 100,  50, 36, 26, WHITE, "R3",   0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*18*/ { "REG R4",     0, 0, 0, 150,  50, 36, 26, WHITE, "R4",   0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*19*/ { "REG D0",     0, 0, 0, 200,  50, 36, 26, WHITE, "D0",   0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*20*/ { "REG D1",     0, 0, 0, 250,  50, 36, 26, WHITE, "D1",   0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*21*/ { "REG IN",     0, 0, 0,   0, 100, 36, 26, WHITE, "IN",   0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*22*/ { "REG OUT",    0, 0, 0,  50, 100, 36, 26, WHITE, "OUT",  0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*23*/ { "REG Carry",  0, 0, 0, 100, 100, 36, 26, WHITE, "CAR",  0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*24*/ { "REG ST",     0, 0, 0, 150, 100, 36, 26, WHITE, "ST",   0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },
/*25*/ { "REG HST",    0, 0, 0, 200, 100, 36, 26, WHITE, "HST",  0, 0, 0, 0, 0, "SHOW", 0, "SET", 0, 0 },

/* Stack Menu */
/*26*/ { "StAddr",     0, 0, 0,   0,   0, 36, 26, WHITE, "SHOW", 0, 0, 0, 0, 0, "ADDRESS", 1, 0, 0, 0 },
/*27*/ { "StType",     0, 0, 0,  50,   0, 36, 26, WHITE, "SHOW", 0, 0, 0, 0, 0, "TYPE", 1, 0, 0, 0 },
/*28*/ { "StMore",     0, 0, 0, 200,   0, 36, 26, WHITE, "LINE", 0, 0, 0, 0, 0, "MORE", 1, 0, 0, 0 },
/*29*/ { "StLess",     0, 0, 0, 250,   0, 36, 26, WHITE, "LINE", 0, 0, 0, 0, 0, "LESS", 1, 0, 0, 0},

/* Debugger Window */
/*30*/ { "dbgRPL",   0, 0, 0,   0,   0, 36, 26, WHITE, "RPL" , 0, 0, 0, 0, 0, "USER", 0, "SYS"  , 0, 0 },
/*31*/ { "dbgASS",   0, 0, 0,  50,   0, 36, 26, WHITE, "ASM" , 0, 0, 0, 0, 0, "HP"  , 0, "CLASS", 0, 0 },
/*32*/ { "GoCont",   0, 0, 0, 150,   0, 36, 26, WHITE, "GO"  , 0, 0, 0, 0, 0, "CONT",   1, 0, 0, 0 },
/*33*/ { "GoFinish", 0, 0, 0, 200,   0, 36, 26, WHITE, "GO"  , 0, 0, 0, 0, 0, "FINISH", 1, 0, 0, 0 },
/*34*/ { "GoReset" , 0, 0, 0, 250,   0, 36, 26, WHITE, "GO"  , 0, 0, 0, 0, 0, "RESET",  1, 0, 0, 0 },
/*35*/ { "GoStep",   0, 0, 0, 300,   0, 36, 26, WHITE, "GO"  , 0, 0, 0, 0, 0, "STEP",   1, 0, 0, 0 },
/*36*/ { "GoOver",   0, 0, 0, 350,   0, 36, 26, WHITE, "GO"  , 0, 0, 0, 0, 0, "OVER",   1, 0, 0, 0 },
/*37*/ { "GoIn",     0, 0, 0, 400,   0, 36, 26, WHITE, "GO"  , 0, 0, 0, 0, 0, "aIN",    1, 0, 0, 0 },
/*38*/ { "TermExit", 0, 0, 0, 500,   0, 36, 26, WHITE, "TERM", 0, 0, 0, 0, 0, "EXIT",   1, 0, 0, 0 },
/*39*/ { "TermQuit", 0, 0, 0, 550,   0, 36, 26, WHITE, "TERM", 0, 0, 0, 0, 0, "QUIT",   1, 0, 0, 0 },

/*40*/ { "Trace",    0, 0, 0,   0,  50, 36, 26, WHITE, "TRACE", 0, 0, 0, 0, 0, "START",   1, "STOP", 0, 0 },
/*41*/ { "BrkAddr",  0, 0, 0, 100,  50, 36, 26, WHITE, "BRK", 0, 0, 0, 0, 0, "ADDRESS", 1, 0, 0, 0 },
/*42*/ { "BrkRng",   0, 0, 0, 150,  50, 36, 26, WHITE, "BRK", 0, 0, 0, 0, 0, "RANGE", 1, 0, 0, 0 },
/*43*/ { "BrkRegr",  0, 0, 0, 200,  50, 36, 26, WHITE, "BRK", 0, 0, 0, 0, 0, "REGISTER", 1, 0, 0, 0 },
/*44*/ { "BrkInstr", 0, 0, 0, 250,  50, 36, 26, WHITE, "BRK", 0, 0, 0, 0, 0, "INSTRUCT", 1, 0, 0, 0 },
/*45*/ { "BrkRPL",   0, 0, 0, 300,  50, 36, 26, WHITE, "BRK", 0, 0, 0, 0, 0, "RPL", 1, 0, 0, 0 },
/*46*/ { "BrkClear", 0, 0, 0, 350,  50, 36, 26, WHITE, "BRK", 0, 0, 0, 0, 0, "CLEAR", 1, 0, 0, 0 },
/*47*/ { "BrkNow",   0, 0, 0, 400,  50, 36, 26, WHITE, "BRK", 0, 0, 0, 0, 0, "NOW", 1, 0, 0, 0 },

/* Memory Debugging */
/*48*/ { "MemUp1",   0, 0, 0,   0, 0, 36, 26, WHITE, 0, 0, up_width, up_height, up_bits, 0, "1",     1, 0, 0, 0 },
/*49*/ { "MemUp10",  0, 0, 0,  50, 0, 36, 26, WHITE, 0, 0, up_width, up_height, up_bits, 0, "10",    1, 0, 0, 0 },
/*50*/ { "MemUp100", 0, 0, 0, 100, 0, 36, 26, WHITE, 0, 0, up_width, up_height, up_bits, 0, "100",   1, 0, 0, 0 },
/*51*/ { "MemUp1k",  0, 0, 0, 150, 0, 36, 26, WHITE, 0, 0, up_width, up_height, up_bits, 0, "1000",  1, 0, 0, 0 },
/*52*/ { "MemUp10k", 0, 0, 0, 200, 0, 36, 26, WHITE, 0, 0, up_width, up_height, up_bits, 0, "10000", 1, 0, 0, 0 },
/*53*/ { "MemHome",  0, 0, 0, 250,   0, 36, 26, WHITE, "GOTO", 0, 0, 0, 0, 0, "HOME",    1, 0, 0, 0 },

/*54*/ { "MemDn1",   0, 0, 0,   0, 50, 36, 26, WHITE, 0, 0, down_width, down_height, down_bits, 0, "1", 1, 0, 0, 0 },
/*55*/ { "MemDn10",  0, 0, 0,  50, 50, 36, 26, WHITE, 0, 0, down_width, down_height, down_bits, 0, "10", 1, 0, 0, 0 },
/*56*/ { "MemDn100", 0, 0, 0, 100, 50, 36, 26, WHITE, 0, 0, down_width, down_height, down_bits, 0, "100", 1, 0, 0, 0 },
/*57*/ { "MemDn1k",  0, 0, 0, 150, 50, 36, 26, WHITE, 0, 0, down_width, down_height, down_bits, 0, "1000", 1, 0, 0, 0 },
/*58*/ { "MemDn10k", 0, 0, 0, 200, 50, 36, 26, WHITE, 0, 0, down_width, down_height, down_bits, 0, "10000", 1, 0, 0, 0 },

/*59*/ { "Mem A",    0, 0, 0,  00, 100, 36, 26, WHITE, "A",  0, 0, 0, 0, 0, "SEE", 0, "FOLL", 0, 0 },
/*60*/ { "Mem B",    0, 0, 0,  50, 100, 36, 26, WHITE, "B",  0, 0, 0, 0, 0, "SEE", 0, "FOLL", 0, 0 },
/*61*/ { "Mem C",    0, 0, 0, 100, 100, 36, 26, WHITE, "C",  0, 0, 0, 0, 0, "SEE", 0, "FOLL", 0, 0 },
/*62*/ { "Mem D",    0, 0, 0, 150, 100, 36, 26, WHITE, "D",  0, 0, 0, 0, 0, "SEE", 0, "FOLL", 0, 0 },
/*63*/ { "MemD0",    0, 0, 0, 200, 100, 36, 26, WHITE, "D0",  0, 0, 0, 0, 0, "SEE", 0, "FOLL", 0, 0 },
/*64*/ { "MemD1",    0, 0, 0, 250, 100, 36, 26, WHITE, "D1",  0, 0, 0, 0, 0, "SEE", 0, "FOLL", 0, 0 },

/*65*/ { "MemProlog",0, 0, 0,   0, 150, 36, 26, WHITE, "SHOW", 0, 0, 0, 0, 0, "PROLOG", 1, 0, 0, 0 },
/*66*/ { "MemAddr",  0, 0, 0,  50, 150, 36, 26, WHITE, "SHOW", 0, 0, 0, 0, 0, "ADDRESS", 1, 0, 0, 0 },
/*67*/ { "MemRPL",   0, 0, 0, 100, 150, 36, 26, WHITE, "SHOW", 0, 0, 0, 0, 0, "RPL", 1, 0, 0, 0 },
/*68*/ { "MemSRPL",  0, 0, 0, 150, 150, 36, 26, WHITE, "SHOW", 0, 0, 0, 0, 0, "SYSRPL", 1, 0, 0, 0 },
/*69*/ { "MemFind",  0, 0, 0, 200, 150, 36, 26, WHITE, "FIND", 0, 0, 0, 0, 0, "VALUE", 1, 0, 0, 0 },
/*70*/ { "MemGoto",  0, 0, 0, 250, 150, 36, 26, WHITE, "GOTO", 0, 0, 0, 0, 0, "ADDRESS", 1, 0, 0, 0 },

/* Objects Menu */
/*71*/ { "LkHome",   0, 0, 0,   0, 0, 36, 26, WHITE, "LOOK", 0, 0, 0, 0, 0, "HOME", 1, 0, 0, 0 },
/*72*/ { "LkStack",  0, 0, 0,  50, 0, 36, 26, WHITE, "LOOK", 0, 0, 0, 0, 0, "STACK", 1, 0, 0, 0 },
/*73*/ { "LkPort0",  0, 0, 0, 100, 0, 36, 26, WHITE, "LOOK", 0, 0, 0, 0, 0, "PORT 0", 1, 0, 0, 0 },
/*74*/ { "LkPort1",  0, 0, 0, 150, 0, 36, 26, WHITE, "LOOK", 0, 0, 0, 0, 0, "PORT 1", 1, 0, 0, 0 },
/*75*/ { "LkPort2",  0, 0, 0, 200, 0, 36, 26, WHITE, "LOOK", 0, 0, 0, 0, 0, "PORT 2", 1, 0, 0, 0 },

/*76*/ { "Send",  0, 0, 0,  0,  50, 36, 26, WHITE, 0, 0, menu_label_width, menu_label_height, menu_label_bits, 0, "SEND", 1, 0, 0, 0 },
/*77*/ { "Recv",  0, 0, 0, 50,  50, 36, 26, WHITE, 0, 0, menu_label_width, menu_label_height, menu_label_bits, 0, "RECEIVE", 1, 0, 0, 0 },
/*78*/ { "Break", 0, 0, 0,100,  50, 36, 26, WHITE, 0, 0, menu_label_width, menu_label_height, menu_label_bits, 0, "BREAK", 1, 0, 0, 0 },

/*79*/  { 0 }

};
#endif

#endif /* !_BUTTONS_H */
