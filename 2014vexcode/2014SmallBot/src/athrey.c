/*
 * athrey.c
 *
 *  Created on: Jan 28, 2014
 *      Author: iRobotics
 */

#if 0

#include "athrey.h"

void brown_dynamite(void)
{
	switch( state )
	{
	case INIT:
		//reset encoders
		break;

	case FORWARD_1:
		TankDrive ( 127, 127 ) ;
		Manipulate ( -100, 0, 0, 0 ) ;
		//if encoder reaches certain value, stop
		{
			TankDrive ( 0, 0 ) ;
			Manipulate ( 0, 0, 0, 0 ) ;
			//reset encoders
			state = TURN_LEFT_1;
		}
		break;

	case TURN_LEFT_1:
		TankDrive ( 0, 127 ) ;
		Manipulate ( 0, 0, 0, 0 ) ;
		//if encoder reaches certain value, stop
		{
			TankDrive ( 0, 0 ) ;
			Manipulate ( 0, 0, 0, 0 ) ;
			//reset encoders
			state = REVERSE_1;
		}
		break;

	case REVERSE_1:
		TankDrive ( -127, -127 ) ;
		Manipulate ( 0, 0, 0, 0 ) ;
		//if encoder reaches certain value, stop
		{
			TankDrive ( 0, 0 ) ;
			Manipulate ( 0, 0, 0, 0 ) ;
			//reset encoders
			state = DUMP_1;
		}
		break;

	case DUMP_1:
		TankDrive ( 0, 0 ) ;
		Manipulate ( 100, 0, 0, 0 ) ;
		//if arm has reached top of cycle, bring arm down to carry limit
		if ( arm > 100)
		{
			Manipulate ( 0, 0, 0, 0 ) ;
			//reset encoders
			state = FORWARD_2;
		}
		break;

	case FORWARD_2:
		TankDrive ( 127, 127 ) ;
		Manipulate ( 0, 0, 0, 0 ) ;
		//if encoder reaches certain value, stop
		{
			TankDrive ( 0, 0 ) ;
			Manipulate ( 0, 0, 0, 0 ) ;
			//reset encoders
			state = TURN_RIGHT_1;
		}
		break;

	case TURN_RIGHT_1:
		TankDrive ( 127, 0 ) ;
		Manipulate ( 0, 0, 0, 0 ) ;
		//if encoder reaches certain value, stop
		{
			TankDrive ( 0, 0 ) ;
			Manipulate ( 0, 0, 0, 0 ) ;
			//reset encoders
			state = FORWARD_3;

	case FORWARD_3:
		TankDrive ( 127, 127 ) ;
		Manipulate ( -100, 0, 0, 0 ) ;
		//if encoder reaches certain value, stop
		{
			TankDrive ( 0, 0 ) ;
			Manipulate ( 0, 0, 0, 0 ) ;
			//reset encoders
			state = TURN_LEFT_2;
		}
		break;

	case TURN_LEFT_1:
		TankDrive ( 0, 127 ) ;
		Manipulate ( 0, 0, 0, 0 ) ;
		//if encoder reaches certain value, stop
		{
			TankDrive ( 0, 0 ) ;
			Manipulate ( 0, 0, 0, 0 ) ;
			//reset encoders
			state = REVERSE_2;
		}
		break;

	case REVERSE_2:
		TankDrive ( -127, -127 ) ;
		Manipulate ( 0, 0, 0, 0 ) ;
		//if encoder reaches certain value, stop
		{
			TankDrive ( 0, 0 );
			Manipulate ( 0, 0, 0, 0 );
			//reset encoders
			state = DUMP_2;
		}
		break;

	case DUMP_2:
		TankDrive ( 0, 0 ) ;
		Manipulate ( 100, 0, 0, 0 );
		//if arm has reached top of cycle, bring arm down to carry limit
		if ( arm > 100)
		{
			Manipulate ( 0, 0, 0, 0 );
			//reset encoders
			state = STOP;
		}
		break;

	case STOP:
		TankDrive ( 0, 0 ) ;
		Manipulate ( 0, 0, 0, 0 ) ;
		//reset encoders
		break;

#endif
