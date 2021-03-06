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
 */


/*********************************************************/
/***                  GENERIC HELPERS                  ***/
/*********************************************************/

void motor( unsigned char channel, int speed )
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

int joystickDeadband( int joystick, float percent )
{
	int joy_out = joystick;

	if( joystick > 127.0*(1.0-percent) )
	{
		joy_out = 127;
	}
	else if( joystick < -127.0*(1.0-percent) )
	{
		joy_out = -127;
	}
	else if( joystick < 127.0*percent && joystick > -127.0*percent )
	{
		joy_out = 0;
	}

	return joy_out;
}

int getAverageGyro()
{
	return (gyroGet(gyro1) + gyroGet(gyro2)) / 2;
}

void resetAverageGyro()
{
	printf("RESET GYRO!\n\r");
	gyroReset(gyro1);
	gyroReset(gyro2);
}

/*********************************************************/
/*********************************************************/
/*********************************************************/


/*********************************************************/
/***                   DRIVE HELPERS                   ***/
/*********************************************************/

/* UNUSED */
void tankDrive( int left, int right )
{
	motor( DRIVEMTRL1, left );
	motor( DRIVEMTRL2, left );
	motor( DRIVEMTRL3, left );

	motor( DRIVEMTRR1, right );
	motor( DRIVEMTRR2, right );
	motor( DRIVEMTRR3, right );
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

	motor( DRIVEMTRR1, rightmotors );
	motor( DRIVEMTRR2, rightmotors );
	motor( DRIVEMTRR3, rightmotors );
}

bool followLine( int speed ) // Black/gray = 0, white = 1
{
	if( !isWhite( LEFT_IR ) && !isWhite( MIDDLE_IR ) && !isWhite( RIGHT_IR ) ) // 000
	{
		Drive( 0, 0);
	}
	if( !isWhite( LEFT_IR ) &&  isWhite( RIGHT_IR ) ) //0_1
	{
		Drive( speed, 127);
	}
	if( !isWhite( LEFT_IR ) && isWhite( MIDDLE_IR ) && !isWhite( RIGHT_IR ) ) //010
	{
		Drive( speed, 0);
	}
	if( isWhite( LEFT_IR ) && !isWhite( RIGHT_IR ) ) //1_0
	{
		Drive( speed, -127);
	}
	if( isWhite( LEFT_IR ) && isWhite( MIDDLE_IR ) && isWhite( RIGHT_IR ) ) //111
	{
		Drive( 0,0);
		return 1;
	}
	return 0;
}

bool isWhite( unsigned char Sensor )
{
	if( analogRead( Sensor ) > 2048 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void goStraight( int drive )
{
	/* Get the observed sensor value */
	goStraight_PID.observed = getAverageGyro();

	/* Run the PID */
	int rotate = (int) PID( &goStraight_PID );

	/* Verify that the control is inside the allowable range */
	if( rotate > 127 )
		rotate = 127;
	else if( rotate < -127 )
		rotate = -127;

	/* Apply the control */
	arcadeDrive( drive, rotate );
}

bool turnViaPID( int desired )
{
	/* Adjust the dynamic gains */
	if( desired > 0 )
	{
		turn_PID.p_gain = TURN_P_LEFT;
		turn_PID.i_gain = TURN_I_LEFT;
		turn_PID.d_gain = TURN_D_LEFT;
	}
	else
	{
		turn_PID.p_gain = TURN_P_RIGHT;
		turn_PID.i_gain = TURN_I_RIGHT;
		turn_PID.d_gain = TURN_D_RIGHT;
	}

	/* The setpoint comes from the caller */
	turn_PID.setpoint = desired;
	/* Get the observed gyro value */
	turn_PID.observed = getAverageGyro();

	/* Run the PID given the above setpoint and observed values */
	double rotate = PID( &turn_PID );

	/* Make sure the control is within the allowable range */
	if( rotate < ROTATE_LEFT )
	{
		rotate = ROTATE_LEFT;
	}
	else if( rotate > ROTATE_RIGHT )
	{
		rotate = ROTATE_RIGHT;
	}

	/* Apply the control */
	printf("rotate = %d\n\r", (int) rotate);
	Drive( 1, (int) rotate );

	/* If the observed value is within a reasonable distance
	 * of the desired value for a short period of time, then
	 * we are done.
	 */
	if( turn_PID.observed < turn_PID.setpoint + 4 &&
		turn_PID.observed > turn_PID.setpoint - 4 )
	{
		turn_PID_counter++;
	}
	else
	{
		turn_PID_counter = 0;
	}

	bool done = FALSE;
	if( turn_PID_counter > 300/LOOP_TIME )
	{
		done = TRUE;
	}

	return done;
}



bool Turn( int degrees )
{
	bool done = FALSE;

	/* If the robot is close to the desired angle, use PID to home in. */
	if( abs( getAverageGyro() ) > abs( degrees )*0.85 - 10 &&
		abs( getAverageGyro( )) < abs( degrees )/0.85 + 10 )
	{
		//printf("turning WITH PID\n\r");
		done = turnViaPID( degrees );
	}
	/* Otherwise, use standard turning to get close to the desired value.
	 * Once we are close, we will use PID to home in.
	 */
	else
	{
		//printf("turning withOUT PID\n\r");
		if( getAverageGyro() - degrees < 0 )
		{
			Drive( DRIVE_OFF, ROTATE_LEFT );
		}
		else
		{
			Drive( DRIVE_OFF, ROTATE_RIGHT );
		}
	}

	return done;
}


void Drive( int drive, int rotate )
{
	if( rotate == 0 && drive != 0 )
	{
		/* Wait for the robot to stabilize before resetting the
		 * gyro and using goStraight
		 */
		goStraight_timer++;
		if( goStraight_timer > 500/LOOP_TIME )
		{
			goStraight( drive );
			if( flag2 == TRUE )
			{
				//printf("RESET GYRO\n\r");
				resetAverageGyro();
				flag2 = FALSE;
			}
			else
			{
				//printf("GOING STRAIGHT\n\r");
				goStraight( drive );
			}
		}
		else
		{
			arcadeDrive( drive, rotate );
		}
	}
	else
	{
		arcadeDrive( drive, rotate );
		flag2 = TRUE;
		goStraight_timer = 0;
	}
}

/*********************************************************/
/*********************************************************/
/*********************************************************/


/*********************************************************/
/***                MANIPULATOR HELPERS                ***/
/*********************************************************/

void Manipulate( int arm, int conveyor, bool override, int armoverride, bool recalibrate, bool mliftup, bool mliftdown )
{
	/*** ARM CONTROL ***/

	/* Get the observed value from the potentiometers */
	arm_PID_L.observed = (double) analogReadCalibrated( LEFTPOT );
	arm_PID_R.observed = (double) analogReadCalibrated( RIGHTPOT );
	//double leftpot = analogReadCalibrated( LEFTPOT );
	//double rightpot = analogReadCalibrated( RIGHTPOT );
	//printf("leftpot: %d\n\r rightpot: %d\n\r", leftpot, rightpot );

	/* Change the setpoints based on input */
	if( flag && (arm_PID_L.observed < PICK_LIMIT_L - 500) &&
				(arm_PID_R.observed < PICK_LIMIT_R - 500) )
	{
		flag = FALSE;
	}

	if( arm > 100 )
	{
		flag = TRUE;
		arm_PID_L.setpoint = UPPER_LIMIT_L;
		arm_PID_R.setpoint = UPPER_LIMIT_R;
		arm_PID_L.p_gain = UPPER_LIMIT_P;
		arm_PID_R.p_gain = UPPER_LIMIT_P;
		arm_PID_L.d_gain = UPPER_LIMIT_D;
		arm_PID_R.d_gain = UPPER_LIMIT_D;
	}
	else if( (arm == 0) && (flag == TRUE)  )
	{
		arm_PID_L.setpoint = PICK_LIMIT_L;
		arm_PID_R.setpoint = PICK_LIMIT_R;
		arm_PID_L.p_gain = PICK_LIMIT_P;
		arm_PID_R.p_gain = PICK_LIMIT_P;
		arm_PID_L.d_gain = PICK_LIMIT_D;
		arm_PID_R.d_gain = PICK_LIMIT_D;
	}
	else if( arm < -100 )
	{
		arm_PID_L.setpoint = LOWER_LIMIT_L;
		arm_PID_R.setpoint = LOWER_LIMIT_R;
		arm_PID_L.p_gain = LOWER_LIMIT_P;
		arm_PID_R.p_gain = LOWER_LIMIT_P;
		arm_PID_L.d_gain = LOWER_LIMIT_D;
		arm_PID_R.d_gain = LOWER_LIMIT_D;
	}

	if( arm_PID_L.observed > PICK_LIMIT_L - 20 || arm_PID_R.observed > PICK_LIMIT_R - 20 )
	{
		if( arm < 0 )
		{
			arm_PID_L.p_gain = BOTTOM_P;
			arm_PID_L.d_gain = BOTTOM_D;
			arm_PID_R.p_gain = BOTTOM_P;
			arm_PID_R.d_gain = BOTTOM_D;
		}
		else if( arm == 0 )
		{
			arm_PID_L.p_gain = 0.00;
			arm_PID_L.d_gain = 0.00;
			arm_PID_R.p_gain = 0.00;
			arm_PID_R.d_gain = 0.00;
		}
	}

	/* Run the PID */
	int arm_l = (int) PID( &arm_PID_L );
	int arm_r = (int) PID( &arm_PID_R );

	/* Verify that the control is inside the allowable range */
	if( arm_l > 127 )
		arm_l = 127;
	else if( arm_l < -127 )
		arm_l = -127;

	if( arm_r > 127 )
		arm_r = 127;
	else if( arm_r < -127 )
		arm_r = -127;

	/* Apply the control */
	if( !override && !recalibrate )
	{
		motor( ARMMTRL, arm_l );
		motor( ARMMTRR, arm_r );
	}
	/* Emergency override control */
	else if( !recalibrate )
	{
		motor( ARMMTRL, armoverride );
		motor( ARMMTRR, armoverride );
	}
	/* Run recalibration */
	else
	{
		motor( ARMMTRL, 0 );
		motor( ARMMTRR, 0 );
		analogCalibrate( LEFTPOT );
		analogCalibrate( RIGHTPOT );
		delay( 500000 );
	}


	/*** CONVEYOR CONTROL ***/

	motor( CONVEYORMTRL, conveyor );
	motor( CONVEYORMTRR, conveyor );


	/* Lift control */

	if( mliftup )
	{
		digitalWrite( CYLINDER_BACKRIGHT, HIGH );
		digitalWrite( CYLINDER_BACKLEFT, HIGH );
		digitalWrite( CYLINDER_FRONTRIGHT, HIGH );
		digitalWrite( CYLINDER_FRONTLEFT, HIGH );
	}
	else if( mliftdown )
	{
		digitalWrite( CYLINDER_BACKRIGHT, LOW );
		digitalWrite( CYLINDER_BACKLEFT, LOW );
		digitalWrite( CYLINDER_FRONTRIGHT, LOW );
		digitalWrite( CYLINDER_FRONTLEFT, LOW );	
	}
}


/*********************************************************/
/*********************************************************/
/*********************************************************/

