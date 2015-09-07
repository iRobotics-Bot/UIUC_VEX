/** @file main.h
 * @brief Header file for global functions
 * 
 * Any experienced C or C++ programmer knows the importance of header files. For those who
 * do not, a header file allows multiple files to reference functions in other files without
 * necessarily having to see the code (and therefore causing a multiple definition). To make
 * a function in "opcontrol.c", "auto.c", "main.c", or any other C file visible to the core
 * implementation files, prototype it here.
 *
 * This file is included by default in the predefined stubs in each VEX Cortex PROS Project.
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

#ifndef MAIN_H_

// This prevents multiple inclusion, which isn't bad for this file but is good practice
#define MAIN_H_

#include <API.h>
#include <pid.h>

// Allow usage of this file in C++ programs
#ifdef __cplusplus
extern "C" {
#endif


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

/***   Motor Macros   ***/
#define		DRIVEMTRL1	 1
#define		DRIVEMTRL2	 3
#define		DRIVEMTRL3	 4
#define		DRIVEMTRL4	 5
#define		DRIVEMTRR1	 6
#define		DRIVEMTRR2	 7
#define		DRIVEMTRR3	 8
#define		DRIVEMTRR4	10
#define		ARMMTRL		 2
#define		ARMMTRR		 9

/* NOTE: Motors can be "inverted" in software
		 using the inversion array in helpers.h */

#define		LOWER_LIMIT_L	-2950
#define		LOWER_LIMIT_R	-2950
#define		CARRY_LIMIT_L	-2400
#define		CARRY_LIMIT_R	-2400
#define		DUMP_LIMIT_L	-1200
#define		DUMP_LIMIT_R	-1200
#define		UPPER_LIMIT_L	0
#define		UPPER_LIMIT_R	0
/*************************/

/***  SENSOR MACROS  ***/
#define		LEFTPOT		1
#define		RIGHTPOT	2
#define		LEFT_IME	1
#define		RIGHT_IME	0

/*** GYRO MACROS ***/
#define 	GYRO1		3

/*** CYLINDER MACROS ***/
#define CYLINDER_ANTENNA		1
#define CYLINDER_CATAPULT1		2
#define CYLINDER_CATAPULT2		3
#define CYLINDER_CATAPULT3		4

/*** UNIVERSAL AUTONOMOUS MACROS ***/
#define	RED					1
#define	BLUE				2

#define	DRIVE_FORWARD		100
#define	DRIVE_FORWARD_SLOW	70
#define	DRIVE_FORWARD_FAST	127
#define	DRIVE_REVERSE		-100
#define	DRIVE_REVERSE_SLOW	-70
#define	DRIVE_OFF			0
#define ROTATE_RIGHT		90
#define	ROTATE_LEFT			-90
#define	ROTATE_OFF			0

#define	ARM_UP				127
#define	ARM_DOWN			-127
#define	ARM_HOLD			0
#define	CATAPULT_HOLD		0
#define	CATAPULT_LAUNCH		1
#define	ANTENNA_HOLD		0
#define	OVERRIDE_ON			1
#define	OVERRIDE_OFF		0
#define	OVERRIDE_ARM_HOLD	0
#define RECALIBRATE_OFF		0
#define	HOLD_OFF			0
/**********************************/

/***   OTHER MACROS  ***/
#define		LOOP_TIME	100UL
#define		TRUE		1
#define		FALSE		0
#define 	STOP 		0

/* GLOBAL VARIABLES */

PID_t arm_PID_L, arm_PID_R, goStraight_PID, turn_PID;
Gyro  gyro;


/* FUNCTION PROTOTYPES */

// Prototypes for initialization, operator control and autonomous

/**
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */
void autonomous();
/**
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO();
/**
 * Runs user initialization code. This function will be started in its own task with the default
 * priority and stack size once when the robot is starting up. It is possible that the VEXnet
 * communication link may not be fully established at this time, so reading from the VEX
 * Joystick may fail.
 *
 * This function should initialize most sensors (gyro, encoders, ultrasonics), LCDs, global
 * variables, and IMEs.
 *
 * This function must exit relatively promptly, or the operatorControl() and autonomous() tasks
 * will not start. An autonomous mode selection menu like the pre_auton() in other environments
 * can be implemented in this task if desired.
 */
void initialize();
/**
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
void operatorControl();

// End C++ export structure
#ifdef __cplusplus
}
#endif

#endif
