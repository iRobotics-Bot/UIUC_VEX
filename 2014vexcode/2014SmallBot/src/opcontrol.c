/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Copyright (c) 2011-2013, Purdue University ACM SIG BOTS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation	 and/or other materials provided with the distribution.
 *     * Neither the name of Purdue University ACM SIG BOTS nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "helpers.h"
#include "opcontrol.h"
#include "auto_scripts/matt.h"

/* GLOBAL VARIABLES */
int imeValue, imeVal;

/***********************************************/
/*** UNCOMMENT THIS LINE TO DEBUG AUTONOMOUS ***/
/***********************************************/
//#define DEBUG_AUTO


/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the schedular is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */


#ifdef DEBUG_AUTO
void autoMode()
#else
void operatorControl()
#endif
{
	/* DO NOT COMMENT THIS LINE OUT */
	printf("\n\r");

	while (1)
	{
		/* Get the driver joystick values */
		int djoyLY = joystickGetAnalog( DRIVER, JOYLY );
		int djoyRX = joystickGetAnalog( DRIVER, JOYRX );

		/* Drive the wheels according to driver input */
		Drive(djoyLY, djoyRX);

		/* Get the manipulator joystick values */
		/*** ONE DRIVER ***/
		// bool mup = joystickGetDigital( DRIVER, TRIGGER_R, JOY_UP );
		// bool mdown = joystickGetDigital( DRIVER, TRIGGER_R, JOY_DOWN );
		// bool moverride = joystickGetDigital( DRIVER, TRIGGER_L, JOY_UP );
		// bool mrecalibrate = joystickGetDigital( DRIVER, BUTTONS_R, JOY_RIGHT );
		// bool throw = joystickGetDigital( DRIVER, BUTTONS_R, JOY_DOWN ); // button 8D for throw
		// bool antenna = joystickGetDigital( DRIVER, BUTTONS_R, JOY_UP );
		// bool mhold = joystickGetDigital( DRIVER, BUTTONS_L, JOY_UP );
		/*** TWO DRIVERS ***/
		bool mup = joystickGetDigital( MANIP, TRIGGER_R, JOY_UP );
		bool mdown = joystickGetDigital( MANIP, TRIGGER_R, JOY_DOWN );
		bool moverride = joystickGetDigital( MANIP, TRIGGER_L, JOY_UP );
		int mjoyLY = joystickGetAnalog( MANIP, JOYLY );
		bool mrecalibrate = joystickGetDigital( MANIP, BUTTONS_R, JOY_RIGHT );
		bool throw = joystickGetDigital( MANIP, BUTTONS_R, JOY_DOWN );
		bool antenna = joystickGetDigital( MANIP, BUTTONS_R, JOY_UP );
		bool mhold = joystickGetDigital( MANIP, BUTTONS_L, JOY_UP );

		/* Control the arm according to driver input */
		int arm;
		if( mup )
		{
			arm = 127;
		}
		else if( mdown )
		{
			arm = -127;
		}
		else
		{
			arm = 0;
		}
		//Manipulate( arm, throw, antenna, moverride, arm, mrecalibrate, mhold );		// ONE DRIVER
		Manipulate( arm, throw, antenna, moverride, mjoyLY, mrecalibrate, mhold );	// TWO DRIVERS


		/* Tuning stuff -- comment out when not needed */
		//int joy = joystickGetAnalog( DRIVER, JOYLY );
		//motor( 5, joy );
		//printf("joystick = %d\n", joy);
		//imeGet( LEFT_IME, &imeValue);
		//imeGet( RIGHT_IME, &imeVal);
		//printf("LEFT IME: %d \n\r", imeValue);
		//printf("RITE IME: %d \n\r", imeVal);
		//if(joystickGetDigital(DRIVER, 8, JOY_DOWN)){
		//	imeReset( LEFT_IME );
		//	imeReset( RIGHT_IME );
		//}
		//printf("left pot = %d   right pot = %d\n\r", analogReadCalibrated( LEFTPOT ), analogReadCalibrated( RIGHTPOT ) );
		//printf("left pot raw = %d\n\r", analogRead( LEFTPOT ) );

		/* DO NOT COMMENT THIS OUT */
		delay(LOOP_TIME);
	}
}


#ifdef DEBUG_AUTO
void operatorControl()
#else
void autoMode()
#endif
{
		resetAverageGyro();
		bool done = FALSE;
		while( 1 )
		{
			// printf("FUCK VEX WITH SOMETHING SANDPAPERY\n\r");

			if( joystickGetDigital( DRIVER, BUTTONS_R, JOY_UP) )
			{
				run_auto = TRUE;
			}
			else if( joystickGetDigital( DRIVER, BUTTONS_R, JOY_DOWN) )
			{
				run_auto = FALSE;
			}


			if( run_auto )
			{
				//done = turnViaPID(81);
				GoMidterminators( BLUE );
			}

			else
			{
				Drive( DRIVE_OFF, ROTATE_OFF );
				Manipulate( ARM_HOLD, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_ON, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			}
			if(done) break;

		/* DO NOT COMMENT THIS OUT */
		delay(LOOP_TIME);

	}
}
