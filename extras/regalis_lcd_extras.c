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

#include <util/delay.h>

#include "regalis_lcd.h"
#include "regalis_lcd_demo.h"
#include "regalis_lcd_config.h"
#include "regalis_lcd_extras.h"

uint8_t regalis_center(uint8_t length) {
	int8_t position = (((REGALIS_LCD_CHARS / 2)) - (length / 2)) - 1;
	return (position < 0x00 ? 0x00 : position);
}

void regalis_soft_puts(const char *str) {
	while(*str != '\0') {
		regalis_lcd_putc(*str);
		++str;
		_delay_ms(SOFT_DELAY);
	}
}

void regalis_soft_clear(uint8_t length) {
	regalis_lcd_instruction(RL_ENTRY_MODE_SET(RL_DECREMENT, RL_SHIFT_OFF));
	while(length-- != 0) {
		regalis_lcd_putc(' ');	
		_delay_ms(SOFT_DELAY);	
	}
	regalis_lcd_putc(' ');
	regalis_lcd_instruction(RL_ENTRY_MODE_SET(RL_INCREMENT, RL_SHIFT_OFF));
}

void regalis_soft_move(uint8_t scr_addr, uint8_t dst_addr, uint8_t length) {
	uint8_t i;
	for(i = 0; i < length; ++i) {
		regalis_lcd_goto_addr(scr_addr + i);
		char c = regalis_lcd_read(RL_READ_DATA);	
		regalis_lcd_goto_addr(scr_addr + i);
		regalis_lcd_putc(' ');
		_delay_ms(SOFT_DELAY / 2);
		regalis_lcd_goto_addr(dst_addr + i);
		regalis_lcd_putc(c);
		_delay_ms(SOFT_DELAY);
	}
}

void regalis_shift_r(uint8_t amount) {
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
