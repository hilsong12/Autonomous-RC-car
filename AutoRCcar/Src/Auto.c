/*
 * Auto.c
 *
 *  Created on: Nov 5, 2025
 *      Author: jaejun
 */

#include "Auto.h"

uint8_t Auto_Mode(uint8_t *US_F, uint16_t *Distance, uint8_t Last_ACT, // 사실상 자율주행 함수임
		uint32_t *Throttle) {

	uint8_t Auto_Pilot = 0;			// 동작모드 변수
	Throttle[0] = 4000;	 			// 주행속도 조절
	Throttle[1] = 4000;
	if ((US_F[US_L] == 1) && (US_F[US_C] == 1) && (US_F[US_R] == 1)) { // 초음파 센서 수신완료 여부체크 < 수신완료되어야만 자율주행 판단 시작
		US_F[US_L] = 0; // US FLAG 초기화
		US_F[US_C] = 0;
		US_F[US_R] = 0;

	printf("LEFT : %d || CENTER : %d || RIGHT : %d  cm\n", Distance[US_L],Distance[US_C], Distance[US_R]);		// 시리얼 출력용

	if(Distance[US_C] >= 25) {		// 전방에 여유가 있을때
		Throttle[0] = 4000;			// 조금 가속해줌
		Throttle[1] = 4000;

		if(Last_ACT == FRONT || Last_ACT == STOP || Last_ACT == REVERSE || Last_ACT == MOVEE || Last_ACT == MOVEQ){	// 좌 / 우 회전하지 않을때만
			if(Distance[US_R] - Distance[US_L] > 10){			// 넓은쪽으로 이동하며 직진 1
				Auto_Pilot = MOVEE;
				printf("centering 1\n");
			}
			else if(Distance[US_L] - Distance[US_R] > 10){		// 넓은쪽으로 이동하며 직진 2
				Auto_Pilot = MOVEQ;
				printf("centering 2 \n");
			}
			else {
				Auto_Pilot = FRONT;								// 좌우가 어느정도 맞으면 직진
				printf("centering3 front\n");
			}
		}
//		else {	 // 이유는 모르겠는데 없어도 잘 돌아감
//			if(Last_ACT == LEFT) {
//				Auto_Pilot = MOVEQ;
//				printf("centering 1\n");
//			}
//			else{
//					Auto_Pilot = MOVEE;
//					printf("centering 2\n");
//				}
//			}
		}

		else if(Distance[US_C] >= 20) {		// 중간쯤
			if ((Distance[US_R] >= 15) && (Distance[US_L] >= 15)) {
				if(Last_ACT == FRONT || Last_ACT == STOP || Last_ACT == REVERSE || Last_ACT == MOVEE || Last_ACT == MOVEQ) {
					if (Distance[US_R] - Distance[US_L] > 10) {
						Auto_Pilot = MOVEE;
						printf("centering 1-1\n");
					}
					else if (Distance[US_L] - Distance[US_R] > 10) {
						Auto_Pilot = MOVEQ;
						printf("centering 2-1 \n");
					}
				}
				Auto_Pilot = FRONT;
				printf("1 case\n");
			}
			else if (Distance[US_R] >= 15) {
				Auto_Pilot = RIGHT;
				printf("2 case\n");
			}
			else if (Distance[US_L] >= 15) {
				Auto_Pilot = LEFT;
				printf("3 case\n");
			}
			else {
				if (Last_ACT != REVERSE)
				Auto_Pilot = REVERSE;
				else {
					if (Distance[US_R] > Distance[US_L])
					Auto_Pilot = RIGHT;
					else
					Auto_Pilot = LEFT;
				}
				printf("4 case\n");
			}
		}

		else {			// 전방과 근접할때
			if ((Distance[US_R] < 10) && (Distance[US_L] < 10)) {
				if (Last_ACT != REVERSE)
				Auto_Pilot = REVERSE;
				else {
					if (Distance[US_R] > Distance[US_L])
					Auto_Pilot = RIGHT;
					else
					Auto_Pilot = LEFT;
				}
				printf("5 case\n");
			}

			// 도리도리 방지용 코드
			// 도리도리 < 좌우로 왔다갔다하면서 무한 반복
			// 그래서 도리도리하기전에 여유가 있는쪽으로 돌기시작하면 해당 방향으로 계속돌도록 조건줌 < 이게 유턴의 원인이었던 듯 ?
			else if(Distance[US_L] >= 10 && Last_ACT == LEFT) {
				Auto_Pilot = LEFT;
				printf("6 case\n");
			} else if (Distance[US_R] >= 10 && Last_ACT == RIGHT) {
				Auto_Pilot = RIGHT;
				printf("7 case\n");
			}
			else if(Distance[US_R] > Distance[US_L]) {
				Auto_Pilot = RIGHT;
				printf("8 case\n");
			} else {
				Auto_Pilot = LEFT;
				printf("9 case\n");
			}
		}
		//Auto_Pilot = STOP; // 거리 측정시 활용
		switch (Auto_Pilot) {		// 모터 값 출력
			case STOP:
			Motor_Stop(Motor, Throttle);
			Last_ACT = STOP;
			printf("Auto_Pilot : STOP\n");
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);// 해당 친구들 부저 출력
			break;
			case FRONT:
			Motor_Front(Motor, Throttle);
			Last_ACT = FRONT;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
			printf("Auto_Pilot : FRONT\n");
			break;
			case LEFT:
			Motor_D_Turn(Motor, Throttle);
			Last_ACT = LEFT;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
			printf("Auto_Pilot : LEFT\n");
			break;
			case RIGHT:
			Motor_A_Turn(Motor, Throttle);
			Last_ACT = RIGHT;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
			printf("Auto_Pilot : RIGHT\n");
			break;
			case MOVEE:
			Motor_Q_Turn(Motor, Throttle);
			Last_ACT = MOVEE;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
			printf("Auto_Pilot : MOVEE\n");
			break;
			case MOVEQ:
			Motor_E_Turn(Motor, Throttle);
			Last_ACT = MOVEQ;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
			printf("Auto_Pilot : MOVEQ\n");
			break;
			case REVERSE:
			Motor_Reverse(Motor, Throttle);
			Last_ACT = REVERSE;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
			printf("Auto_Pilot : REVERSE\n");
			break;
			default:
			Motor_Stop(Motor, Throttle);
			Last_ACT = STOP;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
			printf("Auto_Pilot : STOP\n");
			break;
		}
		HC_SR04_TRIGGER();	// 거리측정 시작
	}

	return Last_ACT;
}

