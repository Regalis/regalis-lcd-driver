# Driver for alphanumeric LCD modules

An open-source, easy to use library for controlling LCD modules
based on HD44780(U). Only **4-bits** interface is supported. 

# Examples

*Before displaying the text on the LCD, you should review the
configuration file (**regalis_lcd_config.h**).*

## I. Hello world

Before any read/write operations you ought to init LCD module
by running `regalis_lcd_init()` function. This will perform
module initialization by executing instructions (set interface
mode to 4-bits, set cursor behaviour etc.). After this operations
cursor will be placed in (0, 0) position - first character, first line.

	(...)
	#include "regalis_lcd.h"
	
	int main() {
		regalis_lcd_init();
		/** Now you can read/write data from/to LCD */
	}

To display character on your LCD, just use `regalis_lcd_putc(char)`
function. If you want to display C string (const char \*), use
`regalis_lcd_puts(const char *)`. **NOTE**, this functions ignores
new line characters (\n).

To display *"Hello world"* message, just type:

	(...)
	#include "regalis_lcd.h"

	int main() {
		regalis_lcd_init();
		regalis_lcd_puts("Hello world!");
	}

## II. Cursor positioning

There are two different functions for manipulating cursor position:

	* `regalis_lcd_goto(X, Y);` (first character, first line means X = 0 and Y = 0)
	* `regalis_lcd_goto_addr(DDRAM_ADDR);`

If you want to display *"Hello world"* in first line and *"regalis\_lcd"* in
second line, you should use this functions:

	(...)
	#include "regalis_lcd.h"

	int main() {
		regalis_lcd_init();
		regalis_lcd_puts("Hello world");
		regalis_lcd_goto(0, 1);
		regalis_lcd_puts("regalis_lcd");
	}

## Clearing display

To clear whole display and set cursor position in (0, 0) use `regalis_lcd_clear()` function.

## Extras

`regalis_lcd` library also provides some simple functions that can be used to animate text:

	* `regalis_soft_puts(const char *)` - print text with nice-looking smooth transition (from left to right)
	* `regalis_soft_clear(uint8_t length)` - print text with nice-looking smooth transition (from left to right)
	* `regalis_soft_move(uint8_t scr_addr, uint8_t dst_addr, uint8_t length)` - move *length* characters from *scr_addr* to *dst_addr* with smooth transition
	* `regalis_shift_r(uint8_t amount)` - shift (left) display, wait and return to previous position

See **extras/regalis_lcd_extras.h** for full list of **extras** functions with documentation.

# Credits

Developed with **VIM** on **Arch GNU/Linux**, tested with **WC1602A0**.

Feel free to report bugs and suggestions.

License
=======

Copyright (C) 2012 Patryk Jaworski \<regalis@regalis.com.pl\>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses.
