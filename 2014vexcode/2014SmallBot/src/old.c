#if 0

/* This file is for storing old algorithms no longer used,
 * but still saved for future reference
 */


/*** THROW BALL ***/

/* This code was used to throw the ball using elbow joints in the arm.
 * It was later ditched because the timing needed to be too precise.
 */

int throw_state = THROW_INIT, ime_count = 0, gyroOffset = 0;

#define THROW_INIT	0x00
#define	FORWARD		0x01
#define	REVERSE		0x02
#define	DROP		0x03

/* helper function to throw the ball */
void throw()
{
	arm_PID_L.observed = analogReadCalibrated( LEFTPOT );
	arm_PID_R.observed = analogReadCalibrated( RIGHTPOT );
	switch (throw_state) {
		case THROW_INIT :
			if(arm_PID_L.observed > CARRY_LIMIT_L - 400 &&   /* if arm is in carry range */
					arm_PID_L.observed < CARRY_LIMIT_L + 200){
				imeReset( RIGHT_IME ); // reset ime values
				imeReset( LEFT_IME );
				throw_state = REVERSE;
			} else {
				throw_flag = FALSE;  // allow driver input if not able to throw
			}
			break;
		case FORWARD :  /* robot drives forward a bit */
			TankDrive(0, 0);
			Manipulate( -127, 0, 0, 0, 0);
			imeGet(LEFT_IME, &ime_count);
			if (ime_count > 75){
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				throw_state = REVERSE;
			}
			break;
		case REVERSE :  /* robot drives backwards a bit */
			TankDrive(-60, -60);
			imeGet(LEFT_IME, &ime_count);
				if(ime_count < -50){
					dump_flag = TRUE;
					while(dump_flag){
						Manipulate(0, 0, 0, 0, 0);
						if(arm_PID_L.observed > HORIZONTAL_L - 200 &&   /* if arm is in dump range */
													arm_PID_L.observed < HORIZONTAL_L + 200){
							digitalWrite(1, HIGH);
							digitalWrite(2, HIGH);
						}
					}
				}
			if (ime_count < -100){
				imeReset( LEFT_IME );
				imeReset( RIGHT_IME );
				throw_state = DROP;
			}
			break;
		case DROP :		/* return arm to original position */
			dump_flag = FALSE;
			TankDrive(0, 0);
			Manipulate(0, 0, 0, 0, 0);
			if(arm_PID_L.observed > CARRY_LIMIT_L - 200 &&   /* if arm is in carry range */
								arm_PID_L.observed < CARRY_LIMIT_L + 200){
				digitalWrite(1, LOW);
				digitalWrite(2, LOW);
				throw_state = THROW_INIT;
				throw_flag = FALSE; /* re-enable user control */
			}
			break;
	}
}

#endif
