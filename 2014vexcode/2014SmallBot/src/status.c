/*
 * status.h
 *
 *  Created on: Apr 19, 2014
 *      Author: Status
 */

#include "auto_scripts/status.h"
#include "helpers.h"
#include "main.h"



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


void FUCKYEAH_AUTONOMOUS(int color){

	switch (state)
	{

		case INIT:// initializes gyro and imes also waits for arm to come down

			resetAverageGyro();

			Manipulate( -127, 0, 0, 0, 0, 0, 0);//parameters: arm, throw, antenna, override, overrideval, recalibrate

			imeReset( LEFT_IME );
			imeReset( RIGHT_IME );

			a++;

			if(a > 350){
				a = 0;
				state = GO1;
			}

			printf(" %lf %lf ", arm_PID_L.setpoint, arm_PID_L.observed);
			printf("INIT Step\n\r");

			break;

		case GO1:// drives until it hits the Ball
			Drive( 127, 0 );

			Manipulate( -127, 0, 0, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current > 323 ) {
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				resetAverageGyro();
				state = ROTATE1;
			}
			printf("GO Step\n\r");
			printf(" %d",current);
			break;

		case ROTATE1://rotate it to face the balls and also dump into catapult
			if(color == RED){
				if(getAverageGyro() > -110)// this is questionable at the moment
					Drive(0, -127);
			}
			if(color == BLUE){
				if(getAverageGyro() < 110)// this is questionable at the moment don't know the angles of the gyro readings
					Drive(0, 127);
			}

			//dump into the catapult
			Manipulate( 127, 0, 0, 0, 0, 0, 0);

			a++;

			if(a > 350){//this is just to wait for the arm to get into position
				a = 0;
				state = BACKWARD1;
				resetAverageGyro();
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
			}
			printf("ROTATE1 Step\n\r");
			break;

		case BACKWARD1:
			Drive(-127, 0);

			if( current > 323 ) {
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				resetAverageGyro();
				state = LAUNCH1;
			}

			break;

		case LAUNCH1:
			Drive( 127, 0);//drive forward
			Manipulate( 0, 0, 0, 0, 0, 0, 0);

			a++;

			if(a > 340){
				Manipulate(0, 1, 0, 0, 0, 0, 0);//LAUNCH!!!

			}

			if(a > 350){//this is just to wait for the arm to get into position
				a = 0;
				state = ROTATE2;
				resetAverageGyro();
			}
			break;

		case ROTATE2:// rotates back for round 2 of launching
			if(color == RED){
				if(getAverageGyro() < -110)// this is questionable at the moment
					Drive(0, 127);
			}
			if(color == BLUE){
				if(getAverageGyro() > 110)// this is questionable at the moment don't know the angles of the gyro readings
					Drive(0, -127);
			}

			//open it back up
			Manipulate( -127, 0, 0, 0, 0, 0, 0);

			a++;

			if(a > 350){//this is just to wait for the arm to get into position
				a = 0;
				state = LAUNCH1;
				resetAverageGyro();
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
			}
			break;

		case GO2:// drives until it hits the Ball 2
			Drive( 127, 0 );

			Manipulate( -127, 0, 0, 0, 0, 0, 0);

			imeGet( LEFT_IME, &current );

			if( current > 600 ) {
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				state = ROTATE3;
			}
			break;

		case ROTATE3://rotate it to face the balls and also dump into catapult
			if(color == RED){
				if(getAverageGyro() > -110)// this is questionable at the moment
					Drive(0, -127);
			}
			if(color == BLUE){
				if(getAverageGyro() < 110)// this is questionable at the moment don't know the angles of the gyro readings
					Drive(0, 127);
			}

			//dump into the catapult
			Manipulate( 127, 0, 0, 0, 0, 0, 0);

			a++;

			if(a > 350){//this is just to wait for the arm to get into position
				a = 0;
				resetAverageGyro();
				state = BACKWARD2;
			}
			break;

		case BACKWARD2:
			Drive(-127, 0);

			if( current > 323 ) {
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				resetAverageGyro();
				state = LAUNCH2;
			}

			break;

		case LAUNCH2:
			Drive( 127, 0);//don't move
			Manipulate( 0, 0, 0, 0, 0, 0, 0);

			a++;

			if(a > 340){
				Manipulate(0, 1, 0, 0, 0, 0, 0);//launch here

			}

			if(a > 350){//this is just to wait for the arm to get into position
				a = 0;
				state = WAIT;
			}
			break;

		case WAIT:
			TankDrive( 0, 0 );
			Manipulate( 0, 0, 0, 0, 0, 0, 0);
			break;
	}
}


