/*
 * test_functions.c
 *
 *  Created on: Feb 2, 2016
 *      Author: Kin Man
 */

#include "test_functions.h"

void test_shooter(bool is_on)
{
	if(is_on) {
		motorSet(1,127);
		motorSet(2,127);
		motorSet(3,127);
		motorSet(4,-127);
	} else {
		motorSet(1,0);
		motorSet(2,0);
		motorSet(3,0);
		motorSet(4,0);
	}
}
