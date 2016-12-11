/**
 * File for operator control code.
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Copyright (c) 2011-2014, Purdue University ACM SIG BOTS. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Purdue University ACM SIG BOTS nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 ********************************************************************************/

#include "main.h"
#include "math.h"

//autonomous drive distance deadband = 1 inch
#define DRIVE_DEADBAND 1
//arm control deadband = 5 degrees
#define ARM_DEADBAND 5

//#define driveL 2
//#define driveR 3
//#define armL 4
//#define armR 5
//#define grabberL 6
//#define grabberR 7
/**
 * Controller Mapping
 *
 */

/**
 * Runs the user operator control code.
 *
 * This function will be started in its own task with the default priority and stack size whenever the robot is enabled via the Field Management System or the VEX Competition Switch in the operator control mode. If the robot is disabled or communications is lost, the operator control task will be stopped by the kernel. Re-enabling the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will run the operator control task. Be warned that this will also occur if the VEX Cortex is tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
void setArmSpeed(int speed)
{
	motorSet(armL, speed);
	motorSet(armR, -speed);
}

void setClaw(int speed)
{
	motorSet(grabberL, speed);
	motorSet(grabberR, -speed);
}

void Manipulator()
{
	//	bool but7L = joystickGetDigital(1, 7, JOY_LEFT);
	//	bool but7D = joystickGetDigital(1, 7, JOY_DOWN);
		bool but8R = joystickGetDigital(2, 8, JOY_RIGHT);
		bool but8D = joystickGetDigital(2, 8, JOY_DOWN);
		bool but5U = joystickGetDigital(2, 5, JOY_UP);
		bool but5D = joystickGetDigital(2, 5, JOY_DOWN);


	if(but8R)
	{
		setArmSpeed(80);
	}
	else if(but8D)
	{
		setArmSpeed(-80);
	}
	else setArmSpeed(0);

	if(but5U)
	{
		setClawSpeed(80);
	}
	else if(but5D)
	{
		setClawSpeed(-80);
	}
	else setClawSpeed(0);
}

void Driver()
{
	int joyLY = joystickGetAnalog(1, 3);
	int joyRX = joystickGetAnalog(1, 1);
	int lOut;
	int rOut;

	if(abs(joyLY) < 10) joyLY = 0;
	if(abs(joyRX) < 10) joyRX = 0;

	lOut = (joyLY - joyRX);
	rOut = (joyLY + joyRX);

	if(joyLY == 0)
	{
		lOut = -joyRX;
		rOut = joyRX;
	}
	if(joyRX == 0)
	{
		lOut = joyLY;
		rOut = joyLY;
	}

	if(lOut > 127 || lOut < -127)
	{
		lOut = (lOut/abs(lOut))*127;
	}
	if(rOut > 127 || rOut < -127)
	{
		rOut = (rOut/abs(rOut))*127;
	}


	motorSet(driveL, (lOut));
	motorSet(driveR, (-rOut));

}

void operatorControl()
{
	while (true)
	{
		Driver();
		Manipulator();
		delay(25);
	}
}
