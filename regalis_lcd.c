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

#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "regalis_lcd.h"
#include "regalis_lcd_config.h"

#define STR(X) #X
#define GET_DDR(X, Y) DDR ## X
#define GET_PORT(X, Y) PORT ## X
#define GET_PIN(X, Y) Y
#define DDR(X) GET_DDR(X)
#define PORT(X) GET_PORT(X)
#define PIN(X) GET_PIN(X)

#define RL_RS_DDR DDR(REGALIS_LCD_RS)
#define RL_RS_PORT PORT(REGALIS_LCD_RS)
#define RL_RS_PIN PIN(REGALIS_LCD_RS)

#define RL_RW_DDR DDR(REGALIS_LCD_RW)
#define RL_RW_PORT PORT(REGALIS_LCD_RW)
#define RL_RW_PIN PIN(REGALIS_LCD_RW)

#define RL_E_DDR DDR(REGALIS_LCD_RW)
#define RL_E_PORT PORT(REGALIS_LCD_RW)
#define RL_E_PIN PIN(REGALIS_LCD_RW)

#define RL_D4_DDR DDR(REGALIS_LCD_D4)
#define RL_D4_PORT PORT(REGALIS_LCD_D4)
#define RL_D4_PIN PIN(REGALIS_LCD_D4)

#define RL_D5_DDR DDR(REGALIS_LCD_D5)
#define RL_D5_PORT PORT(REGALIS_LCD_D5)
#define RL_D5_PIN PIN(REGALIS_LCD_D5)

#define RL_D6_DDR DDR(REGALIS_LCD_D6)
#define RL_D6_PORT PORT(REGALIS_LCD_D6)
#define RL_D6_PIN PIN(REGALIS_LCD_D6)

#define RL_D7_DDR DDR(REGALIS_LCD_D7)
#define RL_D7_PORT PORT(REGALIS_LCD_D7)
#define RL_D7_PIN PIN(REGALIS_LCD_D7)

#if REGALIS_LCD_LINES == 1
	#define RL_LINES 0x0
#elif REGALIS_LCD_LINES == 2
	#define RL_LINES 0x8
#else
	#error invalid value for REGALIS_LCD_LINES (type 1 or 2)
#endif

#if REGALIS_LCD_FONT == 0
	#define RL_FONT 0x0
#elif REGALIS_LCD_FONT == 1
	#define RL_FONT 0x4
#else
	#error invalid value for REGALIS_LCD_FONT (select "5x8" or "5x11")
#endif

#if REGALIS_LCD_CURSOR == 1
	#define RL_CURSOR RL_CURSOR_ON
#elif REGALIS_LCD_CURSOR == 0
	#define RL_CURSOR RL_CURSOR_OFF
#else
	#error invalid value for REGALIS_LCD_CURSOR (select 0 or 1)
#endif

#if REGALIS_LCD_CURSOR_BLINK == 1
	#define RL_CURSOR_BLINK RL_CURSOR_BLINK_ON
#elif REGALIS_LCD_CURSOR == 0
	#define RL_CURSOR_BLINK RL_CURSOR_BLINK_OFF
#else
	#error invalid value for REGALIS_LCD_CURSOR_BLINK (select 0 or 1)
#endif

#if REGALIS_LCD_DDRAM_BH == 1
	#define RL_DDRAM_BH RL_INCREMENT
#elif REGALIS_LCD_DDRAM_BH == 0
	#define RL_DDRAM_BH RL_DECREMENT
#else
	#error invalid value for REGALIS_LCD_DDRAM_BH (select 0 or 1)
#endif

#if REGALIS_LCD_SHIFT == 1
	#define RL_SHIFT RL_SHIFT_ON
#elif REGALIS_LCD_SHIFT == 0
	#define RL_SHIFT RL_SHIFT_OFF
#else
	#error invalid value for REGALIS_LCD_SHIFT (select 0 or 1)
#endif

/* Protected instructions */
#define RL_FUNCTION_SET(LINES, FONT) 0x20 | LINES | FONT

static inline void regalis_lcd_enable();
void regalis_lcd_exec(uint8_t command);


void regalis_lcd_init() {
	_delay_ms(40); // wait for power stabilisation
	regalis_lcd_exec(RL_FUNCTION_SET(RL_LINES, RL_FONT) >> 4); // set 4-bit interface mode
	_delay_ms(4);
	// now exec instructions in 4-bit mode
	regalis_lcd_instruction(RL_FUNCTION_SET(RL_LINES, RL_FONT));
	_delay_us(45);
	regalis_lcd_instruction(RL_DISPLAY_ON_OFF(RL_DISPLAY_ON, RL_CURSOR, RL_CURSOR_BLINK));
	_delay_us(45);
	regalis_lcd_instruction(RL_CLEAR_DISPLAY);
	_delay_ms(2);
	regalis_lcd_instruction(RL_ENTRY_MODE_SET(RL_DDRAM_BH, RL_SHIFT));
	_delay_us(100);
}

/** Execute low-level command
 * @param command is 8-bit value \
 *
 * | X | X | RS | RW | D7 | D6 | D5 | D4 |
 *   7   6   5    4    4    2    1    0
 */
void regalis_lcd_exec(uint8_t command) {
	RL_RS_DDR |= _BV(RL_RS_PIN);
	RL_RW_DDR |= _BV(RL_RW_PIN);
	RL_D7_DDR |= _BV(RL_D7_PIN);
	RL_D6_DDR |= _BV(RL_D6_PIN);
	RL_D5_DDR |= _BV(RL_D5_PIN);
	RL_D4_DDR |= _BV(RL_D4_PIN);

	RL_RS_PORT &= ~(_BV(RL_RS_PIN));
	RL_RS_PORT |= ((command & _BV(5)) >> (5 - RL_RS_PIN));
	RL_RW_PORT &= ~(_BV(RL_RW_PIN));
	RL_RW_PORT |= ((command & _BV(4)) >> (4 - RL_RW_PIN));
	RL_D7_PORT &= ~(_BV(RL_D7_PORT));
	RL_D7_PORT |= ((command & _BV(3)) >> (3 - RL_D7_PIN));
	RL_D6_PORT &= ~(_BV(RL_D6_PORT));
	RL_D6_PORT |= ((command & _BV(2)) >> (2 - RL_D6_PIN));
	RL_D5_PORT &= ~(_BV(RL_D6_PORT));
	RL_D5_PORT |= ((command & _BV(1)) >> (1 - RL_D5_PIN));
	RL_D4_PORT &= ~(_BV(RL_D4_PORT));
	RL_D4_PORT |= ((command & _BV(0)) >> (0 - RL_D4_PIN));

	regalis_lcd_enable();
}

inline void regalis_lcd_instruction(uint8_t instruction) {
	regalis_lcd_exec(0x0 | (instruction >> 4));
	_delay_us(100);
	regalis_lcd_exec(0x0 | (instruction & 0x0F));
}

static inline void regalis_lcd_enable() {
	RL_E_DDR |= _BV(RL_E_PIN);
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		RL_E_PORT |= _BV(RL_E_PIN);
		_delay_us(1);
		RL_E_PORT &= ~(_BV(RL_E_PIN));
	}
}
