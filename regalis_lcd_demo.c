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

#include <util/delay.h>
#include "regalis_lcd.h"
#include "regalis_lcd_demo.h"
#include "regalis_lcd_config.h"
#include "regalis_lcd_extras.h"

void regalis_lcd_demo() {
	_delay_ms(1000);
	regalis_lcd_home();
	regalis_lcd_goto(regalis_center(11), 0);
	regalis_soft_puts("regalis_lcd");
	regalis_lcd_goto(regalis_center(8), 1);
	_delay_ms(1000);
	regalis_soft_puts("= demo =");
	_delay_ms(2000);
	regalis_soft_clear(8);
	_delay_ms(1000);
	regalis_soft_move(regalis_center(11), RL_LINE_1 + regalis_center(11), 11);
	_delay_ms(500);
	regalis_lcd_home();
	regalis_soft_puts("Patryk Jaworski");
	_delay_ms(1000);
	regalis_lcd_goto(REGALIS_LCD_CHARS - 1, 1);
	regalis_soft_clear(REGALIS_LCD_CHARS);
	regalis_lcd_goto(REGALIS_LCD_CHARS - 1, 0);
	regalis_soft_clear(REGALIS_LCD_CHARS);
	regalis_lcd_goto(0, 0);
	regalis_soft_puts("github.com/regalis");
	regalis_shift_r(4);
	regalis_lcd_goto(REGALIS_LCD_CHARS, 0);
	_delay_ms(1000);
	regalis_soft_clear(REGALIS_LCD_CHARS);
}
