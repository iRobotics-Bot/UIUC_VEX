/*
 * status_auto.c
 *
 *  Created on: Feb 18, 2016
 *      Author: Status
 */

#include "API.h"
#include "camera.h"
#include "pid.h"
#include "helpers.h"

#define SHOOT_TIME 100

int ball_flag = 0;

void ball_detect_handler(){
	ball_flag = 1;
}

void status_auto_init()
{
	ioSetInterrupt(BALL_DETECT, INTERRUPT_EDGE_RISING, ball_detect_handler);
	ioSetInterrupt(BALL_DETECT2, INTERRUPT_EDGE_RISING, ball_detect_handler);
	ball_flag = 0;
}

void status_auto(){
//	static timer = 0;
	if(ball_flag == 0)
	{
//		find_ball_general();
		camPursuit();
	}
	else if(ball_flag == 1)
	{
		find_tower_general();
		camAim();
		//hard coded shoot routine
		int i = 0;
		Drive(DRIVE_OFF,DRIVE_OFF);
//		for(i = 0;i < SHOOT_TIME;i++)
//		{
			Manipulate(FORWARD,FORWARD);
			Drive(HALF_SPEED,DRIVE_OFF);
			delay(1000);
			Manipulate(DRIVE_OFF,DRIVE_OFF);
//		}
		ball_flag = 0;
	}
//	timer++;
//	if(!(timer%1500 == 0))
//	{
//		find_ball_general();
//		camPursuit();
//	}
//	else
//	{
//		find_tower_general();
//		camAim();
//		//hard coded shoot routine
//		int i = 0;
//
//		for(i = 0;i < SHOOT_TIME;i++)
//		{
//			Manipulate(FORWARD,FORWARD);
//		}
//		ball_flag = 0;
//	}
}


