# 
# Copyright (C) Patryk Jaworski <regalis@regalis.com.pl>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

OBJS=$(patsubst %.c, %.o, $(wildcard *.c))
F_CPU?=4000000UL
DEVICE?=atmega8
PROGRAMMER?=usbasp

all: main.hex

main.hex: main.elf
	avr-objcopy -O ihex -R .eeprom $< $@

main.elf: $(OBJS)
	avr-gcc $^ -mmcu=$(DEVICE) -Os -Wall -o $@

%.o: %.c
	avr-gcc -c -mmcu=$(DEVICE) -Os -Wall $< -o $@ -DF_CPU=$(F_CPU)

%.o: %.c %.h
	avr-gcc -c -mmcu=$(DEVICE) -Os -Wall $< -o $@ -DF_CPU=$(F_CPU)

clean:
	@rm -rvf *.o main.elf || /bin/true

write: main.hex
	avrdude -p $(DEVICE) -c $(PROGRAMMER) -U flash:w:$<

.PHONY: all clean write
