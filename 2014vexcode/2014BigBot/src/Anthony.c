#include "auto_scripts/Anthony.h"
#include "main.h"
#include "helpers.h"

/* GLOBAL VARIABLES */
static int state = ARMUP;
static int left_ime, right_ime;
static int ball_counter = 0, pickup_timer;

/*	Drive forward, turn right, drive forward, pick up ball, reverse, turn left, drive forward,
	turn right, drive forward, pick up ball, reverse, turn left, drive forward, turn right, drive forward,
	pick up ball
	@param none
	Returns nothing
*/


void hashtagYoloSwag()
{
	switch( state )
	{
		case ARMUP:
			arcadeDrive( 0, 0);
			Manipulate( 127, 0, 0, 0, 0 ,0, 0);
			imeReset( LEFT_IME );
			imeReset( RIGHT_IME );
			state = DRIVE_FORWARD1;
			break;

		case DRIVE_FORWARD1:
			arcadeDrive( 127, 0);
			Manipulate( 0, 0, 0, 0, 0 , 0, 0);
			imeGet(LEFT_IME, &left_ime);
			imeGet(RIGHT_IME, &right_ime);
			if( right_ime > 200 && left_ime < -200 )
			{
				state = TURN_RIGHT;
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
			}
			break;

		case TURN_RIGHT:
			arcadeDrive( 0, 127);
			Manipulate( 0, 0, 0, 0, 0 , 0, 0);
			imeGet(LEFT_IME, &left_ime);
			imeGet(RIGHT_IME, &right_ime);
			if( right_ime > 200 && left_ime > 200 )
			{
				state = DRIVE_FORWARD2;
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
			}
			break;

		case DRIVE_FORWARD2:
			arcadeDrive( 127, 0);
			Manipulate( 0, 0, 0, 0, 0 , 0, 0);
			imeGet(LEFT_IME, &left_ime);
			imeGet(RIGHT_IME, &right_ime);
			if( right_ime > 100 && left_ime < -100 )
			{
				state = PICKUP;
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
			}
			break;

		case PICKUP:
			arcadeDrive( 0, 0);
			Manipulate( 0, 127, 0, 0, 0 , 0, 0);
			pickup_timer++;
			if( pickup_timer > 1000 )
			{
				pickup_timer = 0;
				if( ball_counter != 3 )
				{
					state = REVERSE;
				}
				else
				{
					state = STAHP;
				}
			}
			break;

		case REVERSE:
			arcadeDrive( -127, 0);
			Manipulate( 0, 0, 0, 0, 0 , 0, 0);
			imeGet(LEFT_IME, &left_ime);
			imeGet(RIGHT_IME, &right_ime);
			if( right_ime < -100 && left_ime > 100 )
			{
				state = TURN_LEFT;
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
			}
			break;

		case TURN_LEFT:
			arcadeDrive( 0, -127);
			Manipulate( 0, 0, 0, 0, 0 , 0, 0);
			imeGet(LEFT_IME, &left_ime);
			imeGet(RIGHT_IME, &right_ime);
			if( right_ime < -200 && left_ime < -200 )
			{
				state = DRIVE_FORWARD3;
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
			}
			break;

		case DRIVE_FORWARD3:
			arcadeDrive( 127, 0);
			Manipulate( 0, 0, 0, 0, 0 , 0, 0);
			imeGet(LEFT_IME, &left_ime);
			imeGet(RIGHT_IME, &right_ime);
			if( right_ime > 100 && left_ime < -100 )
			{
				state = TURN_RIGHT;
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
			}
			break;

		case STAHP:
			arcadeDrive( 0, 0);
			Manipulate( 0, 0, 0, 0, 0 , 0, 0);
			break;
	}
}
