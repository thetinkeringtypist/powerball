/**
 * Copyright (C) 2015 Evan Bezeredi <bezeredi.dev@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see {http://www.gnu.org/licenses/}.
 **/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/list.h"
#include "include/misc-macros.h"


/* Local Data Types */
typedef enum{
	NOWIN = 0,
	FOUR = 1,
	SEVEN = 2,
	HUNDRED = 3,
	TENTHOUSAND = 4,
	MILLION = 5,
	GRANDPRIZE = 6
} win_e;


typedef struct{
	char play;       /* The play identifier */
	int  numbers[5]; /* The five play numbers */
	int  pbnum;      /* Powerball number */
	win_e win;       /* Whether or not the number is a winner */
} number_t;


/* Local variables */
static number_t winner;


/* Local Functions */
static void usage(void);
static llist_t* init_numbers(FILE* const infile);
static void skip_line(FILE* infile);
static void check_numbers(llist_t* const pb_numbers);
static void check_win(number_t* n);
static int contains(number_t* const n);
static void print_numbers(llist_t* const pb_numbers);


int main(int argc, char* argv[]){
	FILE* infile;
	llist_t* pb_numbers;

	if(argc != 2){
		usage();
		exit(EXIT_FAILURE);
	}

	if((infile = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "%s: cannot open file '%s'. Exit.\n", argv[0], argv[1]);
		exit(EXIT_FAILURE);
	}

	pb_numbers = init_numbers(infile);
	fclose(infile);

	check_numbers(pb_numbers);
	print_numbers(pb_numbers);

	ll_clear(pb_numbers);
	ll_free(pb_numbers);
}


/**
 * Print the usage information about the program.
 **/
static void usage(void){
	printf("powerball: usage: powerball filename. Exit.\n");
}


/**
 * Read the input file and create a list of numbers from it.
 *
 * @param infile the file to read.
 * @return the list of numbers in the file.
 **/
static llist_t* init_numbers(FILE* const infile){
	llist_t* pb_numbers = ll_init(number_t);
	char c;
	int status, num1, num2, num3, num4, num5, pbnum;
	number_t* n;

	/**
	 * For each line:
	 * 	Create a number
	 * 	Fill in the play, numbers, and pb number
	 * 	add it to the list.
	 **/
	memset(&winner, 0x00, sizeof(winner));
	while((status = fscanf(infile, "%c %d %d %d %d %d | %d\n",
			&c, &num1, &num2, &num3, &num4, &num5, &pbnum)) != EOF){
		if(c == '#' || c == '\n' || c == '\r'){
			skip_line(infile);
			continue;
		}

		if(c == '!'){
			winner.win = GRANDPRIZE;
			winner.play = '!';
			winner.numbers[0] = num1;
			winner.numbers[1] = num2;
			winner.numbers[2] = num3;
			winner.numbers[3] = num4;
			winner.numbers[4] = num5;
			winner.pbnum = pbnum;
			continue;
		}
			
		n = malloc(sizeof(number_t));
		n->play = c;
		n->numbers[0] = num1;
		n->numbers[1] = num2;
		n->numbers[2] = num3;
		n->numbers[3] = num4;
		n->numbers[4] = num5;
		n->pbnum = pbnum;

		ll_addl(pb_numbers, n);
	}

	return pb_numbers;
}


/**
 * Consume all the characters on the current line (i.e. "skipping" the line).
 *
 * @param infile the file to read from.
 **/
static void skip_line(FILE* infile){
	int status;
	char c;

	while((status = fscanf(infile, "%c", &c)) != EOF){
		if(c == '\n')
			break;
	}
}


/**
 * Check each the list of numbers for winners.
 *
 * @param pb_numbers the list of numbers to check.
 **/
static void check_numbers(llist_t* const pb_numbers){
	int size, i;

	size = ll_size(pb_numbers);
	for(i = 0; i < size; i++)
		check_win(ll_get(pb_numbers, i));
}


/**
 * Check a number to see if it's a winner.
 *
 * @param n the number to check
 **/
static void check_win(number_t* n){
	int count;

	count = contains(n);
	switch(count){
		case 5:
			n->win = (n->pbnum == winner.pbnum) ? GRANDPRIZE : MILLION; break;
		case 4:
			n->win = (n->pbnum == winner.pbnum) ? TENTHOUSAND : HUNDRED; break;
		case 3:
			n->win = (n->pbnum == winner.pbnum) ? HUNDRED : SEVEN; break;
		case 2:
			n->win = (n->pbnum == winner.pbnum) ? SEVEN : NOWIN; break;
		case 1:
		default:
			n->win = (n->pbnum == winner.pbnum) ? FOUR : NOWIN;
	}
}


/**
 * Count the numbers that are common between a number and the winning number.
 *
 *	@param n the number to check.
 *	@return the count of numbers that a number and the winning number have in
 *		common.
 **/
static int contains(number_t* const n){
	int i, j, count;
	number_t temp;

	count = 0;
	memcpy(&temp, &winner, sizeof(winner));
	for(i = 0; i < 5; i++){
		for(j = 0; j < 5; j++){
			if(n->numbers[i] == temp.numbers[j]){
				temp.numbers[j] = -1;
				count++;
				break;
			}
		}
	}
	
	return count;
}


/**
 * Prints each number in the list and its winning status.
 **/
static void print_numbers(llist_t* const pb_numbers){
	int size, i;
	char* status;
	number_t* n;
	
	if(winner.play != '!')
		printf("No winning number specified.\n");
	else
		printf("[   Draw   ]: %c %02d %02d %02d %02d %02d | %02d\n", winner.play,
				winner.numbers[0], winner.numbers[1], winner.numbers[2],
				winner.numbers[3], winner.numbers[4], winner.pbnum);

	size = ll_size(pb_numbers);
	for(i = 0; i < size; i++){
		n = ll_get(pb_numbers, i);
		switch(n->win){
			case NOWIN: status = COLORIZE("    $0    ", RED, BOLD); break;
			case FOUR: status = COLORIZE("    $4    ", GREEN, BOLD); break;
			case SEVEN: status = COLORIZE("    $7    ", GREEN, BOLD); break;
			case HUNDRED: status = COLORIZE("   $100   ", GREEN, BOLD); break;
			case TENTHOUSAND: status = COLORIZE("  $10,000 ", GREEN, BOLD); break;
			case MILLION: status = COLORIZE("$1,000,000", GREEN, BOLD); break;
			case GRANDPRIZE: status = COLORIZE(" Jackpot! ", GREEN, BOLD); break;
		}
		
		printf("[%s]: %c %02d %02d %02d %02d %02d | %02d\n" , status, n->play,
				n->numbers[0], n->numbers[1], n->numbers[2], n->numbers[3],
				n->numbers[4], n->pbnum);
	}
}

