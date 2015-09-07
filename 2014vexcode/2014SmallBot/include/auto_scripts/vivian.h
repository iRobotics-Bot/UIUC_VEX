/*
 * vivian.h
 *
 *  Created on: Jan 31, 2014
 *      Author: Matt
 */

#ifndef VIVIAN_H_
#define VIVIAN_H_

/* State Macro Names */
#define GO1			0
#define ARC_LEFT	1
#define GO2			2
#define PICK_UP		3
#define REVERSE1 	4
#define WAIT		5
#define REVERSE2	6
#define GO3			7
#define DUMP 		8
#define INIT		9



/* FOR EACH SIDE */
void auto_win_red();
void auto_win_blue();

#endif /* VIVIAN_H_ */
