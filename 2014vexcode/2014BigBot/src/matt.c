/*
 * matt.c
 *
 *  Created on: Jan 28, 2014
 *      Author: Matt
 */


#include "auto_scripts/matt.h"
#include "main.h"
#include "helpers.h"


/* GLOBAL VARIABLES */
static int state = INIT;
static int i = 0;
static int left_ime, right_ime;
static int stall_timer = 0;

/*************************************************/
/*** UNCOMMENT THIS LINE TO ENABLE PID TURNING ***/
/*************************************************/
#define TURN_USING_PID

void GoMidterminators( int color ){
	printf("State: %d\r\n", state);
	//imeGet( LEFT_IME, &left_ime);
	//imeGet( RIGHT_IME, &right_ime );
	//printf( "LEFT_IME: %d || RIGHT_IME: %d || Gyro: %d\n\r", left_ime, right_ime, getAverageGyro() );
	//("FUCK VEX!\n\r");
		
	//printf("state = %d\n\r", state);

	switch( state )
	{
		case INIT:
			Reset();
			state = RAISE_ARM1;
			break;

		case RAISE_ARM1:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_UP, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 1500/LOOP_TIME )
			{
				Reset();
				state = FORWARD1;
			}
			break;

		case FORWARD1:
			Drive( DRIVE_FORWARD, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime > 400 )
			{
				state = DONE;
				Reset();
			}
			stall_timer++;
			if( stall_timer > 1000/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE1:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = REVERSE1;
				Reset();
			}
			break;

		case REVERSE1:
			Drive( DRIVE_REVERSE, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime < -60 )
			{
				state = PAUSE2;
				Reset();
			}
			stall_timer++;
			if( stall_timer > 500/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE2:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = TURN1;
				Reset();
			}
			break;

		case TURN1:
			if( color == BLUE )
			{
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( 90 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_LEFT );
				if( getAverageGyro() > 50 )
				#endif
				{
					state = PAUSE3;
					Reset();
				}
			}
			else
			{
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( -92 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_RIGHT );
				#endif
				if( getAverageGyro() < -50 )
				{
					state = PAUSE3;
					Reset();
				}
			}
			stall_timer++;
			if( stall_timer > 10000/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE3:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = LOWER_ARM1;
				Reset();
			}
			break;

		case LOWER_ARM1:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_DOWN, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			Reset();
			state = REVERSE0;
			break;

		case REVERSE0:
			Drive( DRIVE_REVERSE, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 1000/LOOP_TIME )
			{
				state = FORWARD2;
				Reset();
			}
			break;

		case FORWARD2:
			Drive( DRIVE_FORWARD, ROTATE_OFF );
			Manipulate( ARM_DOWN, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime > 550 )
			{
				state = PAUSE4;
				Reset();
			}
			stall_timer++;
			if( stall_timer > 1000/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE4:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
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
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( 90 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_LEFT );
				if( getAverageGyro() > 55 )
				#endif
				{
					state = PAUSE5;
					Reset();
				}
			}
			else
			{
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( -90 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_RIGHT );
				if( getAverageGyro() < -55 )
				#endif
				{
					state = PAUSE5;
					Reset();
				}
			}
			stall_timer++;
			if( stall_timer > 10000/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE5:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = RAISE_ARM2;
				Reset();
			}
			break;

		case RAISE_ARM2:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_UP, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			Reset();
			state = PAUSE6;
			break;

		case PAUSE6:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 1500/LOOP_TIME )
			{
				state = PLUCK1;
				Reset();
			}
			break;

		case PLUCK1:
			Drive( DRIVE_FORWARD_SLOW, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_SUCK, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 1500/LOOP_TIME )
			{
				state = REVERSE2;
				Reset();
			}
			break;

		case REVERSE2:
			Drive( DRIVE_REVERSE, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_SUCK, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime < -50 )
			{
				state = TURN3;
				Reset();
			}
			stall_timer++;
			if( stall_timer > 700/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case TURN3:
			if( color == BLUE )
			{

				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( -19 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_RIGHT );
				if( getAverageGyro() < -2 )
				#endif
				{
					state = PLUCK2;
					Reset();
				}
			}
			else
			{
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( 19 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_LEFT );
				if( getAverageGyro() > 2 )
				#endif
				{
					state = PLUCK2;
					Reset();
				}
			}
			stall_timer++;
			if( stall_timer > 10000/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PLUCK2:
			Drive( DRIVE_FORWARD_SLOW, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_SUCK, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 1500/LOOP_TIME )
			{
				state = REVERSE3;
				Reset();
			}
			break;

		case REVERSE3:
			Drive( DRIVE_REVERSE, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_SUCK, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime < -100 )
			{
				state = PAUSE7;
				Reset();
			}
			stall_timer++;
			if( stall_timer > 700/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE7:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_SUCK, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = TURN4;
				Reset();
			}
			break;

		case TURN4:
			if( color == BLUE )
			{
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( -77 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_RIGHT );
				if( getAverageGyro() < -42 )
				#endif
				{
					state = REVERSE4;
					Reset();
				}
			}
			else
			{
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( 77 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_LEFT );
				if( getAverageGyro() > 42 )
				#endif
				{
					state = REVERSE4;
					Reset();
				}
			}
			stall_timer++;
			if( stall_timer > 10000/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case REVERSE4:
			Drive( DRIVE_REVERSE, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 2200/LOOP_TIME )
			{
				state = FORWARD3;
				Reset();
			}
			break;

		case FORWARD3:
			Drive( DRIVE_FORWARD, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime > 120 )
			{
				state = PAUSE8;
				Reset();
			}
			stall_timer++;
			if( stall_timer > 500/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE8:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_SUCK, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = TURN5;
				Reset();
			}
			break;

		case TURN5:
			if( color == BLUE )
			{
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( -71 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_RIGHT );
				if( getAverageGyro() < -54 )
				#endif
				{
					state = PAUSE9;
					Reset();
				}
			}
			else
			{
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( 71 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_LEFT );
				if( getAverageGyro() > 54 )
				#endif
				{
					state = PAUSE9;
					Reset();
				}
			}
			stall_timer++;
			if( stall_timer > 10000/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE9:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = FORWARD4;
				Reset();
			}
			break;

		case FORWARD4:
			Drive( DRIVE_FORWARD, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime > 750 )
			{
				state = RAISE_ARM3;
				Reset();
			}
			stall_timer++;
			if( stall_timer > 2000/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case RAISE_ARM3:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_UP, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = FORWARD5;
				Reset();
			}
			break;

		case FORWARD5:
			Drive( DRIVE_FORWARD_SLOW, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 1700/LOOP_TIME )
			{
				state = SPIT;
				Reset();
			}
			break;

		case SPIT:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_SPIT, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 4000/LOOP_TIME )
			{
				state = REVERSE5;
				Reset();
			}
			break;

		case REVERSE5:
			Drive( DRIVE_REVERSE, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime < -100 )
			{
				state = LOWER_ARM2;
				Reset();
			}
			stall_timer++;
			if( stall_timer > 500/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case LOWER_ARM2:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_DOWN, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				Reset();
				state = REVERSE6;
			}
			break;

		case REVERSE6:
			Drive( DRIVE_REVERSE, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime < -450 )
			{
				state = PAUSE10;
				Reset();
			}
			stall_timer++;
			if( stall_timer > 1000/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE10:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = TURN6;
				Reset();
			}
			break;

		case TURN6:
			if( color == BLUE )
			{
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( 60 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_LEFT );
				if( getAverageGyro() > 150 )
				#endif
				{
					state = PAUSE11;
					Reset();
				}
			}
			else
			{
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( -60 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_RIGHT );
				if( getAverageGyro() < -150 )
				#endif
				{
					state = PAUSE11;
					Reset();
				}
			}
			stall_timer++;
			if( stall_timer > 10000/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE11:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = REVERSE7;
				Reset();
			}
			break;

		case REVERSE7:
			Drive( DRIVE_REVERSE, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 1000/LOOP_TIME )
			{
				state = FORWARD6;
				Reset();
			}
			break;

		case FORWARD6:
			Drive( DRIVE_FORWARD, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime > 100 )
			{
				state = PAUSE12;
				Reset();
			}
			stall_timer++;
			if( stall_timer > 500/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE12:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = TURN7;
				Reset();
			}
			break;

		case TURN7:
			if( color == BLUE )
			{
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( 90 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_LEFT );
				if( getAverageGyro() > 50 )
				#endif
				{
					state = PAUSE13;
					Reset();
				}
			}
			else
			{
				Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
				#ifdef TURN_USING_PID
				if( Turn( -90 ) )
				#else
				Drive( DRIVE_OFF, ROTATE_RIGHT );
				if( getAverageGyro() < -50 )
				#endif
				{
					state = PAUSE13;
					Reset();
				}
			}
			stall_timer++;
			if( stall_timer > 10000/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE13:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = GO_AND_SUCK;
				Reset();
			}
			break;

		case GO_AND_SUCK:
			Drive( DRIVE_FORWARD, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_SUCK, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			imeGet(LEFT_IME, &left_ime);
			if( left_ime > 1000 )
			{
				state = PAUSE14;
				Reset();
			}
			stall_timer++;
			if( stall_timer > 2500/LOOP_TIME )
			{
				state = DONE;
			}
			break;

		case PAUSE14:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_SUCK, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			i++;
			if( i > 700/LOOP_TIME )
			{
				state = DONE;
				Reset();
			}
			break;

		case DONE:
			Drive( DRIVE_OFF, ROTATE_OFF );
			Manipulate( ARM_HOLD, CONVEYOR_HOLD, OVERRIDE_OFF, OVERRIDE_ARM_HOLD, RECALIBRATE_OFF, LIFTUP_OFF, LIFTDOWN_OFF );
			break;
	}
}

void Reset()
{
	imeReset( LEFT_IME );
	imeReset( RIGHT_IME );
	resetAverageGyro();
	i = 0;
	stall_timer = 0;
}

