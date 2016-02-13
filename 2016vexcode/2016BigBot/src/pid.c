/*
 * pid.c
 *
 *  Created on: Jan 27, 2014
 *      Author: iRobotics
 */

#include "pid.h"

double PID( PID_t * data )
{
	/* initialize the change to be applied to the control to 0 */
	double output = 0;
	/* store the error (we will need it multiple times) */
	double error = data->observed - data->setpoint;

	/* if the setpoint changed, we need to reset our accumulated error variables */
	if( data->prevSetpoint != data->setpoint )
	{
		data->integrated_error = 0;
		data->prev_error_for_d = 0;
	}

	/* if it's time to apply P, then do it */
	if( data->p_count > data->p_frequency )
	{
		output += data->p_gain*(error);
		data->p_count = 0;
	}

	/* continue integrating the error */
	data->integrated_error += error;
	/* if it's time to apply I, then do it */
	if( data->i_count > data->i_frequency )
	{
		output += data->i_gain*(data->integrated_error);
		data->i_count = 0;
	}


	/* if it's time to apply D, then do it */
	if( data->d_count > data->d_frequency )
	{
		output += data->d_gain*((error - data->prev_error_for_d) / data->d_count);
		data->prev_error_for_d = error;
		data->d_count = 0;
	}

	/* increment the counters */
	data->p_count++;
	data->i_count++;
	data->d_count++;

	/* set prevSetpoint to be the current setpoint */
	data->prevSetpoint = data->setpoint;

	return output;
}
