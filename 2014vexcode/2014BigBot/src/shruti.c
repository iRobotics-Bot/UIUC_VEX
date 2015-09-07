/*
 * shruti.c
 *
 *  Created on: Jan 28, 2014
 *      Author: Matt
 */

//begin facing balls

#include "auto_scripts/shruti.h"
#include "main.h"
#include "helpers.h"


/* GLOBAL VARIABLES */
static int state = RAISE_ARM;
static int i;
static int left_ime, right_ime;

//potentiometers, encoders for wheel rotations, gyros ave for turns.

//raise arm, go forwards, move back and along bar, forwards, move back and along bar, forwards.
//lower arm, drive up, block team

void autowoot(){
	printf("autonomous...  ");
	switch ( state )
	{
		case RAISE_ARM:
			printf("raising arm");
			tankDrive( 0, 0 );
			Manipulate( 127, 0, 0, 0, 0, 0, 0 );
			i++;
			if (i > 2000) {
				i = 0;
				state = FOR1;
				imeReset(LEFT_IME);
			}
			break;

		case FOR1:
			tankDrive( 100, 100 );
			Manipulate( 0, 0, 0, 0, 0, 0, 0 );
			imeGet(LEFT_IME, &left_ime);
			if (left_ime > 500){
				state = BACK1;
				imeReset(LEFT_IME);
			}
			break;

		case BACK1:
			tankDrive( -100, -100 );
			Manipulate( 0, 0, 0, 0, 0, 0, 0);
			imeGet(LEFT_IME, &left_ime);
			if (left_ime < -200){
				state = LOW_ARM;
				imeReset(LEFT_IME);
			}
			break;

		case LOW_ARM:
			tankDrive( 0, 0 );
			Manipulate( -127, 0, 0, 0, 0, 0, 0 );
			i++;
			if (i > 2000) {
				i = 0;
				state = DONE;
			}
			break;

		case FOR3:
			tankDrive( 127, 127 );
			Manipulate( -127, 0, 0, 0, 0, 0, 0 );
			imeGet(LEFT_IME, &left_ime);
			if (left_ime > 600){
				state = TURN1;
				imeReset(LEFT_IME);
			}
			break;

		case TURN1:
			tankDrive( 127, -127 );
			Manipulate( -127, 0, 0, 0, 0, 0, 0 );
			imeGet(LEFT_IME, &left_ime);
			if (left_ime > 50){
				state = FOR4;
				imeReset(LEFT_IME);
			}
			break;

		case FOR4:
			tankDrive( 127, 127 );
			Manipulate( -127, 0, 0, 0, 0, 0, 0 );
			imeGet(LEFT_IME, &left_ime);
			if (left_ime > 2000){
				state = DONE;
				imeReset(LEFT_IME);
			}
			break;

		case DONE:
			tankDrive(0 ,0 );
			Manipulate( 0, 0, 0, 0, 0, 0, 0 );
			break;
	}

	printf("\n\r");
}
