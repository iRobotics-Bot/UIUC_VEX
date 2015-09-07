/*
 * matt.c
 *
 *  Created on: Apr 22, 2014
 *      Author: Matt
 */


#include "auto_scripts/matt.h"
#include "main.h"
#include "helpers.h"

/*************************************************/
/*** UNCOMMENT THIS LINE TO ENABLE PID turnViaPIDING ***/
/*************************************************/
//#define turnViaPID_USING_PID

void GoMidterminators( int color ){
	imeGet( LEFT_IME, &left_ime);
	imeGet( RIGHT_IME, &right_ime );
	printf( "LEFT_IME: %d || RIGHT_IME: %d || Gyro: %d\n\r", left_ime, right_ime, getAverageGyro() );

	//printf("state = %d\n\r", state);

	/* For 90 degree left turns, use 81. For right turns, use -83 */


	switch( state )
	{
	/*	case FLING_BALL:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				Reset();
				state = TURN1;
			}
			break;

		case TURN1:
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
			break;

		case PAUSE1:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = FORWARD1 ;
				Reset();
			}
			break;*/  

		/*case REVERSE1:
			Drive( DRIVE_REVERSE, ROTATE_OFF );
			Manipulate( ARM_HOLD, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			i++;
			if( i > 500/LOOP_TIME )
			{
				state = FORWARD1;
				Reset();
			}
			break;*/

		case REVERSE1:   // arm down
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_DOWN, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			i++;
			stall_timer++;
			if( stall_timer > 4000 / LOOP_TIME)
					state = DONE;
			if( i > 700/LOOP_TIME )
			{
				state = FORWARD1;
				Reset();
			}
			break;

		case FORWARD1:
			Drive( DRIVE_FORWARD, ROTATE_OFF );
			Manipulate( ARM_DOWN, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			imeGet(LEFT_IME, &left_ime);
			stall_timer++;
			if( stall_timer > 3000 / LOOP_TIME)
					state = DONE;
			if( left_ime > 300 )
			{
				state = FORWARD2;
				Reset();
			}
			break;

		case FORWARD2:
			Drive( DRIVE_FORWARD, ROTATE_OFF );
			Manipulate( ARM_HOLD, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			imeGet(LEFT_IME, &left_ime);
			stall_timer++;
			if( stall_timer > 4000 / LOOP_TIME)
					state = DONE;
			if( left_ime > 700 )
			{
				state = PAUSE2;
				Reset();
			}
			break;

		case PAUSE2:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_UP, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			i++;
			if( i > 1000/LOOP_TIME )
			{
				state = REVERSE2;
				Reset();
			}
			break;

		case REVERSE2:
			Drive( DRIVE_REVERSE, ROTATE_OFF );
			Manipulate( ARM_UP, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			imeGet(LEFT_IME, &left_ime);
			stall_timer++;
			if( stall_timer > 3500 / LOOP_TIME)
					state = DONE;			
			if( left_ime < -300 )
			{
				state = PAUSE3;
				Reset();
			}
			break;

		case PAUSE3:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_UP, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = TURN2;
				Reset();
			}
			break;

		case TURN2:
			if( color == BLUE )
			{
				Manipulate( ARM_UP, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
				#ifdef turnViaPID_USING_PID
				if( turnViaPID( -90 ) )
				#else
				Drive( DRIVE_REVERSE, 127 );
				if( getAverageGyro() < -35 )
				#endif
				{
					state = PAUSE4;
					Reset();
				}
			}
			else
			{
				Manipulate( ARM_UP, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
				#ifdef turnViaPID_USING_PID
				if( turnViaPID( 90 ) )
				#else
				Drive( DRIVE_REVERSE, -127 );
				if( getAverageGyro() > 35 )
				#endif
				{
					state = PAUSE4;
					Reset();
				}
			}
			stall_timer++;
			if( stall_timer > 7000 / LOOP_TIME)
					state = DONE;
			break;

		case PAUSE4:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_UP, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = REVERSE3;
				Reset();
			}
			break;

		case REVERSE3:
			Drive( DRIVE_REVERSE_SLOW, ROTATE_OFF );
			Manipulate( ARM_UP, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			imeGet(LEFT_IME, &left_ime);
			stall_timer++;
			if( stall_timer > 3000 / LOOP_TIME)
					state = DONE;
			if( left_ime < -100 )
			{
				state = PAUSE5;
				Reset();
			}
			break;

		case PAUSE5:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			i++;
			//if( i > 15000/LOOP_TIME )
			if( i > 1000/LOOP_TIME )
			{
				state = FORWARD3;
				Reset();
			}
			break;

		case FORWARD3:
			Drive( DRIVE_FORWARD, ROTATE_OFF );
			Manipulate( ARM_HOLD, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			imeGet(LEFT_IME, &left_ime);
			stall_timer++;
			if( stall_timer > 3000 / LOOP_TIME)
					state = DONE;
			if( left_ime > 100 )
			{
				state = THROW;
				Reset();
			}
			break;

		case THROW:
			Drive( DRIVE_FORWARD, ROTATE_OFF );
			Manipulate( ARM_HOLD, CATAPULT_LAUNCH, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			imeGet(LEFT_IME, &left_ime);
			stall_timer++;
			if( stall_timer > 3000 / LOOP_TIME)
					state = DONE;
			if( left_ime > 300 )
			{
				state = DONE;
				Reset();
			}
			break;

		case FORWARD4:
			Drive( DRIVE_FORWARD, ROTATE_OFF );
			Manipulate( ARM_HOLD, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime > 1000 )
			{
				state = PAUSE6;
				Reset();
			}
			break;

		case PAUSE6:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = TURN3;
				Reset();
			}
			break;

		case TURN3:
			if( color == BLUE )
			{
				Manipulate( ARM_UP, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
				#ifdef turnViaPID_USING_PID
				if( turnViaPID( 90 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_LEFT );
				if( getAverageGyro() > 50 )
				#endif
				{
					state = PAUSE7;
					Reset();
				}
			}
			else
			{
				Manipulate( ARM_UP, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
				#ifdef turnViaPID_USING_PID
				if( turnViaPID( -90 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_RIGHT );
				if( getAverageGyro() < -50 )
				#endif
				{
					state = PAUSE7;
					Reset();
				}
			}
			break;

		case PAUSE7:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_UP, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = FORWARD5;
				Reset();
			}
			break;

		case FORWARD5:
			Drive( DRIVE_FORWARD, ROTATE_OFF );
			Manipulate( ARM_UP, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime > 300 )
			{
				state = DONE;
				Reset();
			}
			break;

		case DONE:
			printf("DONE\n\r");
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CATAPULT_HOLD, ANTENNA_HOLD, OVERRIDE_ON, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, HOLD_OFF );
			break;

	}
}

void Reset()
{
	imeReset( LEFT_IME );
	imeReset( RIGHT_IME );
	resetAverageGyro();
	stall_timer = 0;
	i = 0;
}


