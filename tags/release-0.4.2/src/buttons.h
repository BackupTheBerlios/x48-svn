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

#endif /* !_BUTTONS_H */
