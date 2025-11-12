/*
 * Motor_Con.c
 *
 *  Created on: Oct 29, 2025
 *      Author: user4
 */

#include "Motor_Con.h"

Motor_SET Motor[2] = {
		{GPIOC, GPIO_PIN_6, GPIOC, GPIO_PIN_8},	// 좌측
		{GPIOB, GPIO_PIN_14, GPIOB, GPIO_PIN_15}	// 우측
};
//		GPIO_TypeDef	*port_IN1;
//		uint16_t		Pinnumber_IN1;//
//		GPIO_TypeDef	*port_IN2;
//		uint16_t		Pinnumber_IN2;

void Motor_Front(Motor_SET *Motor, uint32_t *Throttle){		//전진 조건 SET

	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Throttle[Left]);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Throttle[Right]);

	HAL_GPIO_WritePin(Motor[Left].port_IN1, Motor[Left].Pinnumber_IN1, 1);
	HAL_GPIO_WritePin(Motor[Left].port_IN2, Motor[Left].Pinnumber_IN2, 0);
	HAL_GPIO_WritePin(Motor[Right].port_IN1, Motor[Right].Pinnumber_IN1, 1);
	HAL_GPIO_WritePin(Motor[Right].port_IN2, Motor[Right].Pinnumber_IN2, 0);
}

void Motor_Reverse(Motor_SET *Motor, uint32_t *Throttle){	//후진 조건 SET
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Throttle[Left]);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Throttle[Right]);

	HAL_GPIO_WritePin(Motor[Left].port_IN1, Motor[Left].Pinnumber_IN1, 0);
	HAL_GPIO_WritePin(Motor[Left].port_IN2, Motor[Left].Pinnumber_IN2, 1);
	HAL_GPIO_WritePin(Motor[Right].port_IN1, Motor[Right].Pinnumber_IN1, 0);
	HAL_GPIO_WritePin(Motor[Right].port_IN2, Motor[Right].Pinnumber_IN2, 1);
}

void Motor_Stop(Motor_SET *Motor, uint32_t *Throttle){	//후진 조건 SET
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);

	HAL_GPIO_WritePin(Motor[Left].port_IN1, Motor[Left].Pinnumber_IN1, 1);
	HAL_GPIO_WritePin(Motor[Left].port_IN2, Motor[Left].Pinnumber_IN2, 1);
	HAL_GPIO_WritePin(Motor[Right].port_IN1, Motor[Right].Pinnumber_IN1, 1);
	HAL_GPIO_WritePin(Motor[Right].port_IN2, Motor[Right].Pinnumber_IN2, 1);
}

void Motor_A_Turn(Motor_SET *Motor, uint32_t *Throttle){	// a방향 탱크턴
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Throttle[Left]);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Throttle[Right]);

	HAL_GPIO_WritePin(Motor[Left].port_IN1, Motor[Left].Pinnumber_IN1, 0);
	HAL_GPIO_WritePin(Motor[Left].port_IN2, Motor[Left].Pinnumber_IN2, 1);
	HAL_GPIO_WritePin(Motor[Right].port_IN1, Motor[Right].Pinnumber_IN1, 1);
	HAL_GPIO_WritePin(Motor[Right].port_IN2, Motor[Right].Pinnumber_IN2, 0);
}
void Motor_D_Turn(Motor_SET *Motor, uint32_t *Throttle){	// d방향 탱크턴
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Throttle[Left]);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Throttle[Right]);
	HAL_GPIO_WritePin(Motor[Left].port_IN1, Motor[Left].Pinnumber_IN1, 1);
	HAL_GPIO_WritePin(Motor[Left].port_IN2, Motor[Left].Pinnumber_IN2, 0);
	HAL_GPIO_WritePin(Motor[Right].port_IN1, Motor[Right].Pinnumber_IN1, 0);
	HAL_GPIO_WritePin(Motor[Right].port_IN2, Motor[Right].Pinnumber_IN2, 1);
}

void Motor_Q_Turn(Motor_SET *Motor, uint32_t *Throttle){	// 전진 + 좌회전 조건
	Throttle[Left] = Throttle[Left]/5;

	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Throttle[Left]);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Throttle[Right]);

	HAL_GPIO_WritePin(Motor[Left].port_IN1, Motor[Left].Pinnumber_IN1, 1);
	HAL_GPIO_WritePin(Motor[Left].port_IN2, Motor[Left].Pinnumber_IN2, 0);
	HAL_GPIO_WritePin(Motor[Right].port_IN1, Motor[Right].Pinnumber_IN1, 1);
	HAL_GPIO_WritePin(Motor[Right].port_IN2, Motor[Right].Pinnumber_IN2, 0);
}

void Motor_E_Turn(Motor_SET *Motor, uint32_t *Throttle){	// 전진 + 좌회전 조건
	Throttle[Right] = Throttle[Right]/5;

	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Throttle[Left]);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Throttle[Right]);

	HAL_GPIO_WritePin(Motor[Left].port_IN1, Motor[Left].Pinnumber_IN1, 1);
	HAL_GPIO_WritePin(Motor[Left].port_IN2, Motor[Left].Pinnumber_IN2, 0);
	HAL_GPIO_WritePin(Motor[Right].port_IN1, Motor[Right].Pinnumber_IN1, 1);
	HAL_GPIO_WritePin(Motor[Right].port_IN2, Motor[Right].Pinnumber_IN2, 0);

}


void Motor_Z_Turn(Motor_SET *Motor, uint32_t *Throttle){	// 전진 + 좌회전 조건
	Throttle[Left] = Throttle[Left]/5;

	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Throttle[Left]);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Throttle[Right]);

	HAL_GPIO_WritePin(Motor[Left].port_IN1, Motor[Left].Pinnumber_IN1, 0);
	HAL_GPIO_WritePin(Motor[Left].port_IN2, Motor[Left].Pinnumber_IN2, 1);
	HAL_GPIO_WritePin(Motor[Right].port_IN1, Motor[Right].Pinnumber_IN1, 0);
	HAL_GPIO_WritePin(Motor[Right].port_IN2, Motor[Right].Pinnumber_IN2, 1);
}


void Motor_C_Turn(Motor_SET *Motor, uint32_t *Throttle){	// 전진 + 좌회전 조건
	Throttle[Right]= Throttle[Right]/5;

	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Throttle[Left]);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Throttle[Right]);

	HAL_GPIO_WritePin(Motor[Left].port_IN1, Motor[Left].Pinnumber_IN1, 0);
	HAL_GPIO_WritePin(Motor[Left].port_IN2, Motor[Left].Pinnumber_IN2, 1);
	HAL_GPIO_WritePin(Motor[Right].port_IN1, Motor[Right].Pinnumber_IN1, 0);
	HAL_GPIO_WritePin(Motor[Right].port_IN2, Motor[Right].Pinnumber_IN2, 1);
}



