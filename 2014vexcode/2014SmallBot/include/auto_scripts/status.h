/*
 * status.h
 *
 *  Created on: Apr 19, 2014
 *      Author: Status
 */

#ifndef STATUS_H_
#define STATUS_H_

/* State Macro Names */
#define GO1			0
#define ROTATE1		1
#define LAUNCH1		2
#define ROTATE2		3
#define GO2		 	4
#define ROTATE3		5
#define LAUNCH2		6
#define WAIT		8
#define INIT		9
#define BACKWARD1	10
#define BACKWARD2	11

#define RED			0
#define BLUE		1



/* FOR EACH SIDE */

void FUCKYEAH_AUTONOMOUS(int color);

#endif /* STATUS_H_ */
