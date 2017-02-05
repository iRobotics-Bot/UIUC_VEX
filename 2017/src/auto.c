///**
//* File for autonomous code.
//*
//* This file should contain the user autonomous() function and any functions related to it.
//*
//* Copyright (c) 2011-2014, Purdue University ACM SIG BOTS. All rights reserved.
//*
//* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//*
//* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
//* Neither the name of Purdue University ACM SIG BOTS nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*
//* Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be obtained from http://sourceforge.net/projects/freertos/files/ or on request.
//********************************************************************************/
//
//#include "main.h"
//#include "math.h"
//#include <API.h>
////autonomous drive distance deadband = 1 inch
//#define DRIVE_DEADBAND 1
////arm control deadband = 5 degrees
//#define ARM_DEADBAND 5
//
///**
//* Runs the user autonomous code.
//*
//* This function will be started in its own task with the default priority and stack size whenever the robot is enabled via the Field Management System or the VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart the task, not re-start it from where it left off.
//*
//* Code running in the autonomous task cannot access information from the VEX Joystick. However, the autonomous function can be invoked from another task if a VEX Competition Switch is not available, and it can access joystick information if called in this way.
//*
//* The autonomous task may exit, unlike operatorControl() which should never exit. If it does so, the robot will await a switch to another mode or disable/enable cycle.
//*/
//
////NOTE: 393 Integrated encoder has 627.2 counts/revolution in torque config, 392 in speed config
//// Chain assignments in main.h: (This is the order that the motors should be connected in the daisy chain)
//
//
//void setDriveMotors(int Left, int Right)
//{
//  motorSet(driveL, Left);
//  motorSet(driveR, -Right);
//}
//
//void resetDriveEncoders()
//{
//  imeReset(FRONT_LEFT_ENCODER);
//  imeReset(FRONT_RIGHT_ENCODER);
//}
//
//void AutoDrive(float distX, int speed)
//{
//  //distX & distY in inches
//  //drive gear ratio assumed to be 1:1. Change factor in main declaration
//const float countDist = (DRIVE_RATIO*WHEEL_DIA*M_PI)/ENCODER_TICKS;
//int FL_Count, FR_Count;
//float FL_Dist = 0, FR_Dist = 0;
//resetDriveEncoders();
////while any drive is not yet at the final distance
//while((((fabsf(FL_Dist) - fabsf(distX)) > DRIVE_DEADBAND)) || (((fabsf(FR_Dist) - fabsf(distX)) > DRIVE_DEADBAND)))
//{
//  //take cumulative distance for all encoders
//imeGet(FRONT_LEFT_ENCODER, &FL_Count);
//imeGet(FRONT_RIGHT_ENCODER, &FR_Count);
//FL_Dist += (FL_Count*countDist);
//FR_Dist += (FR_Count*countDist);
////check to see if all drive directions need to be driven, or if only one axis needs to move still, to prevent overdriving one axis
//if((((fabsf(FL_Dist) - fabsf(distX)) > DRIVE_DEADBAND) || ((fabsf(FR_Dist) - fabsf(distX)) > DRIVE_DEADBAND)))
//{
//  setDriveMotors((int)speed*distX/fabsf(distX), (int)speed*distX/fabsf(distX));
//}
//else if((((fabsf(FL_Dist) - fabsf(distX)) > DRIVE_DEADBAND) || ((fabsf(FR_Dist) - fabsf(distX)) > DRIVE_DEADBAND)))
//{
//  setDriveMotors((int)speed*distX/fabsf(distX), (int)speed*distX/fabsf(distX));
//}
//}
//setDriveMotors(0, 0, 0);
//}
//
////void AutoRotate(float degrees, int speed)
////{
////    //use IMU for digital compass/gyro
////	float arcLength = 16.5*pow(2, 0.5)*M_PI*degrees/360; //25.014 = diameter of turning circle
////	float rad_l = arcLength, rad_r = arcLength;
////	float FL_Dist = 0, FR_Dist = 0;
////
////	//drive gear ratio assumed to be 1:1. Change factor in main declaration
////	const float COUNTDIST = (DRIVE_RATIO*WHEEL_DIA*M_PI)/ENCODER_TICKS;
////	int count_l, count_r;
////	if(sin(degrees) > 0) { rad_l *= -1; }
////    else                      { rad_r *= -1; }
////	resetDriveEncoders();
////	//while any drive is not yet at the final distance
////	while(fabsf(FL_Dist) - fabsf(arcLength) > DRIVE_DEADBAND || fabsf(FR_Dist) - fabsf(arcLength) > DRIVE_DEADBAND)
////	{
////	  //take cumulative distance for all encoders
////	  imeGet(FRONT_LEFT_ENCODER, &count_l);
////	  imeGet(FRONT_RIGHT_ENCODER, &count_r);
////	  FL_Dist += (count_l*COUNTDIST);
////  	  FR_Dist += (count_r*COUNTDIST);
//// 	  //check to see if all drive directions need to be driven, or if only one axis needs to move still, to prevent overdriving one axis
////	  if((fabsf(FL_Dist) - fabsf(arcLength) > DRIVE_DEADBAND) && (fabsf(FR_Dist) - fabsf(arcLength) > DRIVE_DEADBAND))
////	  {
////	    setDriveMotors((int)speed*rad_l/fabsf(rad_l), (int)speed*rad_r/fabsf(rad_r), 0);
////	  }
////	  else if((fabsf(FL_Dist) - fabsf(arcLength) > DRIVE_DEADBAND))
////	  {
////		  setDriveMotors((int)speed*rad_l/fabsf(rad_l), 0, 0);
////	  }
////	  else if((fabsf(FR_Dist) - fabsf(arcLength) > DRIVE_DEADBAND))
////	  {
////		  setDriveMotors(0, (int)speed*rad_r/fabsf(rad_r), 0);
////	  }
////	}
////	setDriveMotors(0, 0, 0);
////}
//
//void autonomous() {
//
//}
void moveMotors(int tickGoalLeft, int tickGoalRight, int speed)
{
	encoderReset(FRONT_LEFT_ENCODER);
	encoderReset(FRONT_RIGHT_ENCODER);
	
	bool isBusyLeft = true;
	bool isBusyRight = true;
	
	int leftdir = tickGoalLeft / abs(tickGoalLeft);
	int rightdir = tickGoalRight / abs(tickGoalRight);
	
	while(isBusyLeft || isBusyRight)
	{
		if(isBusyLeft)
		{
			motorSet(driveLF, speed*leftdir);
			isBusyLeft = abs(tickGoalLeft) - abs(encoderGet(FRONT_LEFT_ENCODER)) > 0;
		}
		else
		{
			motorSet(driveLF, 0);
			motorSet(driveLB, 0);
			isBusyLeft = false;
		}
		
		if(isBusyRight)
		{
			motorSet(driveRF, speed*rightdir);
			isBusyRight = abs(tickGoalRight) - abs(encoderGet(FRONT_RIGHT_ENCODER)) > 0;
		}
		else
		{  
			motorSet(driveRF, 0);
			motorSet(driveRB, 0);
			isBusyRIght = false;
		}
	}
}

void autonomous()
{
	moveMotors(1000, 1000, 50);
}