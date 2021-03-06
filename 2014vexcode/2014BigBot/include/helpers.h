/*
 * helpers.h
 *
 *  Created on: Jan 25, 2014
 *      Author: iRobotics
 */

#ifndef HELPERS_H_
#define HELPERS_H_

/* GLOBAL VARIABLES */

// Use this array to invert the motors (0 = normal, 1 = invert)
static unsigned char invertMotor[10] = { 0,
										 0,
										 0,
										 0,
										 1,
										 1,
										 1,
								  	  	 1,
								  	  	 1,
								  	  	 1 };
static bool flag;
static bool flag2;
static int turn_PID_counter = 0;
static int goStraight_timer = 0;

/* FUNCTION PROTOTYPES */

void motor(unsigned char channel, int speed);
int joystickDeadband( int joystick, float percent );
int getAverageGyro();
void resetAverageGyro();
void tankDrive( int left, int right );
int scaleJoysticks( int input );
void arcadeDrive( int DriveY, int Rotate );
bool followLine( int speed );
bool isWhite( unsigned char Sensor );
void goStraight( int drive );
bool turnViaPID( int desired );
bool Turn( int degrees );
void Drive(int drive, int rotate);
void Manipulate( int arm, int conveyor, bool override, int armoverride, bool recalibrate, bool mliftup, bool mliftdown );


#endif /* HELPERS_H_ */
