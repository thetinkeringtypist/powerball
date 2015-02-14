Powerball
=========
Check your Powerball numbers! Although a trivial program, `powerball` makes it
easy to check whether or not your powerball numbers are winners or not. It is
extremely useful if you are someone who purchases 10 or more numbers.


How to Use
----------
Create a file in the following format:

	#! This is the format of the input file.
	A 01 02 03 04 05 | 06
	B 07 08 09 10 11 | 12
	C 13 14 15 16 17 | 18
	D 19 20 21 22 23 | 24
	E 25 26 27 28 29 | 30

	F 31 32 33 34 35 | 36
	G 37 38 39 40 41 | 42

and so on for as many numbers as you have. To designate the winning number, use
the following:

	! 01 02 03 04 05 | 06

Then run it through `powerball` and you'll know which ones are winners!
`powerball` uses ansi color codes by default.

Note: `powerball` only supports one winning number. Input files are read one
line at a time. In the event of multiple winning numbers in the file


How to Build
------------
`powerball` is built using autotools, so the natural build chain is the
following:

	./configure
	./make
	./make install

The last command may require root permissions. The executable, `powerball` is
located in the src directory.


Notes
-----
Some notes about `powerball`:
 * `powerball` produces output using ANSI color codes
 * Input files are read one line at a time. In the event of multiple winning
	numbers, `powerball` uses the last winning number specified.


License
-------
This program is in no way associated the Multi-State Lottery Association and is
neither endorsed or supported by said organization. This program is distributed
under the GPL Version 3. See a slight summary below:

	powerball: check the winning status of your powerball numbers.
	Copyright (C) 2015  Evan Bezeredi

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see {http://www.gnu.org/licenses/}.

