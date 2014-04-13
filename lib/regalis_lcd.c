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

#include <inttypes.h>

#include "stm32f4xx.h"

#include "regalis_lcd.h"
#include "delay.h"

#define _BV(X) (1 << (X))

#define STR(X) #X
#define GET_MODER(X) X->MODER
#define GET_OTYPER(X) X->OTYPER
#define GET_OSPEEDR(X) X->OSPEEDR
#define GET_PUPDR(X) X->PUPDR
#define GET_ODR(X) X->ODR
#define GET_IN(X) X->IDR
#define GET_OUT_SET(X) X->BSRRL
#define GET_OUT_CLEAR(X) X->BSRRH
#define GET_PIN(X, Y) Y
#define GET_GPIO(X, Y) GPIO ## X

#define MODER(X) GET_MODER(GET_GPIO(X))
#define OTYPER(X) GET_OTYPER(GET_GPIO(X))
#define OSPEEDR(X) GET_SPEEDR(GET_GPIO(X))
#define PUPDR(X) GET_PUPDR(GET_GPIO(X))
#define ODR(X) GET_ODR(GET_GPIO(X))
#define OUT_SET(X) GET_OUT_SET(GET_GPIO(X))
#define OUT_CLEAR(X) GET_OUT_CLEAR(GET_GPIO(X))
#define PIN(X) GET_PIN(X)
#define IN(X) GET_IN(GET_GPIO(X))
#define GPIO(X) GET_GPIO(X)

#define set_as_output(X, Y) \
	GET_MODER(X) |= (_BV(Y * 2))

#define set_as_input(X, Y) \
	GET_MODER(X) &= ~(_BV(Y * 2) | _BV(Y * 2 + 1))

#define output_init(X, Y) \
	GET_OTYPER(X) &= ~(_BV(Y)); \
	GET_OSPEEDR(X) &= ~(_BV(Y) | _BV(Y + 1))

#define input_init(X, Y) \
	GET_PUPDR(X) |= (_BV(Y * 2))


#define RL_RS_OUT_CLEAR OUT_CLEAR(REGALIS_LCD_RS)
#define RL_RS_OUT_SET OUT_SET(REGALIS_LCD_RS)
#define RL_RS_OUT ODR(REGALIS_LCD_RS)
#define RL_RS_PIN PIN(REGALIS_LCD_RS)

#define RL_RW_OUT_CLEAR OUT_CLEAR(REGALIS_LCD_RW)
#define RL_RW_OUT_SET OUT_SET(REGALIS_LCD_RW)
#define RL_RW_OUT ODR(REGALIS_LCD_RW)
#define RL_RW_PIN PIN(REGALIS_LCD_RW)

#define RL_E_OUT_CLEAR OUT_CLEAR(REGALIS_LCD_E)
#define RL_E_OUT_SET OUT_SET(REGALIS_LCD_E)
#define RL_E_PIN PIN(REGALIS_LCD_E)

#define RL_D4_OUT ODR(REGALIS_LCD_D4)
#define RL_D4_PIN PIN(REGALIS_LCD_D4)
#define RL_D4_IN IN(REGALIS_LCD_D4)

#define RL_D5_OUT ODR(REGALIS_LCD_D5)
#define RL_D5_PIN PIN(REGALIS_LCD_D5)
#define RL_D5_IN IN(REGALIS_LCD_D5)

#define RL_D6_OUT ODR(REGALIS_LCD_D6)
#define RL_D6_PIN PIN(REGALIS_LCD_D6)
#define RL_D6_IN IN(REGALIS_LCD_D6)

#define RL_D7_OUT ODR(REGALIS_LCD_D7)
#define RL_D7_PIN PIN(REGALIS_LCD_D7)
#define RL_D7_IN IN(REGALIS_LCD_D7)

#if REGALIS_LCD_LINES == 1
	#define RL_LINES 0x00
#elif REGALIS_LCD_LINES == 2
	#define RL_LINES 0x08
#elif REGALIS_LCD_LINES == 4
	#define RL_LINES 0x08
#else
	#error invalid value for REGALIS_LCD_LINES (type 1, 2 or 4)
#endif

#if REGALIS_LCD_FONT == 0
	#define RL_FONT 0x00
#elif REGALIS_LCD_FONT == 1
	#define RL_FONT 0x04
#else
	#error invalid value for REGALIS_LCD_FONT (select 0 for "5x8" or  1 for "5x11")
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
#elif REGALIS_LCD_CURSOR_BLINK == 0
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

#define HIGH 1
#define LOW 0
#define bit_low(REG, BIT_NO) REG &= ~(_BV(BIT_NO))
#define bit_high(REG, BIT_NO) REG |= (_BV(BIT_NO))
#define get_bit(REG, BIT_NO) ((REG & _BV(BIT_NO)) >> (BIT_NO))
#define get_bit_32(REG, BIT_NO) (((uint32_t)REG & (uint32_t)_BV(BIT_NO)) >> (BIT_NO))
#define set_bit(REG, BIT_NO, VAL) REG = (((VAL) == HIGH) ? REG | (_BV(BIT_NO)) : REG & ~(_BV(BIT_NO)))

/* Internal functions */

/** Execute low-level command
 * @param command - 6-bit command
 * 	| X | X | RS | RW | D7 | D6 | D5 | D4 |
 * 	| 7 | 6 |  5 |  4 |  3 |  2 |  1 |  0 | 
 */
static void regalis_lcd_exec(uint8_t command);
static void regalis_lcd_enable();
static void regalis_lcd_wait_busy();

inline void _regalis_lcd_instruction(uint8_t instruction) {
	regalis_lcd_exec(instruction >> 4);
	_delay_ms(1);
	regalis_lcd_exec(instruction & 0x0F); 
}

inline void regalis_lcd_instruction(uint8_t instruction) {
	regalis_lcd_wait_busy();
	_regalis_lcd_instruction(instruction);
}


void regalis_lcd_init() {
	
	set_as_output(GPIO(REGALIS_LCD_E), PIN(REGALIS_LCD_E));
	set_as_output(GPIO(REGALIS_LCD_RS), PIN(REGALIS_LCD_RS));
	set_as_output(GPIO(REGALIS_LCD_RW), PIN(REGALIS_LCD_RW));
	output_init(GPIO(REGALIS_LCD_E), PIN(REGALIS_LCD_E));
	output_init(GPIO(REGALIS_LCD_RS), PIN(REGALIS_LCD_RS));
	output_init(GPIO(REGALIS_LCD_RW), PIN(REGALIS_LCD_RW));
	output_init(GPIO(REGALIS_LCD_D4), PIN(REGALIS_LCD_D4));
	output_init(GPIO(REGALIS_LCD_D5), PIN(REGALIS_LCD_D5));
	output_init(GPIO(REGALIS_LCD_D6), PIN(REGALIS_LCD_D6));
	output_init(GPIO(REGALIS_LCD_D7), PIN(REGALIS_LCD_D7));
	input_init(GPIO(REGALIS_LCD_D4), PIN(REGALIS_LCD_D4));
	input_init(GPIO(REGALIS_LCD_D5), PIN(REGALIS_LCD_D5));
	input_init(GPIO(REGALIS_LCD_D6), PIN(REGALIS_LCD_D6));
	input_init(GPIO(REGALIS_LCD_D7), PIN(REGALIS_LCD_D7));

	_delay_ms(100); // wait more than 40ms after Vcc rises to valid value
	regalis_lcd_exec(0x03); // interface is 8 bits long
	_delay_ms(5);
	regalis_lcd_enable();
	_delay_us(150);
	regalis_lcd_enable();
	_delay_ms(1);

	regalis_lcd_exec(0x02); // set 4 bits interface
	_delay_us(64); // after this delay we can use internal regalis_lcd_instruction(X)

	_regalis_lcd_instruction(RL_FUNCTION_SET(RL_MODE_4BIT, RL_LINES, RL_FONT));
	_delay_ms(45);
	_regalis_lcd_instruction(RL_DISPLAY_ON_OFF(0, 0, 0)); // display off
	_delay_ms(45);
	_regalis_lcd_instruction(RL_CLEAR_DISPLAY);
	_delay_ms(45);
	_regalis_lcd_instruction(RL_ENTRY_MODE_SET(RL_DDRAM_BH, RL_SHIFT));
	_delay_ms(45);

	_regalis_lcd_instruction(RL_DISPLAY_ON_OFF(RL_DISPLAY_ON, RL_CURSOR, RL_CURSOR_BLINK));
	_delay_ms(45);

	// ready :)

}

static void regalis_lcd_exec(uint8_t command) {
	set_as_output(GPIO(REGALIS_LCD_D7), PIN(REGALIS_LCD_D7));
	set_as_output(GPIO(REGALIS_LCD_D6), PIN(REGALIS_LCD_D6));
	set_as_output(GPIO(REGALIS_LCD_D5), PIN(REGALIS_LCD_D5));
	set_as_output(GPIO(REGALIS_LCD_D4), PIN(REGALIS_LCD_D4));
	
	set_bit(RL_RS_OUT, RL_RS_PIN, get_bit(command, 5));	
	set_bit(RL_RW_OUT, RL_RW_PIN, get_bit(command, 4));
	set_bit(RL_D7_OUT, RL_D7_PIN, get_bit(command, 3));
	set_bit(RL_D6_OUT, RL_D6_PIN, get_bit(command, 2));
	set_bit(RL_D5_OUT, RL_D5_PIN, get_bit(command, 1));
	set_bit(RL_D4_OUT, RL_D4_PIN, get_bit(command, 0));
	
	regalis_lcd_enable();
}

inline void regalis_lcd_clear() {
	regalis_lcd_instruction(RL_CLEAR_DISPLAY);
}

void regalis_lcd_clear_line(uint8_t line_no) {
	regalis_lcd_goto(0, line_no);
	int8_t length = REGALIS_LCD_CHARS;
	while(length-- > 0) {
		regalis_lcd_putc(' ');
	}
	regalis_lcd_goto(0, line_no);
}

inline void regalis_lcd_home() {
	regalis_lcd_instruction(RL_RETURN_HOME);
}

void regalis_lcd_putc(char c) {
	regalis_lcd_wait_busy();
	regalis_lcd_exec(0x20 | (c >> 4));
	regalis_lcd_wait_busy();
	regalis_lcd_exec(0x20 | (c & 0x0F));
}

void regalis_lcd_puts(const char *str) {
	while(*str != '\0') {
		regalis_lcd_putc(*str);
		str++;
	}
}

inline static void regalis_lcd_enable() {
	bit_high(RL_E_OUT_SET, RL_E_PIN);
	_delay_us(5);
	bit_high(RL_E_OUT_CLEAR, RL_E_PIN);
}

uint8_t regalis_lcd_read(uint8_t register_select) {

	/* data lines as input */
	set_as_input(GPIO(REGALIS_LCD_D7), PIN(REGALIS_LCD_D7));
	set_as_input(GPIO(REGALIS_LCD_D6), PIN(REGALIS_LCD_D6));
	set_as_input(GPIO(REGALIS_LCD_D5), PIN(REGALIS_LCD_D5));
	set_as_input(GPIO(REGALIS_LCD_D4), PIN(REGALIS_LCD_D4));

	/* prepare for read busy flag and address couter */
	if(register_select == RL_READ_BUSY)
		bit_high(RL_RS_OUT_CLEAR, RL_RS_PIN);
	else
		bit_high(RL_RS_OUT_SET, RL_RS_PIN);

	bit_high(RL_RW_OUT_SET, RL_RW_PIN);

	uint8_t data = 0x00;

	bit_high(RL_E_OUT_SET, RL_E_PIN);
	_delay_us(40);
	set_bit(data, 7, get_bit_32(RL_D7_IN, RL_D7_PIN));
	set_bit(data, 6, get_bit_32(RL_D6_IN, RL_D6_PIN));
	set_bit(data, 5, get_bit_32(RL_D5_IN, RL_D5_PIN));
	set_bit(data, 4, get_bit_32(RL_D4_IN, RL_D4_PIN));
	bit_high(RL_E_OUT_CLEAR, RL_E_PIN);

	_delay_us(20);

	bit_high(RL_E_OUT_SET, RL_E_PIN);
	_delay_us(40);
	set_bit(data, 3, get_bit_32(RL_D7_IN, RL_D7_PIN));
	set_bit(data, 2, get_bit_32(RL_D6_IN, RL_D6_PIN));
	set_bit(data, 1, get_bit_32(RL_D5_IN, RL_D5_PIN));
	set_bit(data, 0, get_bit_32(RL_D4_IN, RL_D4_PIN));
	bit_high(RL_E_OUT_CLEAR, RL_E_PIN);

	return data;

}

void regalis_lcd_goto(uint8_t x, uint8_t y) {
	#if REGALIS_LCD_LINES == 1
		regalis_lcd_instruction(RL_DDRAM_SET(x))
	#elif REGALIS_LCD_LINES == 2
		switch (y) {
			case 0:
				regalis_lcd_instruction(RL_DDRAM_SET(RL_LINE_0 + x));
			break;
			case 1:
				regalis_lcd_instruction(RL_DDRAM_SET(RL_LINE_1 + x));
			break;
		}
	#elif REGALIS_LCD_LINES == 4
		switch (y) {
			case 0:
				regalis_lcd_instruction(RL_DDRAM_SET(RL_LINE_0 + x));
			break;
			case 1:
				regalis_lcd_instruction(RL_DDRAM_SET(RL_LINE_1 + x));
			break;
			case 2:
				regalis_lcd_instruction(RL_DDRAM_SET(RL_LINE_2 + x));
			break;
			case 3:
				regalis_lcd_instruction(RL_DDRAM_SET(RL_LINE_3 + x));
			break;
		}
	#endif
}

inline void regalis_lcd_goto_addr(uint8_t addr) {
	regalis_lcd_instruction(RL_DDRAM_SET(addr));
}

static void regalis_lcd_wait_busy() {
	register uint8_t data;
	while ((data = regalis_lcd_read(RL_READ_BUSY)) & _BV(7)) {}
	_delay_us(5); // wait (address counter)
}

