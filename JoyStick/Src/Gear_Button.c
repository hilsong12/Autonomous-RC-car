/*
 * Gear_Button.c
 *
 *  Created on: Oct 30, 2025
 *      Author: user4
 */

#include "Gear_Button.h"


bool Auto_Get(){
	static uint32_t Pre_Time = 0;
	static uint8_t Pre_State = 1; // 안눌려짐을 기본 값으로
	uint32_t Curr_Time = HAL_GetTick();
	bool Ret = false;//

	bool Curr_State = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8);

	if(Curr_State == 0){
		if(Curr_Time - Pre_Time > 100){
			if(Pre_State == 1) Ret = true;
		}
		Pre_Time = Curr_Time;
		Pre_State = 0;
	}
	else{
		Pre_State = 1;
	}
	return Ret;
}




bool Gear_Get(){
	static uint32_t Pre_Time = 0;
	static uint8_t Pre_State = 1; // 안눌려짐을 기본 값으로
	uint32_t Curr_Time = HAL_GetTick();
	bool Ret = false;//

	bool Curr_State = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2);

	if(Curr_State == 0){
		if(Curr_Time - Pre_Time > 100){
			if(Pre_State == 1) Ret = true;
		}
		Pre_Time = Curr_Time;
		Pre_State = 0;
	}
	else{
		Pre_State = 1;
	}
	return Ret;
}
