/*
 * Ultra_Sonic.h
 *
 *  Created on: Nov 3, 2025
 *      Author: user4
 */

#ifndef INC_ULTRA_SONIC_H_
#define INC_ULTRA_SONIC_H_

#include "main.h"
#include "Delay_us.h"
#include "tim.h"

#define US_L	0
#define US_C	1
#define	US_R	2



typedef struct{
		GPIO_TypeDef 	*port_TRIG;
		uint16_t		Pin_TRIG;
		GPIO_TypeDef 	*port_ECHO;
		uint16_t		Pin_ECHO;

}Sonic_Set;

void HC_SR04_TRIGGER();




#endif /* INC_ULTRA_SONIC_H_ */
