/*
 * Delay_us.c
 *
 *  Created on: Oct 16, 2025
 *      Author: user4
 */

#include "Delay_us.h"

void Delay_us(uint16_t us){
	__HAL_TIM_SET_COUNTER(&htim11, 0);	// 타이머 11번을 0으로 초기화
	while((__HAL_TIM_GET_COUNTER(&htim11)) < us);

}
