/*
 * LED.c
 *
 *  Created on: Oct 30, 2025
 *      Author: user4
 */

#include "LED.h"

LED_SET LED[3] = {
		{GPIOC, GPIO_PIN_12},
		{GPIOC, GPIO_PIN_10},
		{GPIOC, GPIO_PIN_11}
};


void LED_CON(uint8_t Remote){

	switch(Remote){
			case 0:
				HAL_GPIO_WritePin(LED[0].port, LED[0].Pin_Number,0);
				HAL_GPIO_WritePin(LED[1].port, LED[1].Pin_Number,0);
				HAL_GPIO_WritePin(LED[2].port, LED[2].Pin_Number,0);
			break;

			case 1:
				HAL_GPIO_WritePin(LED[0].port, LED[0].Pin_Number,1);
				HAL_GPIO_WritePin(LED[1].port, LED[1].Pin_Number,0);
				HAL_GPIO_WritePin(LED[2].port, LED[2].Pin_Number,0);
			break;

			case 2:
				HAL_GPIO_WritePin(LED[0].port, LED[0].Pin_Number,0);
				HAL_GPIO_WritePin(LED[1].port, LED[1].Pin_Number,1);
				HAL_GPIO_WritePin(LED[2].port, LED[2].Pin_Number,0);
			break;

			case 3:
				HAL_GPIO_WritePin(LED[0].port, LED[0].Pin_Number,0);
				HAL_GPIO_WritePin(LED[1].port, LED[1].Pin_Number,0);
				HAL_GPIO_WritePin(LED[2].port, LED[2].Pin_Number,1);
			break;

			case 4:
				HAL_GPIO_WritePin(LED[0].port, LED[0].Pin_Number,1);
				HAL_GPIO_WritePin(LED[1].port, LED[1].Pin_Number,1);
				HAL_GPIO_WritePin(LED[2].port, LED[2].Pin_Number,1);
			break;

			default:
				HAL_GPIO_WritePin(LED[0].port, LED[0].Pin_Number,0);
				HAL_GPIO_WritePin(LED[1].port, LED[1].Pin_Number,0);
				HAL_GPIO_WritePin(LED[2].port, LED[2].Pin_Number,0);
			break;
	}
}
