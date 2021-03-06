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
 *       documentation and/or other materials provided with the distribution.
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
#include "opcontrol.h"
#include "helpers.h"
#include "pid.h"
#include "math.h"
#include "auto_scripts/matt.h"

/***********************************************/
/*** UNCOMMENT THIS LINE TO DEBUG AUTONOMOUS ***/
/****** *****************************************/
//#define	DEBUG_AUTO

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
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
#ifdef DEBUG_AUTO
void operatorControl1()
#else
void operatorControl()
#endif
{
	printf("\n\r");

	while (1)
	{
		/* Get the driver joystick values */
		int djoyLY = joystickDeadband( joystickGetAnalog( DRIVER, JOYLY ), 0.05 );
		int djoyRY = joystickDeadband( joystickGetAnalog( DRIVER, JOYRY ), 0.05 );
		int djoyRX = joystickDeadband( joystickGetAnalog( DRIVER, JOYRX ), 0.05 );

		/* Drive the wheels according to driver input */
		//printf("djoyRX = %d\n\r", djoyRX);
		Drive( djoyLY, djoyRX );

		/* Get the manipulator joystick values */
		/*** ONE DRIVER ***/
		//bool marmup = joystickGetDigital( DRIVER, 6, JOY_UP );
		//bool marmdown = joystickGetDigital( DRIVER, 6, JOY_DOWN );
		//bool mconveyorout = joystickGetDigital( DRIVER, 5, JOY_UP );
		//bool mconveyorin = joystickGetDigital( DRIVER, 5, JOY_DOWN );
		//bool moverride = joystickGetDigital( DRIVER, 8, JOY_DOWN );
		//bool mrecalibrate = joystickGetDigital( DRIVER, 8, JOY_RIGHT );
		//bool mliftup = joystickGetDigital( DRIVER, 7, JOY_UP );
		//bool mliftdown = joystickGetDigital( DRIVER, 7, JOY_DOWN);

		//bool resetgyro1 = joystickGetDigital( DRIVER, 7, JOY_LEFT );
		//bool resetgyro2 = joystickGetDigital( DRIVER, 7, JOY_RIGHT );
		//bool resetgyroboth = joystickGetDigital( DRIVER, 7, JOY_DOWN );

		/*** TWO DRIVERS ***/
		bool marmup = joystickGetDigital( MANIP, 6, JOY_UP );
		bool marmdown = joystickGetDigital( MANIP, 6, JOY_DOWN );
		bool mconveyorout = joystickGetDigital( MANIP, 5, JOY_UP );
		bool mconveyorin = joystickGetDigital( MANIP, 5, JOY_DOWN );
		bool moverride = joystickGetDigital( MANIP, 8, JOY_DOWN );
		int mjoyLY = joystickGetAnalog( MANIP, JOYLY );
		bool mrecalibrate = joystickGetDigital( MANIP, 8, JOY_RIGHT );
		bool mliftup = joystickGetDigital( DRIVER, 8, JOY_UP );
		bool mliftdown = joystickGetDigital( DRIVER, 8, JOY_DOWN);

		/* Control the arm according to driver input */
		int arm = 0, conveyor = 0;
		if( marmup )
		{
			arm = 127;
		}
		else if( marmdown )
		{
			arm = -127;
		}
		if( mconveyorout )
		{
			conveyor = 127;
		}
		else if( mconveyorin )
		{
			conveyor = -127;
		}

		//Manipulate( arm, conveyor, moverride, arm, mrecalibrate, mliftup, mliftdown ); 	/* ONE DRIVER */
		Manipulate( arm, conveyor, moverride, mjoyLY, mrecalibrate , mliftup, mliftdown );	/* TWO DRIVERS */

		/* Stuff to use for tuning */
		//int joy = joystickGetAnalog( DRIVER, JOYLY );
		//motor( 7, joy );
		//printf("joystick = %d\n", joy);
		//printf("left pot = %d\n\r",  analogReadCalibrated( LEFTPOT ) );
		//printf("right pot = %d\n\r",  analogReadCalibrated( RIGHTPOT ) );
		//if(resetgyro1)
		//	gyroReset(gyro1);
		//if(resetgyro2)
		//	gyroReset(gyro2);
		//if(resetgyroboth)
		//	resetAverageGyro();
		//printf("gyro1 = %d |||| gyro2 = %d |||| average = %d\n\r", gyroGet(gyro1), gyroGet(gyro2), getAverageGyro());
		/* DO NOT COMMENT THIS OUT */
		delay(LOOP_TIME);

	}
}

#ifdef DEBUG_AUTO
void operatorControl()
#else
void operatorControl1()
#endif
{
	//printf("FUCK VEX WITH A HUGE DICK!\n\r");
	resetAverageGyro();
	bool done = FALSE;
	int left_ime = 0;

	while( 1 )
	{
		//imeGet(LEFT_IME, &left_ime);
		//printf("left ime = %d\n\r", left_ime);

		//printf("Joystick button = %d\n\r", joystickGetDigital( DRIVER, BUTTONS_R, JOY_UP ) );

		if( joystickGetDigital( DRIVER, BUTTONS_R, JOY_UP) )
		{
			run_auto = TRUE;
		}
		else if( joystickGetDigital( DRIVER, BUTTONS_R, JOY_DOWN) )
		{
			run_auto = FALSE;
		}

		//printf( "gyro = %d\n\r", getAverageGyro() );
		if( run_auto )
		{
			//printf("GO!\n\r");
			//done = Turn( -90 );
			GoMidterminators( BLUE );
		}

		else
		{
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
		}

		if( done )
			break;

		/* DO NOT COMMENT THIS OUT */
		delay(LOOP_TIME);
	}

	printf("DONE!\n\r");
}
