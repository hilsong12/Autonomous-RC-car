/*
 * Joy.c
 *
 *  Created on: Oct 28, 2025
 *      Author: user4
 */

#include "Joy.h"

void Print_Joy(uint32_t *Throttle){
	printf("Left_Throttle : %lu\n", Throttle[0]);
	printf("Right Throttle : %lu\n", Throttle[1]);
}
