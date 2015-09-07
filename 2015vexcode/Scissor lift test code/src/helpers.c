/* This file contains helper functions */ 

#include <math.h>
#include "main.h"
#include "helpers.h"


/*	Runs a motor. Direction is determined from array in helper.h 
 *	Inputs: channel - the motor port 
 *			speed - speed of motor from 0 to 127
 *	Outputs: none
 *  Return value: none										
 */ 
void runMotor(unsigned char channel, int speed)
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

/*	Sets deadband from joystick input. Used with joystickGetAnalog()
 *	Inputs: joystick - joystick value  
 *			speed - speed of motor from 0 to 127
 *	Outputs: none
 *  Return value: The deadband applied joystick value									
 */ 
int joystickDeadband(int joystick, float percent)
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

/*	Run all motors from a deadband applied joystick value
 *	Inputs: joy_val - joystick value  
 *	Outputs: none
 *  Return value: none										
 */ 
void runAllMotors(int joy_val_LY, int joy_val_RY)
{
	runMotor(1 , joy_val_LY);
	runMotor(2 , joy_val_LY);
	runMotor(3 , joy_val_LY);
	runMotor(4 , joy_val_LY);
	runMotor(5 , joy_val_LY);
	runMotor(6 , joy_val_RY);
	runMotor(7 , joy_val_RY);
	runMotor(8 , joy_val_RY);
	runMotor(9 , joy_val_RY);
	runMotor(10 , joy_val_RY);
}
