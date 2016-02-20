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
//#include "opcontrol.h"
#include "helpers.h"
#include "pid.h"
//#include "math.h"
#include "shooter.h"

//int  override = 0;
int count = 0;

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
void operatorControl() {
	while(true){
//	while (!override)
//	{
//		if (joystickGetDigital(DRIVER, BUTTONS_L, JOY_DOWN))
//			override = OVERRIDE_ON;
//	}
//	while (override)
//	{
				/* Get the driver joystick values */
//				printf("code gets here!\r\n");
	//	arcadeDrive();
//		Drive(100,100);
			int djoyLY = joystickDeadband( joystickGetAnalog( DRIVER, JOYLY ), 0.05 );
			//int djoyRY = joystickDeadband( joystickGetAnalog( DRIVER, JOYRY ), 0.05 );
			int djoyRX = joystickDeadband( joystickGetAnalog( DRIVER, JOYRX ), 0.05 );

			/* Drive the wheels according to driver input */
			//printf("djoyRX = %d\n\r", djoyRX);
			arcadeDrive( djoyLY, djoyRX );   // sticks are reversed, find a solution later 

				/* Get the manipulator joystick values */
				bool mintake = joystickGetDigital(DRIVER, BUTTONS_R, JOY_DOWN);
				bool mconveyor = joystickGetDigital(DRIVER, BUTTONS_R, JOY_RIGHT);
				bool mshooter = joystickGetDigital(DRIVER, BUTTONS_R, JOY_UP);

				/* Intake code */
				if (mintake) {
					motor( INTAKEMTR, INTAKE_BALLS);
				} else {
					motor( INTAKEMTR, LOW);
				}
				/* Conveyor Code */
				if (mconveyor) {
					motor( CONVEYORMTR1, CONVEYOR_SUCK );
					motor( CONVEYORMTR2, CONVEYOR_SUCK );
				} else {
					motor( CONVEYORMTR1, LOW );
					motor( CONVEYORMTR2, LOW );
				}

				/* shooter code */
//				if (mshooter)
//					shooter(&shooter_PID, SHOOTER_SPIT);
				if(mshooter){
					 motor(SHOOTERMTRL1, 70);
					 motor(SHOOTERMTRL2, 70);
					 motor(SHOOTERMTRR1, 70);
					motor(SHOOTERMTRR2, 70);
					count++;
//					int shooter_spd;
//					if(count == 4){
//						shooter(&shooter_PID, SHOOTER_SPIT);
//						count = 0;
//					}
//					printf("Shooter Speed: %d \r\n", shooter_spd);
				} else {
					 motor(SHOOTERMTRL1, 0);
					 motor(SHOOTERMTRL2, 0);
					 motor(SHOOTERMTRR1, 0);
					motor(SHOOTERMTRR2, 0);
				}
				

//				if (override && joystickGetDigital(DRIVER, BUTTONS_L, JOY_DOWN))
//					override = OVERRIDE_OFF;

				delay(25);
				//}
	}
}
