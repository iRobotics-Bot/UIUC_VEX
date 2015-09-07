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

static int ime_val_right = 0;
static int ime_val_left = 0;

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

void liftcontrol(int joy_val_LY)
{
	imeGet(IME_RIGHT, &ime_val_right);
	imeGet(IME_LEFT, &ime_val_left);
	if(joy_val_LY < 0) {
		if (abs(ime_val_right) - abs(ime_val_left) > 25) {
			printf("right imeval1: %d      imeval2: %d\r\n", abs(ime_val_right), abs(ime_val_left));
			runMotor(1 , 0);
			runMotor(2 , 0);
			runMotor(3 , 0);
			runMotor(4 , 0);
			runMotor(5 , 0);
			runMotor(6 , joy_val_LY);
			runMotor(7 , joy_val_LY);
			runMotor(8 , joy_val_LY);
			runMotor(9 , joy_val_LY);
			runMotor(10 , joy_val_LY);
		} else if (abs(ime_val_left) - abs(ime_val_right) > 25) {
			printf("left imeval1: %d      imeval2: %d\r\n", abs(ime_val_right), abs(ime_val_left));
			runMotor(1 , joy_val_LY);
			runMotor(2 , joy_val_LY);
			runMotor(3 , joy_val_LY);
			runMotor(4 , joy_val_LY);
			runMotor(5 , joy_val_LY);
			runMotor(6 , 0);
			runMotor(7 , 0);
			runMotor(8 , 0);
			runMotor(9 , 0);
			runMotor(10 , 0);
		} else {
			printf("imeval1: %d      imeval2: %d\r\n", abs(ime_val_right), abs(ime_val_left));
			runMotor(1 , joy_val_LY);
			runMotor(2 , joy_val_LY);
			runMotor(3 , joy_val_LY);
			runMotor(4 , joy_val_LY);
			runMotor(5 , joy_val_LY);
			runMotor(6 , joy_val_LY);
			runMotor(7 , joy_val_LY);
			runMotor(8 , joy_val_LY);
			runMotor(9 , joy_val_LY);
			runMotor(10 , joy_val_LY);
		}
	} else {
		if (abs(ime_val_right) - abs(ime_val_left) > 25) {
			printf("right imeval1: %d      imeval2: %d\r\n", abs(ime_val_right), abs(ime_val_left));
			runMotor(1 , joy_val_LY);
			runMotor(2 , joy_val_LY);
			runMotor(3 , joy_val_LY);
			runMotor(4 , joy_val_LY);
			runMotor(5 , joy_val_LY);
			runMotor(6 , 0);
			runMotor(7 , 0);
			runMotor(8 , 0);
			runMotor(9 , 0);
			runMotor(10 , 0);
		} else if (abs(ime_val_left) - abs(ime_val_right) > 25) {
			printf("left imeval1: %d      imeval2: %d\r\n", abs(ime_val_right), abs(ime_val_left));
			runMotor(1 , 0);
			runMotor(2 , 0);
			runMotor(3 , 0);
			runMotor(4 , 0);
			runMotor(5 , 0);
			runMotor(6 , joy_val_LY);
			runMotor(7 , joy_val_LY);
			runMotor(8 , joy_val_LY);
			runMotor(9 , joy_val_LY);
			runMotor(10 , joy_val_LY);
		} else {
			printf("imeval1: %d      imeval2: %d\r\n", abs(ime_val_right), abs(ime_val_left));
			runMotor(1 , joy_val_LY);
			runMotor(2 , joy_val_LY);
			runMotor(3 , joy_val_LY);
			runMotor(4 , joy_val_LY);
			runMotor(5 , joy_val_LY);
			runMotor(6 , joy_val_LY);
			runMotor(7 , joy_val_LY);
			runMotor(8 , joy_val_LY);
			runMotor(9 , joy_val_LY);
			runMotor(10 , joy_val_LY);
		}
	}
}

void TankDrive( int left, int right )
{
	runMotor( 2, left );
	runMotor( 3, left );
	runMotor( 8, right );
	runMotor( 9, right );
}

