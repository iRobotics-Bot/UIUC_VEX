#include "shooter.h"

void shooter (PID_t * data, double speed) {
		/* change setpoint of PID to the desired input speed */
		data->setpoint = speed;
		/* Uncomment this if using IME to get observed velocity */
//		imeGetVelocity(IME_ADDR, &ime_velocity);
		/* Uncomment this if IME hasn't been implemented */
		data->observed = speed - 10;
		/* Get the PID output to add to desired motor speed */
		double output = PID(data);

		/* Add output onto desired motor speed */
		double desired = data->setpoint + output;
		motor( SHOOTERMTRL1,  desired);
		motor( SHOOTERMTRL2,  desired);
		motor( SHOOTERMTRR1,  desired);
		motor( SHOOTERMTRR2,  desired);
}
