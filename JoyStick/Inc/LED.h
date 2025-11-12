/*
 * LED.h
 *
 *  Created on: Oct 30, 2025
 *      Author: user4
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"


typedef struct{
	GPIO_TypeDef *port;
	uint16_t Pin_Number;
}LED_SET;


void LED_CON(uint8_t Remote);

#endif /* INC_LED_H_ */




