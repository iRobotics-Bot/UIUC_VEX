/*
 * helpers.h
 *
 *  Created on: Jan 25, 2014
 *      Author: iRobotics
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include "main.h"
#include "pid.h"
/* MACROS */

/*** Joystick Macros ***/
#define		DRIVER		1
#define		MANIP		2

#define		JOYLY		3
#define		JOYLX		4
#define		JOYRY		2
#define		JOYRX		1

#define		BUTTONS_L	7
#define 	BUTTONS_R	8
#define 	TRIGGER_L	5
#define 	TRIGGER_R	6
/***********************/

/*** UNIVERSAL AUTONOMOUS MACROS ***/
#define	RED					1
#define	BLUE				2
/**********************************/

/*** DRIVE MACROS ***/
#define	DRIVE_OFF			0
#define		LOOP_TIME	100UL
/********************/

/***   Motor Macros   ***/
#define		DRIVEMTRL1	 1
#define		DRIVEMTRL2	 2
#define		DRIVEMTRL3	 3
#define		DRIVEMTRL4	 4
#define		DRIVEMTRR1	 5
#define		DRIVEMTRR2	 6
#define		DRIVEMTRR3	 7
#define		DRIVEMTRR4	 8
#define		SHTERMTRS	 9
#define		INTKMTRS	 10
#define		MAX_SPEED	 127
#define		HALF_SPEED	(127/2)
/************************/

/* GLOBAL VARIABLES */

static int turn_PID_counter = 0;
static int goStraight_timer = 0;
PID_t arm_PID_L, arm_PID_R, goStraight_PID, turn_PID;
Gyro  gyro;


// Use this array to invert the motors (0 = normal, 1 = invert)
static unsigned char invertMotor[10] = { 1,   // motor 1
										 0,   // motor 2
										 0,   // motor 3
										 0,   // motor 4
										 1,   // motor 5
										 1,   // motor 6
								  	  	 1,   // motor 7
								  	  	 1,   // motor 8
								  	  	 0,   // motor 9
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
void Manipulate( bool intake, bool shoot);


#endif /* HELPERS_H_ */
