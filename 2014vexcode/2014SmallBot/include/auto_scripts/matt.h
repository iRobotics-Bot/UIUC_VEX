/*
 * matt.h
 *
 *  Created on: April 22, 2014
 *      Author: Matt
 */

#ifndef MATT_H_
#define MATT_H_


/* MACROS */

#define FLING_BALL		1
#define	TURN1			2
#define	REVERSE1		3
#define	FORWARD1		4
#define	FORWARD2		5
#define	REVERSE2		6
#define	TURN2			7
#define	REVERSE3		8
#define	FORWARD3		9
#define	THROW			10
#define	FORWARD4		11
#define	TURN3			12
#define	FORWARD5		13
#define	DONE			14

#define	PAUSE1			100
#define	PAUSE2			101
#define	PAUSE3			102
#define	PAUSE4			103
#define	PAUSE5			104
#define	PAUSE6			105
#define	PAUSE7			106


/* GLOBAL VARIABLES */
static int state = REVERSE1;
static int i, stall_timer = 0;
static int left_ime, right_ime;


/* FUNCTION PROTOTYPES */
void GoMidterminators();
void Reset();


#endif /* MATT_H_ */
