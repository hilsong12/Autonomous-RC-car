/*
 * Motor_Con.h
 *
 *  Created on: Oct 29, 2025
 *      Author: user4
 */

#ifndef INC_MOTOR_CON_H_
#define INC_MOTOR_CON_H_
#include "main.h"
#include "tim.h"

typedef struct{
		GPIO_TypeDef	*port_IN1;
		uint16_t		Pinnumber_IN1;
		GPIO_TypeDef	*port_IN2;
		uint16_t		Pinnumber_IN2;
}Motor_SET;

extern Motor_SET Motor[2];

#define Left 0
#define Right 1

void Motor_Front(Motor_SET *Motor, uint32_t *Throttle);
void Motor_Reverse(Motor_SET *Motor, uint32_t *Throttle);
void Motor_Stop(Motor_SET *Motor, uint32_t *Throttle);
void Motor_A_Turn(Motor_SET *Motor, uint32_t *Throttle);
void Motor_D_Turn(Motor_SET *Motor, uint32_t *Throttle);
void Motor_Q_Turn(Motor_SET *Motor, uint32_t *Throttle);
void Motor_E_Turn(Motor_SET *Motor, uint32_t *Throttle);
void Motor_Z_Turn(Motor_SET *Motor, uint32_t *Throttle);
void Motor_C_Turn(Motor_SET *Motor, uint32_t *Throttle);

#endif /* INC_MOTOR_CON_H_ */
