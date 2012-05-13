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

#ifndef __REGALIS_LCD_EXTRAS_
#define __REGALIS_LCD_EXTRAS_

/** Number of milliseconds used for soft transitions */
#define SOFT_DELAY 80

/** Number of milliseconds used for shifts */
#define SHIFT_DELAY 650

/** Obtain relative jump to center your text
 * This function calculates the number of characters with which
 * you should move the cursor to center your text.
 * @param length text length
 * @see regalis_lcd_goto()
 * @see regalis_lcd_goto_addr()
 */
uint8_t regalis_center(uint8_t length);

/** Print text with nice-looking smooth transition (from left to right)
 * Function will start from actual cursor position
 * @param src C string to print
 */
void regalis_soft_puts(const char *str);

/** Clear text with nice-looking smooth transition (from right to left)
 * Function will start from actual cursor position
 * @param length number of character
 */
void regalis_soft_clear(uint8_t length);

/** Move length characters from scr_addr to dst_addr with smooth transition
 * After move operation cursor will be placed in position dst_addr + length + 1 
 * The memory areas must not overlap...
 * @param scr_addr DDRAM source address (address of first character to move)
 * @param dst_addr DDRAM destination address
 * @param legth number of characters to move
 */
void regalis_soft_move(uint8_t scr_addr, uint8_t dst_addr, uint8_t length);

/** Shift (left) display, wait and return to previous position
 * @param amount number of shifts
 */
void regalis_shift_r(uint8_t amount);

#endif
