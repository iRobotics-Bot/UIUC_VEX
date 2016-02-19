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

void status_auto(){
	camPursuit();
	if(digitalRead(BALL_DETECT))
	{
		camAim();
//		auto_shoot();
	}
}

void auto_shoot()
{
	int i = 0;

	for(i = 0;i < SHOOT_TIME;i++)
	{
		Manipulate(FORWARD,FORWARD);
	}
	return;
}
