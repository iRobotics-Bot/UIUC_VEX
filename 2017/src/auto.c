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
// #define FRONT_LEFT_ENCODER 0
// #define FRONT_RIGHT_ENCODER 1
// #define REAR_LEFT_ENCODER 2
// #define REAR_RIGHT_ENCODER 3
// #define H_ENCODER 4
// #define DRIVE_RATIO 1
// #define ENCODER_TICKS 627.2
// #define WHEEL_DIA 4

//int driveF1 = 2;
//int driveF2 = 3;
//int driveR1 = 4;
//int driveR2 = 5;
//int driveH = 6;
//int armPivot = 7;
//int armPivot2 = 8;
//int launchIn = 2; // Port controls launch for inner 2 pistons
//int launchOut = 3; // Port controls launch for outer 2 pistons

void setDriveMotors(int Left, int Right)
{
  motorSet(driveF1, Right);
  motorSet(driveF2, Right);
  motorSet(driveR1, Left);
  motorSet(driveR2, Left);
}

void resetDriveEncoders()
{
  imeReset(FRONT_LEFT_ENCODER);
  imeReset(FRONT_RIGHT_ENCODER);
}

void AutoDrive(float distX, float distY, int speed)
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
    //use IMU for digital compass/gyro
	float arcLength = 16.5*pow(2, 0.5)*M_PI*degrees/360; //25.014 = diameter of turning circle
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

void Launch(bool strength)
{
	long startTime = millis();
	if (!strength)
	{
		digitalWrite(launchIn, true);
		while(millis()  < (startTime + 1000)){}
		digitalWrite(launchIn, false);
	}
	else
	{
		digitalWrite(launchIn, true);
		digitalWrite(launchOut, true);
		while(millis()  < (startTime + 1000)){}
		digitalWrite(launchIn, false);
		digitalWrite(launchOut, false);
	}
}

void setArmAngle(float angle, int speed)
{
  float targetV = (angle/360)*1023;
  float deltaV = analogRead(1)-targetV;
  float analogDeadband = (ARM_DEADBAND/360)*1023;
  while(fabsf(deltaV) > analogDeadband)
  {
    motorSet(armPivot, speed * (deltaV/fabsf(deltaV)));
  }
  motorSet(armPivot, 0);
}

void setArmAngleSketchy(bool state)
{
	long startTime = millis();
	if(state)
	{
		while(millis() < startTime + 3000)
		{
			motorSet(armPivot, 127);
			motorSet(armPivot2, -127);
		}
	}
	else
	{
		while(millis() < startTime + 3000)
		{
			motorSet(armPivot, -127);
			motorSet(armPivot2, 127);
		}
	}
}

void clawSet(bool state)
{
	if(state)
	{
		while(!digitalRead(clawSensor))
		{
			motorSet(cubePincer, 100);
		}
	}
	else
	{
		long startTime = millis();
		while(millis() < startTime + 1000)
		{
			motorSet(cubePincer, -100);
		}
	}
}

void autonomous()
{
	AutoDrive(-36, 0, 127);
	clawSet(true);
	AutoDrive(0, 30, 127);
	clawSet(false);
	setArmAngleSketchy(true);
	clawSet(true);
	AutoDrive(0, 5, 127);
	Launch(true);
	AutoRotate(180, 127);
	setArmAngleSketchy(false);
	clawSet(false);
	AutoDrive(24, 0, 127);
	AutoDrive(0, 33, 127);
	setArmAngleSketchy(true);
	AutoDrive(0, -35, 127);
	AutoRotate(180, 127);
	Launch(false);
}