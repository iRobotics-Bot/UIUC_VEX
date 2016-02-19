/*
 * shooter.h
 *
 *  Created on: Feb. 16, 2016
 *      Author: iRobotics
 */

#ifndef SHOOTER_H_
#define SHOOTER_H_

#include "pid.h"
#include "main.h"
#include "helpers.h"


void shooter (PID_t * data, double speed);

#endif /* SHOOTER_H_ */
