/*
 * vivian.c
 *
 *  Created on: Jan 31, 2014
 *      Author: Matt
 */

#include "auto_scripts/vivian.h"
#include "helpers.h"
#include "main.h"

#if 0

/* Global Variables */
static int current;
static int state = INIT;
static int a = 0;

	//Battle Plan:
	/*
	 * deploy arm, go foward, arc and bump over the big ball (push small balls in process)
	 * acquire the second ball and dump it over in line with one of the balls on the bar
	 * Then raid enemy territory while avoiding enemy tiles at all cost. take no prisoners
	 *
	 */


void auto_win_red(){

	switch (state)
	{

		case INIT:

			imeReset( LEFT_IME );
			imeReset( RIGHT_IME );
			Manipulate( -127, 0, 0, 0, 0);

			a++;

			if(a > 25000){
				a = 0;
				state = GO1;
			}
			break;

		case GO1:
			TankDrive( 127, 127 );
			Manipulate( -127, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current > 323 ) {
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = ARC_LEFT;
			}
			break;

		case ARC_LEFT:
			TankDrive( -30, 127 );
			Manipulate( -127, 0, 0, 0, 0);

			imeGet( RIGHT_IME, &current );

			if( current < -750 ) {
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = WAIT;
			}
			break;

		case REVERSE1:
			TankDrive( 30, -127 );
			Manipulate( -127, 0, 0, 0, 0);

			imeGet( RIGHT_IME, &current );

			if( current > 550 ){
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = GO2;
			}
			break;

		case GO2:
			TankDrive( 127, 127 );
			Manipulate( -127, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current > 250 || current < -250) {
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = WAIT;
			}
			break;

		case PICK_UP:
			TankDrive( 0, 0 );
			Manipulate( 0, 0, 0, 0, 0);

			state = WAIT;

			break;

		case REVERSE2:
			TankDrive( 127, 0 );
			Manipulate( 0, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current ){
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = WAIT;
			}
			break;

		case GO3:
			TankDrive( -127, -127 );
			Manipulate( 0, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current > 600 ){
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = DUMP;
			}
			break;

		case DUMP:
			TankDrive( 0, 0 );
			Manipulate( 127, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current > 600 ){
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = WAIT;
			}
			break;

		case WAIT:
			TankDrive( 0, 0 );
			Manipulate( 0, 0, 0, 0, 0);
			break;
	}



}

void auto_win_blue(){

	switch (state)
	{

		case INIT:

			imeReset( LEFT_IME );
			imeReset( RIGHT_IME );
			Manipulate( -127, 0, 0, 0, 0);

			a++;

			if(a > 20000){
				a = 0;
				state = GO1;
			}
			break;

		case GO1:
			TankDrive( 127, 127 );
			Manipulate( -127, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current > 350 ) {
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = ARC_LEFT;
			}
			break;

		//Note: This arcs right
		case ARC_LEFT:
			TankDrive( 127, -30 );
			Manipulate( -127, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current > 650 || current < -650) {
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = WAIT;
			}
			break;

		case REVERSE1:
			TankDrive( -127, 30 );
			Manipulate( -127, 0, 0, 0, 0);

			imeGet( RIGHT_IME, &current );

			if( current < -450 || current > 450){
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = GO2;
			}
			break;

		case GO2:
			TankDrive( 127, 127 );
			Manipulate( -127, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current > 250 || current < -250) {
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = WAIT;
			}
			break;

		case PICK_UP:
			TankDrive( 0, 0 );
			Manipulate( 0, 0, 0, 0, 0 );

			state = REVERSE2;

			break;

		case REVERSE2:
			TankDrive( 127, 0 );
			Manipulate( 0, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current ){
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = GO3;
			}
			break;

		case GO3:
			TankDrive( -127, -127 );
			Manipulate( 0, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current > 600 ){
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = DUMP;
			}
			break;

		case DUMP:
			TankDrive( 0, 0 );
			Manipulate( 127, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current > 600 ){
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = WAIT;
			}
			break;

		case WAIT:
			TankDrive( 0, 0 );
			Manipulate( 0, 0, 0, 0, 0);
			break;
	}



}

#endif
