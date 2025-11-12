/*
 * Ultra_Sonic.c
 *
 *  Created on: Nov 3, 2025
 *      Author: user4
 */

#include "Ultra_Sonic.h"


Sonic_Set Sonic[3] = {
		{GPIOC, GPIO_PIN_10, GPIOB, GPIO_PIN_4}, // 좌
		{GPIOC, GPIO_PIN_11, GPIOB, GPIO_PIN_5}, // 중
		{GPIOD, GPIO_PIN_2, GPIOC, GPIO_PIN_9}, // 우

};


void HC_SR04_TRIGGER(){
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	HAL_GPIO_WritePin(Sonic[US_L].port_TRIG, Sonic[US_L].Pin_TRIG, 0);
	HAL_GPIO_WritePin(Sonic[US_C].port_TRIG, Sonic[US_C].Pin_TRIG, 0);
	HAL_GPIO_WritePin(Sonic[US_R].port_TRIG, Sonic[US_R].Pin_TRIG, 0);
	Delay_us(1);
	HAL_GPIO_WritePin(Sonic[US_L].port_TRIG, Sonic[US_L].Pin_TRIG, 1);
	HAL_GPIO_WritePin(Sonic[US_C].port_TRIG, Sonic[US_C].Pin_TRIG, 1);
	HAL_GPIO_WritePin(Sonic[US_R].port_TRIG, Sonic[US_R].Pin_TRIG, 1);
	Delay_us(10);
	HAL_GPIO_WritePin(Sonic[US_L].port_TRIG, Sonic[US_L].Pin_TRIG, 0);
	HAL_GPIO_WritePin(Sonic[US_C].port_TRIG, Sonic[US_C].Pin_TRIG, 0);
	HAL_GPIO_WritePin(Sonic[US_R].port_TRIG, Sonic[US_R].Pin_TRIG, 0);
	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC1);
};
