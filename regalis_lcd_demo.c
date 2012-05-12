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
#include <inttypes.h>
#include "regalis_lcd.h"
#include "regalis_lcd_demo.h"
#include "regalis_lcd_config.h"

#define SOFT_DELAY 80
#define SHIFT_DELAY 650

static char visit[] = "VISIT"; // 5
static char site[] = "github.com/regalis"; // 18
static char demo[] = " = demo ="; // 8
static char lib_name[] = "regalis_lcd"; // 11
static char author[] = "Patryk Jaworski"; // 15

uint8_t center(uint8_t length) {
	int8_t position = (((REGALIS_LCD_CHARS / 2)) - (length / 2)) - 1;
	return (position < 0x00 ? 0x00 : position);
}

static void regalis_soft_print(const char *str) {
	while(*str != '\0') {
		regalis_lcd_putc(*str);
		++str;
		_delay_ms(SOFT_DELAY);
	}
}

static void regalis_soft_clear(uint8_t length) {
	regalis_lcd_instruction(RL_ENTRY_MODE_SET(RL_DECREMENT, RL_SHIFT_OFF));
	while(length-- != 0) {
		regalis_lcd_putc(' ');	
		_delay_ms(SOFT_DELAY);	
	}
	regalis_lcd_putc(' ');
	regalis_lcd_instruction(RL_ENTRY_MODE_SET(RL_INCREMENT, RL_SHIFT_OFF));
}

static void regalis_soft_move(uint8_t scr_addr, uint8_t dst_addr, uint8_t length) {
	int16_t i;
	for(i = 0; i < length; ++i) {
		regalis_lcd_goto_addr(scr_addr + i);
		uint8_t c = regalis_lcd_read(RL_READ_DATA);	
		regalis_lcd_goto_addr(scr_addr + i);
		regalis_lcd_putc(' ');
		_delay_ms(SOFT_DELAY / 2);
		regalis_lcd_goto_addr(dst_addr + i);
		regalis_lcd_putc(c);
		_delay_ms(SOFT_DELAY);
	}
}

static void regalis_shift_r(uint8_t amount) {
	uint8_t i;
	for(i = 0; i < amount; ++i) {
		regalis_lcd_instruction(RL_SHIFT_DISPLAY(RL_DISPLAY_LEFT));
		_delay_ms(SHIFT_DELAY);
	}
	_delay_ms(SHIFT_DELAY * 3);
	for(i = 0; i < amount; ++i) {
		regalis_lcd_instruction(RL_SHIFT_DISPLAY(RL_DISPLAY_RIGHT));
		_delay_ms(SHIFT_DELAY);
	}
}

void regalis_lcd_demo() {
	_delay_ms(1000);
	regalis_lcd_home();
	regalis_lcd_goto(center(11), 0);
	regalis_soft_print(lib_name);
	regalis_lcd_goto(center(8), 1);
	_delay_ms(1000);
	regalis_soft_print(demo);
	_delay_ms(2000);
	regalis_soft_clear(8);
	_delay_ms(1000);
	regalis_soft_move(center(11), RL_LINE_2 + center(11), 11);
	_delay_ms(500);
	regalis_lcd_home();
	regalis_soft_print(author);
	_delay_ms(1000);
	regalis_lcd_goto(REGALIS_LCD_CHARS - 1, 1);
	regalis_soft_clear(REGALIS_LCD_CHARS);
	regalis_lcd_goto(REGALIS_LCD_CHARS - 1, 0);
	regalis_soft_clear(REGALIS_LCD_CHARS);
	regalis_lcd_goto(0, 0);
	regalis_soft_print(site);
	regalis_shift_r(4);
	regalis_lcd_goto(REGALIS_LCD_CHARS, 0);
	_delay_ms(1000);
	regalis_soft_clear(REGALIS_LCD_CHARS);
}
