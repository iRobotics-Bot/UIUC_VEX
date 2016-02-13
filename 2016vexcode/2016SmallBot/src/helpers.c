/*
 * helpers.c
 *
 *  Created on: Jan 25, 2014
 *      Author: iRobotics
 */

#include <math.h>
#include "main.h"
#include "helpers.h"


/*
 * This file contains various helper functions to be used in initialization,
 * autonomous, or teleoperated modes.
 * Drive and manipulate source code is also in here.
 */

void motor(unsigned char channel, int speed)
{
	if( invertMotor[channel-1] )
	{
		motorSet( channel, (-speed) );
	}
	else
	{
		motorSet( channel, speed );
	}
}



void goStraight( int drive )
{
	/* Get the observed sensor value */
	goStraight_PID.observed = getAverageGyro();

	/* Run the PID */
	int rotate = (int) PID( &goStraight_PID );
	//printf("rotate: %d\n\r", rotate);

	/* Verify that the control is inside the allowable range */
	if( rotate > 127 )
		rotate = 127;
	else if( rotate < -127 )
		rotate = -127;

	/* Apply the control */
	arcadeDrive( drive, rotate );
}

int getAverageGyro()
{
	return gyroGet(gyro);
}

void resetAverageGyro()
{
	gyroReset(gyro);
}

int scaleJoysticks( int input )
{
	int scalefactor = 2;
	int output = 0;
	float normalizedinput = 0;
	bool inputnegative = false;

	if(input < 0)
	{
		inputnegative = true;
	}
	else
	{
		inputnegative = false;
	}
	normalizedinput = (float)abs(input)/(float)127;

	output = (int)((float)127 * (float)pow(normalizedinput, scalefactor));

	if(inputnegative == true)
	{
		output = -output;
	}
	return output;
}

bool turnViaPID( int desired )
{
	/* The setpoint comes from the caller */
	turn_PID.setpoint = desired;
	/* Get the observed gyro value */
	turn_PID.observed = getAverageGyro();
	printf("setpoint: %lf \n\r", desired);
	printf("observed: %lf \n\r", turn_PID.observed);

	/* Run the PID given the above setpoint and observed values */
	int rotate = PID( &turn_PID );
	// printf("rotate: %d \n\r", rotate);
	printf("Gyro Value: %d \n\r", getAverageGyro());

	/* Make sure the control is within the allowable range */
	if( rotate > 127 )
	{
		rotate = 127;
	}
	else if( rotate < -127 )
	{
		rotate = -127;
	}

	/* Apply the control */
	printf("rotate: %d \n\r", rotate);
	Drive( DRIVE_OFF, rotate * 0.75 );  // half speed

	/* If the observed value is within a reasonable distance
	 * of the desired value for a short period of time, then
	 * we are done.
	 */
	if( turn_PID.observed < turn_PID.setpoint + 2 &&
		turn_PID.observed > turn_PID.setpoint - 2 )
	{
		turn_PID_counter++;
	}
	else
	{
		turn_PID_counter = 0;
	}

	bool done = false;
	if( turn_PID_counter > 300/LOOP_TIME )
	{
		done = true;
	}

	return done;
}

void arcadeDrive( int drive, int rotate )
{
	int fwdrev = 0; //+ for forward, - for reverse
	int spin = 0; //+ for CW, - for CCW
	int leftmotors = 0; //speed for the left motors
	int rightmotors = 0; //speed for the right motors
	bool crot = true; //tells whether we will allow for counter-rotation of wheels or not

	fwdrev = scaleJoysticks(drive); //read joysticks, scale them approprately
	spin = scaleJoysticks(rotate);

	if(-127 < fwdrev && fwdrev < 127) //detect if we're inside or outside the counter-rotate threshold
	{
		crot = true;
	}
	else
	{
		crot = false;
	}

    leftmotors = fwdrev + spin;
    rightmotors = fwdrev - spin;

    if(!crot && fwdrev >= 0 ) //if we're not supposed to be counterrotating, make sure no motors run opposite of direction of travel
    {
    	if(leftmotors < 0)
    	{
    		leftmotors = 0;
    	}
    	if(rightmotors < 0)
    	{
    		rightmotors = 0;
    	}
    }
    else if(!crot && fwdrev <= 0 )
    {
    	if(leftmotors > 0)
    	{
    		leftmotors = 0;
    	}
    	if(rightmotors > 0)
    	{
    		rightmotors = 0;
    	}
    }

    //scale outputs to be sure we don't do anything weird to the motors
    if(leftmotors > 127)
    {
    	leftmotors = 127;
    }
    if(leftmotors < -127)
    {
    	leftmotors = -127;
    }
    if(rightmotors > 127)
    {
    	rightmotors = 127;
    }
    if(rightmotors < -127)
    {
    	rightmotors = -127;
    }

	motor( DRIVEMTRL1, leftmotors );
	motor( DRIVEMTRL2, leftmotors );
	motor( DRIVEMTRL3, leftmotors );
	motor( DRIVEMTRL4, leftmotors );

	motor( DRIVEMTRR1, rightmotors );
	motor( DRIVEMTRR2, rightmotors );
	motor( DRIVEMTRR3, rightmotors );
	motor( DRIVEMTRR4, rightmotors );
}

void TankDrive( int left, int right )
{
	motor( DRIVEMTRL1, left );
	motor( DRIVEMTRL2, left );
	motor( DRIVEMTRL3, left );
	motor( DRIVEMTRL4, left );

	motor( DRIVEMTRR1, right );
	motor( DRIVEMTRR2, right );
	motor( DRIVEMTRR3, right );
	motor( DRIVEMTRR4, right );
}

void Drive( int drive, int rotate )
{
//	if( rotate == 0 && drive != 0)
//	{
//		/* Wait for the robot to stabilize before resetting the
//		 * gyro and using goStraight
//		 */
//		goStraight_timer++;
//		if( goStraight_timer > 700/LOOP_TIME )
//		{
//			goStraight( drive );
//			if( flag2 == true )
//			{
//				printf("RESET GYRO\n\r");
//				resetAverageGyro();
//				flag2 = false;
//			}
//			else
//			{
//				//printf("GOING STRAIGHT\n\r");
//				goStraight( drive );
//			}
//		}
//		else
//		{
//			arcadeDrive( drive, rotate );
//		}
//	}
//	else
//	{
		arcadeDrive( drive, rotate );
//		flag2 = true;
//		goStraight_timer = 0;
//	}
}

void Manipulate( bool intake, bool shoot)
{
	if(intake)
	{
		motor( INTKMTRS, MAX_SPEED );
	}
	else
	{
		motor( INTKMTRS, DRIVE_OFF);
	}
	if(shoot)
	{
		motor( SHTERMTRS, MAX_SPEED );
	}
	else
	{
		motor( SHTERMTRS, DRIVE_OFF);
	}
}
