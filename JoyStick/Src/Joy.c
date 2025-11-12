/*
 * Joy.c
 *
 *  Created on: Oct 28, 2025
 *      Author: user4
 */

#include "Joy.h"
#include "main.h"
#include "adc.h"

void Print_Joy(uint16_t *Throttle){
	printf("Left_X : %d\n", Throttle[0]);
	printf("Left_Y : %d\n", Throttle[2]);
	HAL_Delay(500);
	//왼쪽 방향
	//오른쪽 속도
	//printf("Right_X : %d\n", Throttle[1]);
	//printf("Right_Y : %d\n", Throttle[3]);

}

void Read_Joy_Data(uint32_t *Throttle){
	Throttle[0] = HAL_ADC_GetValue(&hadc1);
	Throttle[1] = HAL_ADC_GetValue(&hadc1);

}
