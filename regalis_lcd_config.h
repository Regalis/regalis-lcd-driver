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


/**********************************************
 *               Connection block             *
 **********************************************/

#define REGALIS_LCD_RS  B, 4
#define REGALIS_LCD_RW  B, 5
#define REGALIS_LCD_E   B, 6
#define REGALIS_LCD_D4  B, 0
#define REGALIS_LCD_D5  B, 1
#define REGALIS_LCD_D6  B, 2
#define REGALIS_LCD_D7  B, 3

/**********************************************
 *       Basic LCD configuration block        *
 **********************************************/

/** Number of lines in your display
 * Valid values:
 * \a 1
 * \a 2
*/
#define REGALIS_LCD_LINES 2

/** LCD font size
 * Valid values:
 * \a 0 for "5x8" font
 * \a 1 for "5x11" font
*/
#define REGALIS_LCD_FONT 0

/** LCD cursor
 * Valid values:
 * \a 1 - cursor is on
 * \a 0 - cursor is off
*/
#define REGALIS_LCD_CURSOR 0

/** LCD cursor blink
 * Valid values:
 * \a 1 - cursor blink is on, that performs alternate between
 * all the high data and display character at the cursor position
 * \a 0 - blink is off
*/
#define REGALIS_LCD_CURSOR_BLINK 0


/**********************************************
 * Advenced LCD configuration block           *
 * NOTE! Read your LCD driver datasheet first *
 **********************************************/


/** DDRAM/OGRAM default behavior after read/write operation
 * Valid values:
 * \a 0 - auto decrement mode
 * \a 1 - auto increment mode
 */
#define REGALIS_LCD_DDRAM_BH 1

/** Shift of entire display
 * Valid values:
 * \a 0 - disable auto shift
 * \a 1 - enable auto shift
 */
#define REGALIS_LCD_SHIFT 1

