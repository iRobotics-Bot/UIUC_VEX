/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions related to it.
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
#include "helpers.h"

/*
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO()
{
	pinMode( CYLINDER_ANTENNA, OUTPUT );
	pinMode( CYLINDER_CATAPULT1, OUTPUT );
	pinMode( CYLINDER_CATAPULT2, OUTPUT );
	pinMode( CYLINDER_CATAPULT3, OUTPUT );

	digitalWrite( CYLINDER_ANTENNA, LOW );
	digitalWrite( CYLINDER_CATAPULT1, LOW );
	digitalWrite( CYLINDER_CATAPULT2, LOW );
	digitalWrite( CYLINDER_CATAPULT3, LOW );
}

/*
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
void initialize()
{
	arm_PID_L.observed = 0;
	arm_PID_L.setpoint = LOWER_LIMIT_L;
	arm_PID_L.p_gain = 0.13;
	arm_PID_L.i_gain = 0.00000;
	arm_PID_L.d_gain = 0.01;
	arm_PID_L.p_frequency = 0;
	arm_PID_L.i_frequency = 0;
	arm_PID_L.d_frequency = 2;
	arm_PID_L.integrated_error = 0;
	arm_PID_L.prev_error_for_d = 0;
	arm_PID_L.p_count = 0;
	arm_PID_L.i_count = 0;
	arm_PID_L.d_count = 0;
	arm_PID_L.prevSetpoint = 0;

	arm_PID_R.observed = 0;
	arm_PID_R.setpoint = LOWER_LIMIT_R;
	arm_PID_R.p_gain = 0.13;
	arm_PID_R.i_gain = 0.00000;
	arm_PID_R.d_gain = 0.01;
	arm_PID_R.p_frequency = 0;
	arm_PID_R.i_frequency = 0;
	arm_PID_R.d_frequency = 2;
	arm_PID_R.integrated_error = 0;
	arm_PID_R.prev_error_for_d = 0;
	arm_PID_R.p_count = 0;
	arm_PID_R.i_count = 0;
	arm_PID_R.d_count = 0;
	arm_PID_R.prevSetpoint = 0;

	goStraight_PID.observed = 0;
	goStraight_PID.setpoint = 0;
	goStraight_PID.p_gain = 4.5;
	goStraight_PID.i_gain = 0.00000;
	goStraight_PID.d_gain = 1.0;
	goStraight_PID.p_frequency = 0;
	goStraight_PID.i_frequency = 0;
	goStraight_PID.d_frequency = 2;
	goStraight_PID.integrated_error = 0;
	goStraight_PID.prev_error_for_d = 0;
	goStraight_PID.p_count = 0;
	goStraight_PID.i_count = 0;
	goStraight_PID.d_count = 0;
	goStraight_PID.prevSetpoint = 0;

	turn_PID.observed = 0;
	turn_PID.setpoint = 0;
	turn_PID.p_gain = 4.7;
	turn_PID.i_gain = 0.00000;
	turn_PID.d_gain = 0.0;
	turn_PID.p_frequency = 0;
	turn_PID.i_frequency = 0;
	turn_PID.d_frequency = 2;
	turn_PID.integrated_error = 0;
	turn_PID.prev_error_for_d = 0;
	turn_PID.p_count = 0;
	turn_PID.i_count = 0;
	turn_PID.d_count = 0;
	turn_PID.prevSetpoint = 0;


	analogCalibrate( LEFTPOT );
	analogCalibrate( RIGHTPOT );

	/* initialize all motors to 0 */
	motor( DRIVEMTRL1, STOP );
	motor( DRIVEMTRL2, STOP );
	motor( DRIVEMTRL3, STOP );
	motor( DRIVEMTRL4, STOP );

	motor( DRIVEMTRR1, STOP );
	motor( DRIVEMTRR2, STOP );
	motor( DRIVEMTRR3, STOP );
	motor( DRIVEMTRR4, STOP );

	imeInitializeAll();

	//Initialize Gyro
	gyro = gyroInit( GYRO1, 196 );
}
