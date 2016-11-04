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

//#define driveF1 2
//#define driveF2 3
//#define driveR1 4
//#define driveR2 5
//#define driveH 6
//#define armPivot 7
//#define armPivot2 8
//#define cubePincer 9
//#define elevator1 1
//#define elevator2 10
//#define launchIn 2 // Port controls launch for inner 2 pistons
//#define launchOut 3 // Port controls launch for outer 2 pistons
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
void setArmSpeed()
{
	bool but7L = joystickGetDigital(1, 7, JOY_LEFT);
	bool but7D = joystickGetDigital(1, 7, JOY_DOWN);
	bool but8R = joystickGetDigital(1, 8, JOY_RIGHT);
	bool but8D = joystickGetDigital(1, 8, JOY_DOWN);
	bool but6U = joystickGetDigital(1, 6, JOY_UP);
	bool but6D = joystickGetDigital(1, 6, JOY_DOWN);

	if (but7L)
	{
		motorSet(armPivot, 127);
		motorSet(armPivot2, -127);
	}
	else if (but7D)
	{
		motorSet(armPivot, -127);
		motorSet(armPivot2, 127);
	}
	else
	{
		motorSet(armPivot, 0);
		motorSet(armPivot2, 0);
	}

		if (but8R && !digitalRead(clawSensor))
		{
			motorSet(cubePincer, 64);
		}
		else if (but8D)
		{
			motorSet(cubePincer, -64);
		}
		else
		{
			motorSet(cubePincer, 0);
		}

	if (but6U)
	{
		motorSet(elevator1, -127);
		motorSet(elevator2, 127);
	}
	else if (but6D)
	{
		motorSet(elevator1, -127);
		motorSet(elevator2, 127);
	}
	else
	{
		motorSet(elevator1, 0);
		motorSet(elevator2, 0);
	}
}

void setLaunch()
{
	bool but6U = joystickGetDigital(1, 6, JOY_UP);
	bool but6D = joystickGetDigital(1, 6, JOY_DOWN); // launches all pistons
	if (but6U)
	{
		digitalWrite(launchIn, true);
		delay(1000);
		digitalWrite(launchIn, false);
	}
	if (but6D)
	{
		digitalWrite(launchIn, true);
		digitalWrite(launchOut, true);
		delay(1000);
		digitalWrite(launchIn, false);
		digitalWrite(launchOut, false);
	}

}

void setDrive()
{
	int joyLX = joystickGetAnalog(1, 4);
	int joyLY = joystickGetAnalog(1, 3);
	int joyRX = joystickGetAnalog(1, 1);
	int fOut;
	int rOut;
	int hOut;

	if(abs(joyLX) < 20) joyLX = 0;
	if(abs(joyLY) < 20) joyLY = 0;
	if(abs(joyRX) < 20) joyRX = 0;

	fOut = (joyLX - joyRX);
	rOut = (joyLX + joyRX);
	hOut = joyLY;

	if(joyLX == 0)
	{
		fOut = -joyRX;
		rOut = joyRX;
	}
	if(joyRX == 0)
	{
		fOut = joyLX;
		rOut = joyLX;
	}

	if(fOut > 127 || fOut < -127)
	{
		fOut = (fOut/abs(fOut))*127;
	}
	if(rOut > 127 || rOut < -127)
	{
		rOut = (rOut/abs(rOut))*127;
	}

	motorSet(driveF1, fOut);
	motorSet(driveF2, fOut);
	motorSet(driveR1, rOut);
	motorSet(driveR2, rOut);
	motorSet(driveH, hOut);
}

void operatorControl()
{
	while (true)
	{
		setDrive();
		setLaunch();
		setArmSpeed();
//		if(!isJoystickConnected(1)) motorSet(driveF1, 10);
		delay(25);
	}
}
