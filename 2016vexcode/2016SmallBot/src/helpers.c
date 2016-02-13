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

void Manipulate( int arm, bool throw, bool antenna, bool override, int override_val, bool recalibrate, bool hold_ball )
{
//	//motor( ARMMTRL, arm );
//	//motor( ARMMTRR, arm );
//
//	/* Get the observed value from the potentiometers */
//	arm_PID_L.observed = (double) analogReadCalibrated( LEFTPOT );
//	arm_PID_R.observed = (double) analogReadCalibrated( RIGHTPOT );
//	//printf("PID_L: %f \n\r", arm_PID_L.observed);   // read pot vals; uncomment when needed
//	//printf("PID_R: %f \n\r", arm_PID_R.observed);
//
//	/* Change the setpoints based on input */
//	if( arm < 100 )
//	{
//		i = 0;
//	}
//
//	if( arm < -100 )
//	{
//		arm_PID_L.setpoint = LOWER_LIMIT_L;
//		arm_PID_R.setpoint = LOWER_LIMIT_R;
//
//		dump_flag = FALSE;
//	}
//	else if( arm > 100 )
//	{
//		/* Debounce the "up" button */
//		i++;
//		if( i > 100/LOOP_TIME )
//		{
//			dump_flag = TRUE;
//		}
//		arm_PID_L.setpoint = UPPER_LIMIT_L;
//		arm_PID_R.setpoint = UPPER_LIMIT_R;
//	}
//	else if( dump_flag == TRUE )
//	{
//		if( (arm_PID_L.observed > DUMP_LIMIT_L - 30) &&
//			(arm_PID_R.observed > DUMP_LIMIT_R - 30) )
//		{
//			dump_flag = FALSE;
//		}
//		arm_PID_L.setpoint = DUMP_LIMIT_L;
//		arm_PID_R.setpoint = DUMP_LIMIT_R;
//	}
//	else
//	{
//		arm_PID_L.setpoint = CARRY_LIMIT_L;
//		arm_PID_R.setpoint = CARRY_LIMIT_R;
//	}
//	if( hold_ball == TRUE )
//	{
//		arm_PID_L.setpoint = DUMP_LIMIT_L;
//		arm_PID_R.setpoint = DUMP_LIMIT_R;
//	}
//	if( throw_flag == TRUE )
//	{
//		arm_PID_L.setpoint = CARRY_LIMIT_L;
//		arm_PID_R.setpoint = CARRY_LIMIT_R;
//	}
//
//	/* Run the PID */
//	int arm_l = -(int)PID( &arm_PID_L );
//	int arm_r = -(int)PID( &arm_PID_R );
//
//	/* Verify that the control is inside the allowable range */
//	if( arm_l > 127 )
//	{
//		arm_l = 127;
//	}
//	else if( arm_l < -127 )
//	{
//		arm_l = -127;
//	}
//	if( arm_r > 127 )
//	{
//		arm_r = 127;
//	}
//	else if( arm_r < -127 )
//	{
//		arm_r = -127;
//	}
//
//	/* Apply the control */
//	if( !override && !recalibrate )
//	{
//		motor( ARMMTRL, arm_l );
//		motor( ARMMTRR, arm_r );
//	}
//	/* Emergency override control */
//	else if( !recalibrate )
//	{
//		motor( ARMMTRL, override_val );
//		motor( ARMMTRR, override_val );
//	}
//	/* Run recalibration */
//	else
//	{
//		motor( ARMMTRL, 0 );
//		motor( ARMMTRR, 0 );
//		analogCalibrate( LEFTPOT );
//		analogCalibrate( RIGHTPOT );
//		delay( 500000 );
//	}
//
//	/* Throw the ball if the arm is out of the way */
//	if( !hold_ball )
//	{
//		if( throw )
//		{
//			throw_flag = TRUE;
//		}
//
//		if( throw_flag && arm_PID_L.observed < CARRY_LIMIT_L + 40 && arm_PID_R.observed < CARRY_LIMIT_R + 40 )
//		{
//			if( throw_counter < 700/LOOP_TIME )
//			{
//				throw_counter++;
//				digitalWrite(2, HIGH);
//				digitalWrite(3, HIGH);
//				digitalWrite(4, HIGH);
//			}
//			else
//			{
//				throw_counter = 0;
//				throw_flag = FALSE;
//				digitalWrite(2, LOW);
//				digitalWrite(3, LOW);
//				digitalWrite(4, LOW);
//			}
//		}
//		else
//		{
//			digitalWrite(2, LOW);
//			digitalWrite(3, LOW);
//			digitalWrite(4, LOW);
//		}
//	}
//	else
//	{
//		digitalWrite(2, HIGH);
//		digitalWrite(3, LOW);
//		digitalWrite(4, LOW);
//	}
//
//	/* Release the antennas if the command is given */
//	if( antenna )
//	{
//		printf("RELEASE ANTENNAS\n\r");
//		digitalWrite( CYLINDER_ANTENNA, HIGH );
//	}
}
