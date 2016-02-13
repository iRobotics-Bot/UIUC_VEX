/*
 * pid.h
 *
 *  Created on: Jan 28, 2014
 *      Author: iRobotics
 */

#ifndef PID_H_
#define PID_H_


/* TYPEDEFS */
typedef struct PID_t
{
	/* user inputs */
	double observed;
	double setpoint;
	double p_gain;
	double i_gain;
	double d_gain;
	int p_frequency;
	int i_frequency;
	int d_frequency;

	/* data for PID function to record */
	double integrated_error;
	double prev_error_for_d;
	int p_count;
	int i_count;
	int d_count;
	double prevSetpoint;
} PID_t;


/* FUNCTION PROTOTYPES */
double PID( PID_t * data );


#endif /* PID_H_ */
