/*
 * helpers.h
 *
 *  Created on: Jan 25, 2014
 *      Author: iRobotics
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include "main.h"

/* GLOBAL VARIABLES */

static bool dump_flag;
static int i = 0;
static bool flag2;
static bool throw_flag = FALSE;
static int throw_counter = 0;
static int turn_PID_counter = 0;
static int goStraight_timer = 0;


// Use this array to invert the motors (0 = normal, 1 = invert)
static unsigned char invertMotor[10] = { 1,   // motor 1
										 0,   // motor 2
										 0,   // motor 3
										 0,   // motor 4
										 0,   // motor 5
										 1,   // motor 6
								  	  	 1,   // motor 7
								  	  	 1,   // motor 8
								  	  	 1,   // motor 9
								  	  	 0 }; // motor 10

/* FUNCTION PROTOTYPES */

/**
 * This is the generic drive function. It takes in the parameters drive which is the value of
 * the power forward or backward. It also takes in the rotation value. This function ensures a
 * straight driving method to stay on track.
 *
 * @param drive this is the value of the forward or backward you want. Values range from -127
 * to 127 I think. 127 should be full speed forward and -127 is backward
 * @param rotate this is the rotation value and it takes -127 to 127 as well. Those are the max
 * turning speeds. I'm not entirely sure at this point in time what direction is positive,
 * right or left?
 */
void Drive(int drive, int rotate);
/**
 * Not entirely sure what this does. I know that it plays a role in the inversion of motor
 * power based on the array above
 */
void motor(unsigned char channel, int speed);
/**
 * This function uses a power value -127 to 127 for the speed at which it drives straight. This
 * function uses a gyro to attempt a straight drive.
 *
 * @param the velocity at which it drives Straight
 */
void goStraight( int drive );
/**
 * This function is used within Drive and it is the basic arcade drive scheme. It doesn't have
 * any special features that ensures straighter go straight.
 *
 * @param drive this is the value of the forward or backward you want. Values range from -127
 * to 127 I think. 127 should be full speed forward and -127 is backward
 * @param rotate this is the rotation value and it takes -127 to 127 as well. Those are the max
 * turning speeds. I'm not entirely sure at this point in time what direction is positive,
 * right or left?
 */
void arcadeDrive( int drive, int rotate);
/**
 * This gets the value of the gyro's angle
 */
int getAverageGyro();
/**
 * This resets the relative angle of the gyro to zero and subsequent "gets" are relative to
 * this angle
 */
void resetAverageGyro();
/**
 * This function modifies the input of the Joysticks and outputs a different expotential
 * scaling of the initial values
 *
 * @param input the origial you want to scale
 * @return this is the scaled value
 */
int scaleJoysticks( int input );

bool turnViaPID( int desired );

/**
 * This is the tank drive that is not used but each parameter controls the power sent to that
 * side of the drivetrain
 *
 * @param left this is the left power values -127 to 127
 * @param right this is the right power value like left
 */
void TankDrive( int left, int right );
/**
 * This is the Manipulator function controlling all the different aspects of other robot
 * functionality
 *
 * @param arm this esentially takes in 3 different values which refer to different positions of
 * the arm, 127 is folded, 0 is a neutral carrying position, -127 is down to pick up the ball
 * @param throw this is the bool that triggers the catapult to lauch
 * @param antenna this is the bool that triggers the release of the antennas to touch the
 * hangbar
 * @param override this overrides the arm values and it allows the driver to determine what
 * angle he or she wants
 * @param override_val this simply changes the set point to a custom value I believe
 * @param recalibrate this allows the driver to recalibrate the potentiometers of the arm.
 */
void Manipulate( int arm, bool throw, bool antenna, bool override, int override_val, bool recalibrate, bool hold_ball );


#endif /* HELPERS_H_ */
