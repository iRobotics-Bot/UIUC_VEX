/*
 * pid.h
 *
 *  Created on: Jan 27, 2014
 *      Author: iRobotics
 */

#ifndef PID_H_
#define PID_H_


/* TYPEDEFS */
typedef struct PID_t
{
	/* user inputs */
	double observed; 	//observed motor speed
	double setpoint; 	//desired motor speed
	double p_gain; 		//factor multiplied by error
	double i_gain;
	double d_gain; 		//factor multiplied by derivative
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
double PID( PID_t * data ); 	//add on to desired motor to adjust speed


#endif /* PID_H_ */
