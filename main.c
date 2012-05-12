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
#include "regalis_lcd.h"
#include "regalis_lcd_demo.h"

int main() {
	/* ->  Initialize LCD display
	 *
	 * NOTE! Set your regalis_lcd_config.h first!
	 */
	regalis_lcd_init();

	/* -> Check your configuration, run a simple regalis_lcd demo */
	while(1) {
		regalis_lcd_demo();	
	}
}
