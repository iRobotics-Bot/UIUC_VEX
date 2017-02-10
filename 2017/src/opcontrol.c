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
//#define DRIVE_DEADBAND 1
//arm control deadband = 5 degrees
//#define ARM_DEADBAND 5

//motors:
//#define driveLF 2
//#define driveLR 3
//#define driveRF 4
//#define driveRR 5
//#define armL 6
//#define armR 7
//#define grabber 8
//#define grabberExt 9

//sensors
//pot: 1


/**
 * Controller Mapping
 *		8R/8D: Arm Control
 *		5U/5D: Claw Control
 *		Drive: Halo
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

	bool raisePotToggled = false;
	bool raisePrevState = false;

	bool lowerPotToggled = false;
	bool lowerPrevState = false;

	bool inPotToggled = false;
	bool inPrevState = false;

	bool outPotToggled = false;
	bool outPrevState = false;


void setArmSpeed(int speed)
{
	motorSet(armL, speed);
	motorSet(armR, -speed);
}

void setClawSpeed(int speed)
{
	motorSet(grabber, speed);
}

void setClawExtSpeed(int speed)
{
	motorSet(grabberExt, speed);
}

void armP(bool but7L, bool but7D, bool but8R, bool but8D)
{
	int armPotDifference = 0;

	if(but7L && but7L != raisePrevState)
		{
			lowerPotToggled = false;
			raisePotToggled = !raisePotToggled;
			raisePrevState = but7L;
		}
		else if(but7L != raisePrevState)
		{
			raisePrevState = but7L;
		}

		if(but7D && but7D != lowerPrevState)
		{
			raisePotToggled = false;
			lowerPotToggled = !lowerPotToggled;
			lowerPrevState = but7D;
		}
		else if(but7D != lowerPrevState)
		{
			lowerPrevState = but7D;
		}

	    if(but8R)
	   	{
	   		setArmSpeed(80);
	   		raisePotToggled = false;
	   		lowerPotToggled = false;
		}
	    else if(but8D)
		{
			setArmSpeed(-80);
			raisePotToggled = false;
			lowerPotToggled = false;
		}
		else setArmSpeed(0);

	    if(raisePotToggled) armPotDifference = POT_GOAL_UP - analogRead(1);
	    else if(lowerPotToggled) armPotDifference = POT_GOAL_DOWN - analogRead(1);

		if((raisePotToggled || lowerPotToggled) && abs(armPotDifference) > ARM_DEADZONE)
		{
	//		setArmSpeed(POT_SPEED * (potDifference/abs(potDifference)));
			setArmSpeed((ARM_SPEED_MIN + ARM_P*abs(armPotDifference))*(armPotDifference/abs(armPotDifference)));
		}
		else if(raisePotToggled || lowerPotToggled)
		{
			setArmSpeed(0);
		}
}

void clawP(bool but5U, bool but5D, bool but8U, bool but8L)
{
	int clawPotDifference = 0;

	if(but5U && but5U != inPrevState)
	{
		outPotToggled = false;
		inPotToggled = !inPotToggled;
		inPrevState = but5U;
	}
	else if(but5U != inPrevState)
	{
		inPrevState = but5U;
	}

	if(but5D && but5D != outPrevState)
	{
		inPotToggled = false;
		outPotToggled = !outPotToggled;
		outPrevState = but5D;
	}
	else if(but5D != outPrevState)
	{
		outPrevState = but5D;
	}

	if(but8U)
	{
		setClawSpeed(-50);
		inPotToggled = false;
		outPotToggled = false;
	}
	else if(but8L)
	{
		setClawSpeed(50);
		inPotToggled = false;
		outPotToggled = false;
	}
	else setClawSpeed(0);

	if(inPotToggled) clawPotDifference = (CLAW_CLOSE - analogRead(2));
    else if(outPotToggled) clawPotDifference = (CLAW_OPEN - analogRead(2));

	if((inPotToggled || outPotToggled) && abs(clawPotDifference) > CLAW_DEADZONE)
	{
		setClawSpeed((CLAW_SPEED_MIN + CLAW_P*abs(clawPotDifference))*(clawPotDifference/abs(clawPotDifference)));
	}
	else if(inPotToggled || outPotToggled)
	{
		setClawSpeed(0);
	}

}

void Manipulator()
{
		bool but8R = joystickGetDigital(1, 8, JOY_RIGHT);
		bool but8D = joystickGetDigital(1, 8, JOY_DOWN);
		bool but8U = joystickGetDigital(1, 8, JOY_UP);
		bool but8L = joystickGetDigital(1, 8, JOY_LEFT);
		bool but7L = joystickGetDigital(1, 7, JOY_LEFT);
		bool but7D = joystickGetDigital(1, 7, JOY_DOWN);
		bool but5U = joystickGetDigital(1, 5, JOY_UP);
		bool but5D = joystickGetDigital(1, 5, JOY_DOWN);
		bool but6U = joystickGetDigital(1, 6, JOY_UP);
		bool but6D = joystickGetDigital(1, 6, JOY_DOWN);
		bool but7U = joystickGetDigital(1, 7, JOY_UP);
		bool but7R = joystickGetDigital(1, 7, JOY_RIGHT);

	if(but7U)
	{
		POT_GOAL_DOWN = analogRead(1);
		POT_GOAL_UP = POT_GOAL_DOWN + 1200;
	}
	if(but7R)
	{
		CLAW_CLOSE = analogRead(2);
		CLAW_OPEN = CLAW_CLOSE + 2450;
	}

	armP(but7L, but7D, but8R, but8D);
	clawP(but5U, but5D, but8U, but8L);

	if(but6U)
	{
		setClawExtSpeed(40);
	}
	else if(but6D)
	{
		setClawExtSpeed(-40);
	}
	else setClawExtSpeed(0);

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

	motorSet(driveLF, (lOut));
	motorSet(driveLR, (lOut));
	motorSet(driveRF, (rOut));
	motorSet(driveRR, (rOut));
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
