/*
 * LED.h
 *
 *  Created on: Nov 4, 2025
 *      Author: jaejun
 */

#ifndef INC_LED_H_
#define INC_LED_H_


#include "main.h"

typedef struct{
		GPIO_TypeDef 	*port;
		uint8_t			Pin_Num;
}LED_SET;


void LED_CON(uint8_t Remote);

#endif /* INC_LED_H_ */
