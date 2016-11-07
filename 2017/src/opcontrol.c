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

bool runAutoDrive = false;
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

	if (but7D/* && digitalRead(armStop)*/)
	{
		motorSet(armPivot, 127);
		motorSet(armPivot2, -127);
	}
	else if (but7L)
	{
		motorSet(armPivot, -127);
		motorSet(armPivot2, 127);
	}
	else
	{
		motorSet(armPivot, 0);
		motorSet(armPivot2, 0);
	}

		if (but8R && digitalRead(clawSensor))
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
//	float fOut;
//	float rOut;
//	float hOut;

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
//		fOut = (fOut/fabsf(fOut))*127;
	}
	if(rOut > 127 || rOut < -127)
	{
		rOut = (rOut/abs(rOut))*127;
//		rOut = (rOut/fabsf(rOut))*127;
	}

//	rOut = rOut * (100/127);

//	motorSet(driveF1, floor(rOut));
//	motorSet(driveF2, floor(rOut));
//	motorSet(driveR1, floor(-fOut));
//	motorSet(driveR2, floor(-fOut));
//	motorSet(driveH, floor(hOut));
	motorSet(driveF1, (rOut));
	motorSet(driveF2, (rOut));
	motorSet(driveR1, (-fOut));
	motorSet(driveR2, (-fOut));
	motorSet(driveH, (hOut));
}

void setDriveMotors2(int Left, int Right, int H)
{
  motorSet(driveF1, Right);
  motorSet(driveF2, Right);
  motorSet(driveR1, Left);
  motorSet(driveR2, Left);
  motorSet(driveH, H);
}

void resetDriveEncoders2()
{
  imeReset(FRONT_LEFT_ENCODER);
  imeReset(FRONT_RIGHT_ENCODER);
  imeReset(H_ENCODER);
}

void AutoDrive2(float distX, float distY, int speed)
{
  //distX & distY in inches
  //drive gear ratio assumed to be 1:1. Change factor in main declaration
	bool but5U = joystickGetDigital(1, 5, JOY_UP);
if(but5U) runAutoDrive = true;
if(runAutoDrive)
{
const float countDist = (DRIVE_RATIO*WHEEL_DIA*M_PI)/ENCODER_TICKS;
int FL_Count, FR_Count, H_Count;
float FL_Dist = 0, FR_Dist = 0, H_Dist = 0;
resetDriveEncoders2();
//while any drive is not yet at the final distance
while((((fabsf(FL_Dist) - fabsf(distX)) > DRIVE_DEADBAND) || ((fabsf(FR_Dist) - fabsf(distX)) > DRIVE_DEADBAND)) || ((fabsf(H_Dist) - fabsf(distY)) > DRIVE_DEADBAND))
{
  //take cumulative distance for all encoders
imeGet(FRONT_LEFT_ENCODER, &FL_Count);
imeGet(FRONT_RIGHT_ENCODER, &FR_Count);
imeGet(H_ENCODER, &H_Count);
FL_Dist += (FL_Count*countDist);
FR_Dist += (FR_Count*countDist);
H_Dist += (H_Count*countDist);
//check to see if all drive directions need to be driven, or if only one axis needs to move still, to prevent overdriving one axis
if((((fabsf(FL_Dist) - fabsf(distX)) > DRIVE_DEADBAND) || ((fabsf(FR_Dist) - fabsf(distX)) > DRIVE_DEADBAND)) && ((fabsf(H_Dist) - fabsf(distY)) > DRIVE_DEADBAND))
{
  setDriveMotors2((int)speed*distX/fabsf(distX), (int)speed*distX/fabsf(distX), (int)speed*distY/fabsf(distY));
}
else if((((fabsf(FL_Dist) - fabsf(distX)) > DRIVE_DEADBAND) || ((fabsf(FR_Dist) - fabsf(distX)) > DRIVE_DEADBAND)))
{
  setDriveMotors2((int)speed*distX/fabsf(distX), (int)speed*distX/fabsf(distX), 0);
}
else setDriveMotors2(0, 0, (int)speed*distY/fabsf(distY));
}
setDriveMotors2(0, 0, 0);
}
runAutoDrive = false;
}



void operatorControl()
{
	while (true)
	{
		setDrive();
		setLaunch();
		setArmSpeed();
//		AutoDrive2(10, 0, 100);
		delay(25);
	}
}
