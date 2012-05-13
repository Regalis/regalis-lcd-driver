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
	#define RL_DISPLAY_ON 0x04
	#define RL_DISPLAY_OFF 0x00
	#define RL_CURSOR_ON 0x02
	#define RL_CURSOR_OFF 0x00
	#define RL_CURSOR_BLINK_ON 0x01
	#define RL_CURSOR_BLINK_OFF 0x00
#define RL_ENTRY_MODE_SET(INC, SHIFT) 0x04 | INC | SHIFT
	#define RL_INCREMENT 0x02
	#define RL_DECREMENT 0x00
	#define RL_SHIFT_ON 0x01
	#define RL_SHIFT_OFF 0x00
#define RL_FUNCTION_SET(MODE, LINES, FONT) 0x20 | MODE | LINES | FONT
	#define RL_MODE_4BIT 0x00
	#define RL_MODE_8BIT 0x10
	#define RL_LINES_1 0x00
	#define RL_LINES_2 0x08
	#define RL_FONT_5x8 0x00
	#define RL_FONT_5x11 0x04
#define RL_DDRAM_SET(ADDR) 0x80 | ((ADDR) & 0x7F)
	#define RL_LINE_1 0x00
	#define RL_LINE_2 0x40
#define RL_SHIFT_CURSOR(DIRECTION) 0x10 | (DIRECTION)
	#define RL_CURSOR_RIGHT 0x4
	#define RL_CURSOR_LEFT 0x0
#define RL_SHIFT_DISPLAY(DIRECTION) 0x10 | (DIRECTION)
	#define RL_DISPLAY_RIGHT 0xC
	#define RL_DISPLAY_LEFT 0x8

/* Defines - function arguments */
#define RL_READ_BUSY 0xFF
#define RL_READ_DATA 0x00


/** Initialize LCD */
void regalis_lcd_init();

/** Clear display */
void regalis_lcd_clear();

/** Return to home (first line, first character) */
void regalis_lcd_home();

/** Read data from LCD
 * @param register_select read busy flag (bit 7) and address counter (bits 0-6)
 * or DDRAM address. Possible values:
 * 	\b RL_READ_BUSY
 * 	\b RL_READ_DATA
 * 	@return unsigned 8-bits value
 */
uint8_t regalis_lcd_read(uint8_t register_select);

/** Execute instruction
 * @param rl_instruction instruction to execute, should be one of:
 *  \b RL_CLEAR_DISPLAY
 *  \b RL_RETURN_HOME
 *  \b RL_DISPLAY_ON_OFF(ON_OFF, CURSOR, BLINK)
 *  \b RL_ENTRY_MODE_SET(INC, SHIFT)
 *  \b RL_FUNCTION_SET(MODE, LINES, FONT) 
 */
void regalis_lcd_instruction(uint8_t rl_instruction);

/** Send single character to LCD
 * @param character ASCII character
 */
void regalis_lcd_putc(char character);

/** Send string to LCD
 * @param string C string terminated with '\0'
 */
void regalis_lcd_puts(const char* string);

/** Move cursor to (x, y) position
 * @param x horizontal position
 * @param y vertical position
 */
void regalis_lcd_goto(uint8_t x, uint8_t y);

/** Move cursor to specified address
 * @param addr 6 bits address
 */
void regalis_lcd_goto_addr(uint8_t addr);

#endif

