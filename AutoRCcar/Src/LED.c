/*
 * LED.c
 *
 *  Created on: Nov 4, 2025
 *      Author: jaejun
 */


#include "LED.h"


LED_SET LED[3] = {
		{GPIOA, GPIO_PIN_5},
		{GPIOA, GPIO_PIN_6},
		{GPIOA, GPIO_PIN_7}
};


void LED_CON(uint8_t Remote){
	if(Remote == 0){	// LED OFF
		HAL_GPIO_WritePin(LED[0].port, LED[0].Pin_Num, 0);
		HAL_GPIO_WritePin(LED[1].port, LED[1].Pin_Num, 0);
		HAL_GPIO_WritePin(LED[2].port, LED[2].Pin_Num, 0);
	}
	if(Remote == 1){	// LED G ON
			HAL_GPIO_WritePin(LED[0].port, LED[0].Pin_Num, 0);
			HAL_GPIO_WritePin(LED[1].port, LED[1].Pin_Num, 1);
			HAL_GPIO_WritePin(LED[2].port, LED[2].Pin_Num, 0);
		}
	if(Remote == 2){	// LED B ON
			HAL_GPIO_WritePin(LED[0].port, LED[0].Pin_Num, 0);
			HAL_GPIO_WritePin(LED[1].port, LED[1].Pin_Num, 0);
			HAL_GPIO_WritePin(LED[2].port, LED[2].Pin_Num, 1);
		}
	if(Remote == 3){	// LED R ON
			HAL_GPIO_WritePin(LED[0].port, LED[0].Pin_Num, 1);
			HAL_GPIO_WritePin(LED[1].port, LED[1].Pin_Num, 0);
			HAL_GPIO_WritePin(LED[2].port, LED[2].Pin_Num, 0);
		}
	if(Remote == 4){	// LED W ON
			HAL_GPIO_WritePin(LED[0].port, LED[0].Pin_Num, 1);
			HAL_GPIO_WritePin(LED[1].port, LED[1].Pin_Num, 1);
			HAL_GPIO_WritePin(LED[2].port, LED[2].Pin_Num, 1);
		}

};
