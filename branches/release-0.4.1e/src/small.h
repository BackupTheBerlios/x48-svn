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

/* $Log: small.h,v $
 * Revision 1.5  1995/01/11  18:20:01  ecd
 * major update to support HP48 G/GX
 *
 * Revision 1.4  1994/12/07  20:16:41  ecd
 * completed small font
 *
 * Revision 1.4  1994/12/07  20:16:41  ecd
 * completed small font
 *
 * Revision 1.3  1994/11/02  14:51:27  ecd
 * minor changes
 *
 * Revision 1.2  1994/10/06  16:28:03  ecd
 * changed char to unsigned
 *
 * Revision 1.1  1994/09/13  15:05:11  ecd
 * Initial revision
 *
 *
 * $Id: small.h,v 1.5 1995/01/11 18:20:01 ecd Exp ecd $
 */

#ifndef _SMALL_H
#define _SMALL_H 1

#define small_ascent 8
#define small_descent 4

#define blank_width 4
#define blank_height 7
static unsigned char blank_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define hash_width 5
#define hash_height 7
static unsigned char hash_bits[] = {
   0x00, 0x0a, 0x1f, 0x0a, 0x0a, 0x1f, 0x0a};

#define lbrace_width 3
#define lbrace_height 7
static unsigned char lbrace_bits[] = {
   0x04, 0x02, 0x01, 0x01, 0x01, 0x02, 0x04};

#define rbrace_width 3
#define rbrace_height 7
static unsigned char rbrace_bits[] = {
   0x01, 0x02, 0x04, 0x04, 0x04, 0x02, 0x01};

#define comma_width 3
#define comma_height 7
static unsigned char comma_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x03};

#define slash_width 3
#define slash_height 7
static unsigned char slash_bits[] = {
   0x04, 0x04, 0x02, 0x02, 0x02, 0x01, 0x01};

#define two_width 5
#define two_height 7
static unsigned char two_bits[] = {
   0x0e, 0x11, 0x10, 0x08, 0x04, 0x02, 0x1f};

#define three_width 5
#define three_height 7
static unsigned char three_bits[] = {
   0x0e, 0x11, 0x10, 0x0c, 0x10, 0x11, 0x0e};

#define small_colon_width 2
#define small_colon_height 7
static unsigned char small_colon_bits[] = {
   0x00, 0x03, 0x03, 0x00, 0x03, 0x03, 0x00};

#define A_width 5
#define A_height 7
static unsigned char A_bits[] = {
   0x0e, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11};

#define B_width 5
#define B_height 7
static unsigned char B_bits[] = {
   0x0f, 0x11, 0x11, 0x0f, 0x11, 0x11, 0x0f};

#define C_width 5
#define C_height 7
static unsigned char C_bits[] = {
   0x0e, 0x11, 0x01, 0x01, 0x01, 0x11, 0x0e};

#define D_width 5
#define D_height 7
static unsigned char D_bits[] = {
   0x0f, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0f};

#define E_width 5
#define E_height 7
static unsigned char E_bits[] = {
   0x1f, 0x01, 0x01, 0x0f, 0x01, 0x01, 0x1f};

#define F_width 5
#define F_height 7
static unsigned char F_bits[] = {
   0x1f, 0x01, 0x01, 0x0f, 0x01, 0x01, 0x01};

#define G_width 5
#define G_height 7
static unsigned char G_bits[] = {
   0x0e, 0x11, 0x01, 0x01, 0x19, 0x11, 0x0e};

#define H_width 5
#define H_height 7
static unsigned char H_bits[] = {
   0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11};

#define I_width 1
#define I_height 7
static unsigned char I_bits[] = {
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};

#define J_width 4
#define J_height 7
static unsigned char J_bits[] = {
   0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0x06};

#define K_width 5
#define K_height 7
static unsigned char K_bits[] = {
   0x11, 0x09, 0x05, 0x03, 0x05, 0x09, 0x11};

#define L_width 4
#define L_height 7
static unsigned char L_bits[] = {
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x0f};

#define M_width 5
#define M_height 7
static unsigned char M_bits[] = {
   0x11, 0x1b, 0x15, 0x11, 0x11, 0x11, 0x11};

#define N_width 5
#define N_height 7
static unsigned char N_bits[] = {
   0x11, 0x11, 0x13, 0x15, 0x19, 0x11, 0x11};

#define O_width 5
#define O_height 7
static unsigned char O_bits[] = {
   0x0e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e};

#define P_width 5
#define P_height 7
static unsigned char P_bits[] = {
   0x0f, 0x11, 0x11, 0x0f, 0x01, 0x01, 0x01};

#define Q_width 5
#define Q_height 7
static unsigned char Q_bits[] = {
   0x0e, 0x11, 0x11, 0x11, 0x15, 0x09, 0x16};

#define R_width 5
#define R_height 7
static unsigned char R_bits[] = {
   0x0f, 0x11, 0x11, 0x0f, 0x05, 0x09, 0x11};

#define S_width 5
#define S_height 7
static unsigned char S_bits[] = {
   0x0e, 0x11, 0x01, 0x0e, 0x10, 0x11, 0x0e};

#define T_width 5
#define T_height 7
static unsigned char T_bits[] = {
   0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};

#define U_width 5
#define U_height 7
static unsigned char U_bits[] = {
   0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e};

#define V_width 5
#define V_height 7
static unsigned char V_bits[] = {
   0x11, 0x11, 0x11, 0x11, 0x0a, 0x0a, 0x04};

#define W_width 5
#define W_height 7
static unsigned char W_bits[] = {
   0x11, 0x11, 0x11, 0x11, 0x15, 0x1b, 0x11};

#define X_width 5
#define X_height 7
static unsigned char X_bits[] = {
   0x11, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x11};

#define Y_width 5
#define Y_height 7
static unsigned char Y_bits[] = {
   0x11, 0x11, 0x0a, 0x04, 0x04, 0x04, 0x04};

#define Z_width 5
#define Z_height 7
static unsigned char Z_bits[] = {
   0x1f, 0x10, 0x08, 0x04, 0x02, 0x01, 0x1f};

#define lbracket_width 3
#define lbracket_height 7
static unsigned char lbracket_bits[] = {
   0x07, 0x01, 0x01, 0x01, 0x01, 0x01, 0x07};

#define rbracket_width 3
#define rbracket_height 7
static unsigned char rbracket_bits[] = {
   0x07, 0x04, 0x04, 0x04, 0x04, 0x04, 0x07};

#define arrow_width 7
#define arrow_height 7
static unsigned char arrow_bits[] = {
   0x00, 0x08, 0x18, 0x3f, 0x18, 0x08, 0x00};

#define diff_width 5
#define diff_height 7
static unsigned char diff_bits[] = {
   0x0e, 0x10, 0x10, 0x1e, 0x11, 0x11, 0x0e};

#define integral_width 5
#define integral_height 8
static unsigned char integral_bits[] = {
   0x0c, 0x12, 0x02, 0x04, 0x04, 0x08, 0x09, 0x06};

#define sigma_width 6
#define sigma_height 9
static unsigned char sigma_bits[] = {
   0x3f, 0x21, 0x02, 0x04, 0x08, 0x04, 0x02, 0x21, 0x3f};

#define sqr_width 11
#define sqr_height 10
static unsigned char sqr_bits[] = {
   0x00, 0x03, 0x80, 0x04, 0x00, 0x04, 0x00, 0x02, 0x26, 0x01, 0x94, 0x07,
   0x08, 0x00, 0x14, 0x00, 0x53, 0x00, 0x21, 0x00};

#define root_width 18
#define root_height 13
static unsigned char root_bits[] = {
   0x26, 0x00, 0x00, 0x14, 0x00, 0x00, 0x08, 0xfe, 0x03, 0x14, 0x02, 0x02,
   0x53, 0x02, 0x00, 0x21, 0x99, 0x00, 0x00, 0x91, 0x00, 0x10, 0x91, 0x00,
   0xa0, 0x50, 0x00, 0xc0, 0x60, 0x00, 0x80, 0x20, 0x00, 0x00, 0x14, 0x00,
   0x00, 0x0c, 0x00};

#define pow10_width 13
#define pow10_height 9
static unsigned char pow10_bits[] = {
   0x00, 0x12, 0x00, 0x0c, 0x32, 0x04, 0x4b, 0x0a, 0x4a, 0x09, 0x4a, 0x00,
   0x4a, 0x00, 0x4a, 0x00, 0x32, 0x00};

#define exp_width 11
#define exp_height 9
static unsigned char exp_bits[] = {
   0x80, 0x04, 0x00, 0x03, 0x00, 0x01, 0x8c, 0x02, 0x52, 0x02, 0x09, 0x00,
   0x07, 0x00, 0x21, 0x00, 0x1e, 0x00};

#define under_width 6
#define under_height 7
static unsigned char under_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f};

#define prog_width 16
#define prog_height 7
static unsigned char prog_bits[] = {
   0x48, 0x12, 0x24, 0x24, 0x12, 0x48, 0x09, 0x90, 0x12, 0x48, 0x24, 0x24,
   0x48, 0x12};

#define string_width 10
#define string_height 7
static unsigned char string_bits[] = {
   0x85, 0x02, 0x85, 0x02, 0x85, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00};

#define equal_width 5
#define equal_height 7
static unsigned char equal_bits[] = {
   0x00, 0x1f, 0x00, 0x00, 0x1f, 0x00, 0x00};

#define nl_width 8
#define nl_height 7
static unsigned char nl_bits[] = {
   0x00, 0x84, 0x86, 0xff, 0x06, 0x04, 0x00};

#define pi_width 6
#define pi_height 7
static unsigned char pi_bits[] = {
   0x20, 0x1f, 0x12, 0x12, 0x12, 0x12, 0x12};

#define angle_width 8
#define angle_height 7
static unsigned char angle_bits[] = {
   0x40, 0x20, 0x10, 0x28, 0x44, 0x42, 0xff};

#define lcurly_width 5
#define lcurly_height 7
static unsigned char lcurly_bits[] = {
   0x18, 0x04, 0x04, 0x02, 0x04, 0x04, 0x18};

#define rcurly_width 5
#define rcurly_height 7
static unsigned char rcurly_bits[] = {
   0x03, 0x04, 0x04, 0x08, 0x04, 0x04, 0x03};

#define sqr_gx_width 11
#define sqr_gx_height 13
static unsigned char sqr_gx_bits[] = {
   0x00, 0x03, 0x80, 0x04, 0x00, 0x04, 0x00, 0x02, 0x00, 0x01, 0x80, 0x07,
   0x00, 0x00, 0x66, 0x00, 0x14, 0x00, 0x08, 0x00, 0x14, 0x00, 0x53, 0x00,
   0x21, 0x00};

#define root_gx_width 18
#define root_gx_height 15
static unsigned char root_gx_bits[] = {
   0x66, 0x00, 0x00, 0x14, 0x00, 0x00, 0x08, 0x00, 0x00, 0x14, 0x00, 0x00,
   0x53, 0xfe, 0x03, 0x21, 0x02, 0x02, 0x00, 0x02, 0x00, 0x00, 0x99, 0x00,
   0x00, 0x91, 0x00, 0x10, 0x91, 0x00, 0xa0, 0x50, 0x00, 0xc0, 0x60, 0x00,
   0x80, 0x20, 0x00, 0x00, 0x14, 0x00, 0x00, 0x0c, 0x00};

#define pow10_gx_width 13
#define pow10_gx_height 12
static unsigned char pow10_gx_bits[] = {
   0x00, 0x12, 0x00, 0x0c, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x09, 0x32, 0x00,
   0x4b, 0x00, 0x4a, 0x00, 0x4a, 0x00, 0x4a, 0x00, 0x4a, 0x00, 0x32, 0x00};

#define exp_gx_width 11
#define exp_gx_height 11
static unsigned char exp_gx_bits[] = {
   0x80, 0x04, 0x00, 0x03, 0x00, 0x01, 0x80, 0x02, 0x40, 0x02, 0x0c, 0x00,
   0x12, 0x00, 0x09, 0x00, 0x07, 0x00, 0x21, 0x00, 0x1e, 0x00};

#define parens_gx_width 20
#define parens_gx_height 12
static unsigned char parens_gx_bits[] = {
   0x0c, 0x00, 0x03, 0x06, 0x00, 0x06, 0x06, 0x00, 0x06, 0x03, 0x00, 0x0c,
   0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c,
   0x03, 0x00, 0x0c, 0x06, 0x00, 0x06, 0x06, 0x00, 0x06, 0x0c, 0x00, 0x03};

#define hash_gx_width 8
#define hash_gx_height 12
static unsigned char hash_gx_bits[] = {
   0x00, 0x00, 0x48, 0x48, 0xfe, 0x24, 0x24, 0x7f, 0x12, 0x12, 0x00, 0x00};

#define bracket_gx_width 12
#define bracket_gx_height 12
static unsigned char bracket_gx_bits[] = {
   0x0f, 0x0f, 0x03, 0x0c, 0x03, 0x0c, 0x03, 0x0c, 0x03, 0x0c, 0x03, 0x0c,
   0x03, 0x0c, 0x03, 0x0c, 0x03, 0x0c, 0x03, 0x0c, 0x03, 0x0c, 0x0f, 0x0f};

#define under_gx_width 10
#define under_gx_height 12
static unsigned char under_gx_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0xff, 0x03};

#define prog_gx_width 24
#define prog_gx_height 12
static unsigned char prog_gx_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xc3, 0x18,
   0x8c, 0x81, 0x31, 0xc6, 0x00, 0x63, 0x63, 0x00, 0xc6, 0xc6, 0x00, 0x63,
   0x8c, 0x81, 0x31, 0x18, 0xc3, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define quote_gx_width 12
#define quote_gx_height 12
static unsigned char quote_gx_bits[] = {
   0x05, 0x0a, 0x05, 0x0a, 0x05, 0x0a, 0x05, 0x0a, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define curly_gx_width 14
#define curly_gx_height 12
static unsigned char curly_gx_bits[] = {
   0x0c, 0x0c, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x03, 0x30,
   0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x0c, 0x0c};

#define colon_gx_width 8
#define colon_gx_height 12
static unsigned char colon_gx_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0xc3, 0x00, 0x00, 0xc3, 0xc3, 0x00};

#define angle_gx_width 12
#define angle_gx_height 12
static unsigned char angle_gx_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x01, 0xc0, 0x00, 0xe0, 0x01,
   0xb0, 0x03, 0x18, 0x03, 0x0c, 0x03, 0x06, 0x03, 0xff, 0x0f, 0xff, 0x0f};

#define pi_gx_width 10
#define pi_gx_height 12
static unsigned char pi_gx_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x03, 0xff, 0x01,
   0xcc, 0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc, 0x00};

#define nl_gx_width 18
#define nl_gx_height 12
static unsigned char nl_gx_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x03, 0xf0, 0x00, 0x03,
   0xfc, 0x00, 0x03, 0xff, 0xff, 0x03, 0xff, 0xff, 0x03, 0xfc, 0x00, 0x00,
   0xf0, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define comma_gx_width 3
#define comma_gx_height 12
static unsigned char comma_gx_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x04, 0x04, 0x02};

#define arrow_gx_width 18
#define arrow_gx_height 12
static unsigned char arrow_gx_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x3c, 0x00,
   0x00, 0xfc, 0x00, 0xff, 0xff, 0x03, 0xff, 0xff, 0x03, 0x00, 0xfc, 0x00,
   0x00, 0x3c, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define equal_gx_width 8
#define equal_gx_height 12
static unsigned char equal_gx_bits[] = {
   0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00};

typedef struct letter_t {
  unsigned int   w, h;
  unsigned char *bits;
} letter_t;

letter_t small_font[] = {
  { 0, 0, 0 },
  { nl_gx_width, nl_gx_height, nl_gx_bits },		/* \001 == \n gx */
  { comma_gx_width, comma_gx_height, comma_gx_bits },	/* \002 == comma gx */
  { arrow_gx_width, arrow_gx_height, arrow_gx_bits },	/* \003 == \-> gx */
  { equal_gx_width, equal_gx_height, equal_gx_bits },	/* \004 == equal gx */
  { pi_gx_width, pi_gx_height, pi_gx_bits },		/* \005 == pi gx */
  { 0, 0, 0 },                                          /* \006          */
  { 0, 0, 0 },                                          /* \007          */
  { 0, 0, 0 },                                          /* \008          */
  { 0, 0, 0 },                                          /* \009          */
  { 0, 0, 0 },                                          /* \010          */
  { 0, 0, 0 },                                          /* \011          */
  { 0, 0, 0 },                                          /* \012          */
  { 0, 0, 0 },                                          /* \013          */
  { 0, 0, 0 },                                          /* \014          */
  { 0, 0, 0 },                                          /* \015          */
  { 0, 0, 0 },	/* # 16 */
  { 0, 0, 0 },                                          /* \017          */
  { 0, 0, 0 },                                          /* \018          */
  { 0, 0, 0 },                                          /* \019          */
  { 0, 0, 0 },                                          /* \020          */
  { 0, 0, 0 },                                          /* \021          */
  { 0, 0, 0 },                                          /* \022          */
  { 0, 0, 0 },                                          /* \023          */
  { 0, 0, 0 },                                          /* \024          */
  { 0, 0, 0 },                                          /* \025          */
  { 0, 0, 0 },                                          /* \026          */
  { 0, 0, 0 },                                          /* \027          */
  { 0, 0, 0 },                                          /* \028          */
  { 0, 0, 0 },                                          /* \029          */
  { 0, 0, 0 },                                          /* \030          */
  { 0, 0, 0 },                                          /* \031          */
  { blank_width, blank_height, blank_bits },	        /* \032   ' '    */
  { 0, 0, 0 },                                          /* \033          */
  { 0, 0, 0 },                                          /* \034          */
  { hash_width, hash_height, hash_bits },               /* \035   '#'    */
  { 0, 0, 0 },                                          /* \036          */
  { 0, 0, 0 },                                          /* \037          */
  { 0, 0, 0 },                                          /* \038          */
  { 0, 0, 0 },                                          /* \039          */
  { lbrace_width, lbrace_height, lbrace_bits },         /* \040   '}'    */
  { rbrace_width, rbrace_height, rbrace_bits },         /* \041   '{'    */
  { 0, 0, 0 },                                          /* \042          */
  { 0, 0, 0 },                                          /* \043          */
  { comma_width, comma_height, comma_bits },            /* \044   ','    */
  { 0, 0, 0 },                                          /* \045          */
  { 0, 0, 0 },                                          /* \046          */
  { slash_width, slash_height, slash_bits },            /* \047   '\'    */
  { O_width, O_height, O_bits },	                /* \048   '0'    */
  { I_width, I_height, I_bits },                        /* \049   '1'    */
  { two_width, two_height, two_bits },
  { three_width, three_height, three_bits },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { small_colon_width, small_colon_height, small_colon_bits },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { equal_width, equal_height, equal_bits },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },	/* # 64 */
  { A_width, A_height, A_bits },
  { B_width, B_height, B_bits },
  { C_width, C_height, C_bits },
  { D_width, D_height, D_bits },
  { E_width, E_height, E_bits },
  { F_width, F_height, F_bits },
  { G_width, G_height, G_bits },
  { H_width, H_height, H_bits },
  { I_width, I_height, I_bits },
  { J_width, J_height, J_bits },
  { K_width, K_height, K_bits },
  { L_width, L_height, L_bits },
  { M_width, M_height, M_bits },
  { N_width, N_height, N_bits },
  { O_width, O_height, O_bits },
  { P_width, P_height, P_bits },	/* # 80 */
  { Q_width, Q_height, Q_bits },
  { R_width, R_height, R_bits },
  { S_width, S_height, S_bits },
  { T_width, T_height, T_bits },
  { U_width, U_height, U_bits },
  { V_width, V_height, V_bits },
  { W_width, W_height, W_bits },
  { X_width, X_height, X_bits },
  { Y_width, Y_height, Y_bits },
  { Z_width, Z_height, Z_bits },
  { lbracket_width, lbracket_height, lbracket_bits },
  { 0, 0, 0 },
  { rbracket_width, rbracket_height, rbracket_bits },
  { 0, 0, 0 },
  { under_width, under_height, under_bits },
  { 0, 0, 0 },	/* # 96 */
  { arrow_width, arrow_height, arrow_bits },		/* a == left arrow   */
  { diff_width, diff_height, diff_bits },		/* b == differential */
  { integral_width, integral_height, integral_bits },	/* c == integral */
  { sigma_width, sigma_height, sigma_bits },		/* d == sigma */
  { sqr_width, sqr_height, sqr_bits },			/* e == sqr */
  { root_width, root_height, root_bits },		/* f == root */
  { pow10_width, pow10_height, pow10_bits },		/* g == pow10 */
  { exp_width, exp_height, exp_bits },			/* h == exp */
  { prog_width, prog_height, prog_bits },		/* i == << >> */
  { string_width, string_height, string_bits },		/* j == " " */
  { nl_width, nl_height, nl_bits },			/* k == New Line */
  { pi_width, pi_height, pi_bits },			/* l == pi */
  { angle_width, angle_height, angle_bits },		/* m == angle */
  { sqr_gx_width, sqr_gx_height, sqr_gx_bits },		/* n == sqr gx */
  { root_gx_width, root_gx_height, root_gx_bits },	/* o == root gx */
  { pow10_gx_width, pow10_gx_height, pow10_gx_bits },	/* p == pow10 gx */
  { exp_gx_width, exp_gx_height, exp_gx_bits },		/* q == exp gx */
  { parens_gx_width, parens_gx_height, parens_gx_bits },/* r == ( ) gx */
  { hash_gx_width, hash_gx_height, hash_gx_bits },	/* s == # gx */
  { bracket_gx_width, bracket_gx_height, bracket_gx_bits }, /* t == [] gx */
  { under_gx_width, under_gx_height, under_gx_bits },	/* u == _ gx */
  { prog_gx_width, prog_gx_height, prog_gx_bits },	/* v == << >> gx */
  { quote_gx_width, quote_gx_height, quote_gx_bits },	/* w == " " gx */
  { curly_gx_width, curly_gx_height, curly_gx_bits },	/* x == {} gx */
  { colon_gx_width, colon_gx_height, colon_gx_bits },	/* y == :: gx */
  { angle_gx_width, angle_gx_height, angle_gx_bits },	/* z == angle gx */
  { lcurly_width, lcurly_height, lcurly_bits },
  { 0, 0, 0 },
  { rcurly_width, rcurly_height, rcurly_bits },
  { 0, 0, 0 },
  { 0, 0, 0 }
};

#endif /* !_SMALL_H */
