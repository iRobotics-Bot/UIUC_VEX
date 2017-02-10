/**
* File for autonomous code.
*
* This file should contain the user autonomous() function and any functions related to it.
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
#include <API.h>
//autonomous drive distance deadband = 1 inch
#define DRIVE_DEADBAND 1
//arm control deadband = 5 degrees
#define ARM_DEADBAND 5

/**
* Runs the user autonomous code.
*
* This function will be started in its own task with the default priority and stack size whenever the robot is enabled via the Field Management System or the VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart the task, not re-start it from where it left off.
*
* Code running in the autonomous task cannot access information from the VEX Joystick. However, the autonomous function can be invoked from another task if a VEX Competition Switch is not available, and it can access joystick information if called in this way.
*
* The autonomous task may exit, unlike operatorControl() which should never exit. If it does so, the robot will await a switch to another mode or disable/enable cycle.
*/

//NOTE: 393 Integrated encoder has 627.2 counts/revolution in torque config, 392 in speed config
// Chain assignments in main.h: (This is the order that the motors should be connected in the daisy chain)
//FL:1
//FR:2

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

void setDriveMotors(int Left, int Right)
{
  motorSet(driveLF, Left);
  motorSet(driveLR, Left);
  motorSet(driveRF, Right);
  motorSet(driveRR, Right);
}

void resetDriveEncoders()
{
  imeReset(FRONT_LEFT_ENCODER);
  imeReset(FRONT_RIGHT_ENCODER);
}

void AutoDrive(float distX, int speed)
{
	//distX & distY in inches
	//drive gear ratio assumed to be 1:1. Change factor in main declaration
	const float countDist = (DRIVE_RATIO*WHEEL_DIA*M_PI)/ENCODER_TICKS;
	int FL_Count, FR_Count;
	float FL_Dist = 0, FR_Dist = 0;
	resetDriveEncoders();
	//while any drive is not yet at the final distance
	while((((fabsf(FL_Dist) - fabsf(distX)) > DRIVE_DEADBAND) || ((fabsf(FR_Dist) - fabsf(distX)) > DRIVE_DEADBAND)))
	{
		//take cumulative distance for all encoders
		imeGet(FRONT_LEFT_ENCODER, &FL_Count);
		imeGet(FRONT_RIGHT_ENCODER, &FR_Count);
		FL_Dist += (FL_Count*countDist);
		FR_Dist += (FR_Count*countDist);
		//check to see if all drive directions need to be driven, or if only one axis needs to move still, to prevent overdriving one axis
		if((((fabsf(FL_Dist) - fabsf(distX)) > DRIVE_DEADBAND) || ((fabsf(FR_Dist) - fabsf(distX)) > DRIVE_DEADBAND)))
		{
			setDriveMotors((int)speed*distX/fabsf(distX));
		}
		else setDriveMotors(0, 0);
	}
	setDriveMotors(0, 0);
}

void AutoRotate(float degrees, int speed)
{
	float arcLength = 19.25*degrees/360; //25.014 = diameter of turning circle
	float rad_l = arcLength, rad_r = arcLength;
	float FL_Dist = 0, FR_Dist = 0;

	//drive gear ratio assumed to be 1:1. Change factor in main declaration
	const float COUNTDIST = (DRIVE_RATIO*WHEEL_DIA*M_PI)/ENCODER_TICKS;
	int count_l, count_r;
	if(sin(degrees) > 0) { rad_l *= -1; }
    else                 { rad_r *= -1; }
	resetDriveEncoders();
	//while any drive is not yet at the final distance
	while(fabsf(FL_Dist) - fabsf(arcLength) > DRIVE_DEADBAND || fabsf(FR_Dist) - fabsf(arcLength) > DRIVE_DEADBAND)
	{
	  //take cumulative distance for all encoders
	  imeGet(FRONT_LEFT_ENCODER, &count_l);
	  imeGet(FRONT_RIGHT_ENCODER, &count_r);
	  FL_Dist += (count_l*COUNTDIST);
  	  FR_Dist += (count_r*COUNTDIST);
 	  //check to see if all drive directions need to be driven, or if only one axis needs to move still, to prevent overdriving one axis
	  if((fabsf(FL_Dist) - fabsf(arcLength) > DRIVE_DEADBAND) && (fabsf(FR_Dist) - fabsf(arcLength) > DRIVE_DEADBAND))
	  {
		  setDriveMotors((int)speed*rad_l/fabsf(rad_l), (int)speed*rad_r/fabsf(rad_r));
	  }
	  else if((fabsf(FL_Dist) - fabsf(arcLength) > DRIVE_DEADBAND))
	  {
		  setDriveMotors((int)speed*rad_l/fabsf(rad_l), 0);
	  }
	  else if((fabsf(FR_Dist) - fabsf(arcLength) > DRIVE_DEADBAND))
	  {
		  setDriveMotors(0, (int)speed*rad_r/fabsf(rad_r));
	  }
	}
	setDriveMotors(0, 0);
}

void setArm(bool state)
{
	int armPotDifference = 0;
	if(state) armPotDifference = POT_GOAL_UP - analogRead(1);
	else armPotDifference = POT_GOAL_DOWN - analogRead(1);

	if(abs(armPotDifference) > ARM_DEADZONE)
	{
		setArmSpeed((ARM_SPEED_MIN + ARM_P*abs(armPotDifference))*(armPotDifference/abs(armPotDifference)));
	}
	else
	{
		setArmSpeed(0);
	}
}

void setClaw(int state)
{
	int clawPotDifference = 0;
	if(state == 0) clawPotDifference = (CLAW_CLOSE - analogRead(2));
    else if(state == 1) clawPotDifference = (CLAW_OPEN - analogRead(2));
    else if(state == 2) clawPotDifference = (CLAW_WALL - analogRead(2));
    else if(state == 3) clawPotDifference = (CLAW_PLOW - analogRead(2));

	if(abs(clawPotDifference) > CLAW_DEADZONE)
	{
		setClawSpeed((CLAW_SPEED_MIN + CLAW_P*abs(clawPotDifference))*(clawPotDifference/abs(clawPotDifference)));
	}
	else
	{
		setClawSpeed(0);
	}
}

void setWall(bool state)
{
		long startTime = millis();
		if(state)
		{
			while(millis() < startTime + 1000)
			{
				setClawExtSpeed(40);
			}
		}
		else
		{
			while(millis() < startTime + 3000)
			{
				setClawExtSpeed(-40);
			}
		}
}

void autonomous()
{
	setArm(true);
	setClaw(3);
	setArm(false);
	AutoDrive(72, 100);
	setClaw(0);
	AutoRotate(90, 100);
	setClaw(1);
	AutoRotate(55, 100);
	AutoDrive(36, 100);
	setClaw(0);
	AutoRotate(-55, 100);
	setArm(true);
	AutoDrive(36, 100);
	setClaw(3);
	setClaw(0);
	AutoDrive(-24, 100);
	setClaw(2);
	AutoDrive(24, 100);
}
