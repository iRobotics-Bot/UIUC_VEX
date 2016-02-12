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

/**
* Runs the user autonomous code.
*
* This function will be started in its own task with the default priority and stack size whenever the robot is enabled via the Field Management System or the VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart the task, not re-start it from where it left off.
*
* Code running in the autonomous task cannot access information from the VEX Joystick. However, the autonomous function can be invoked from another task if a VEX Competition Switch is not available, and it can access joystick information if called in this way.
*
* The autonomous task may exit, unlike operatorControl() which should never exit. If it does so, the robot will await a switch to another mode or disable/enable cycle.
*/
#define GYROPORT 3
void motors(int speedArray[4])
	{
		//Top Right
		motorSet(1,-speedArray[0]);
		motorSet(2,speedArray[0]); //reverse speed for motors on channel 1,3,4,5,6 and 8
		//Top Left
		motorSet(3,-speedArray[1]);
		motorSet(4,-speedArray[1]);
		//Bottom Left
		motorSet(5,-speedArray[2]);
		motorSet(6,-speedArray[2]);
		//Bottom Right
		motorSet(7,speedArray[3]); //
		motorSet(8,speedArray[3]); //
	}
void goStraight(int speed)
{
//	Gyro gyro = gyroInit( GYROPORT, 196 );
//	int gyroValue = gyroGet(gyro);
	int speedArray[4] = {speed, speed, speed, speed};
	motors(speedArray);
}

void autonomous() {
	while(true){
	int state=1;
	switch( state )
		{
			case 1:
				goStraight(127);
				break;
			/* case TURN1:
				if( color == BLUE )
				{
					Manipulate( ARM_HOLD, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
					#ifdef turnViaPID_USING_PID
					if( turnViaPID( 81 ) )
					#else
					Drive( DRIVE_OFF, ROTATE_LEFT );
					if( getAverageGyro() > 50 )
					#endif
					{
						state = PAUSE1;
						Reset();
					}
				}
				else
				{
					Manipulate( ARM_HOLD, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
					#ifdef turnViaPID_USING_PID
					if( turnViaPID( -83 ) )
					#else
					Drive( DRIVE_OFF, ROTATE_RIGHT );
					if( getAverageGyro() < -50 )
					#endif
					{
						state = PAUSE1;
						Reset();
					}
				}
				break;*/
		}
	}
}
