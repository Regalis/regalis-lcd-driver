/*
* 
* Copyright (C) Patryk Jaworski <regalis@regalis.com.pl>
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* 
*/

#ifndef __REGALIS_LCD_
#define __REGALIS_LCD_
#include <inttypes.h>

/* INSTRUCTIONS (can be executed manually with regalis_lcd_instruction()) */
#define RL_CLEAR_DISPLAY 0x01
#define RL_RETURN_HOME 0x02
#define RL_DISPLAY_ON_OFF(ON_OFF, CURSOR, BLINK) 0x08 | ON_OFF | CURSOR | BLINK
	#define RL_DISPLAY_ON 0x03
	#define RL_DISPLAY_OFF 0x00
	#define RL_CURSOR_ON 0x02
	#define RL_CURSOR_OFF 0x00
	#define RL_CURSOR_BLINK_ON 0x01
	#define RL_CURSOR_BLINK_OFF 0x00
#define RL_ENTRY_MODE_SET(INC, SHIFT) 0x03 | INC | SHIFT
	#define RL_INCREMENT 0x02
	#define RL_DECREMENT 0x00
	#define RL_SHIFT_ON 0x01
	#define RL_SHIFT_OFF 0x00


void regalis_lcd_init();
void regalis_lcd_clear();
void regalis_lcd_instruction(uint8_t rl_instruction);

#endif
