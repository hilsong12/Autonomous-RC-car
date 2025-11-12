/*
 * Auto.h
 *
 *  Created on: Nov 5, 2025
 *      Author: jaejun
 */

#ifndef INC_AUTO_H_
#define INC_AUTO_H_

#include "main.h"
#include "Motor_Con.h"
#include "stdio.h"
#include "Ultra_Sonic.h"


#define US_L	0
#define US_C	1
#define	US_R	2

enum{
	STOP,
	FRONT,
	LEFT,
	RIGHT,
	REVERSE,
	MOVEQ,
	MOVEE
};

uint8_t Auto_Mode(uint8_t *US_F, uint16_t *Distance, uint8_t Last_ACT, uint32_t *Throttle);
//
#endif /* INC_AUTO_H_ */
