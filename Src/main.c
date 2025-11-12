/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "LED.h"
#include "Delay_us.h"
#include "Ultra_Sonic.h"
#include "Auto.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Left	0
#define Right 	1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

int _write(int file, unsigned char* p, int len){

    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, p, len, 100);
    return (status == HAL_OK ? len : 0);
}



/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/*----------- 제어용 -----------*/
uint32_t Throttle[2];		// 모터 PWM 제어용 변수
uint8_t Remote_Data;		// 조이스틱에서 값 받아옴
uint8_t Auto_Shift;			// Auto 모드 여부
uint8_t Gear_Shift;			// PWM 단계별 구분용
uint8_t Direction;			// 이동할 방향

/*-----------초음파용----------------*/

uint16_t IC_Value_1[3] = {0};
uint16_t IC_Value_2[3] = {0};
uint16_t Echo_Time[3] = {3};
uint16_t Capture_Flag[3] = {0};		// 초음파 수신용 Flag
uint16_t Temp_Dis[3][6] = {0}; 		// 이동평균 사용 // [][0]은 입력용
uint16_t Distance[3] = {0};			// 거리
uint8_t US_F[3] = {0};				// 초음파 수신완료 확인용 Flag


/*----------- 동작기억용 -----------*/

uint8_t Last_ACT = 0;				// 마지막 동작 기억
uint8_t Last_Auto_Shift = 0;		// 직전 자율주행 여부를 기억해줌



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1){ 			// 블루투스 통신용
		//printf("Remote Data : %d\n",Remote_Data); 		// 블루투스 연결시 시간 소요 많아서 주석
		HAL_UART_Receive_DMA(&huart1, &Remote_Data, sizeof(Remote_Data));
	}

}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){	// 좌측 ECHO 입력
	  if(Capture_Flag[US_L] == 0){ 	// 상승 신호 받음
		  IC_Value_1[US_L] = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);
		  Capture_Flag[US_L] = 1; // 캡쳐 플래그 세움 ( 캡쳐했음 )
		  __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
	  }
	  else if(Capture_Flag[US_L] == 1){	// 하강 신호 받음
		  IC_Value_2[US_L] = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);
		  __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
		  if(IC_Value_2[US_L] > IC_Value_1[US_L]){
			  Echo_Time[US_L] = IC_Value_2[US_L] - IC_Value_1[US_L];	// 이상적인 case
		  }
		  else if(IC_Value_2[US_L] < IC_Value_1[US_L]){			// 운이 나쁘게 타이머가 끝나버린 뒤 두번째 캡쳐 받음
			  Echo_Time[US_L] = (0xffff)-IC_Value_1[US_L] + IC_Value_2[US_L];
		  };
		  // 5개 평균내서 사용함 < 튀는 값 영향 감소
		  Temp_Dis[US_L][0] = Echo_Time[US_L] / 58;
		  if(Temp_Dis[US_L][0] - Temp_Dis[US_L][1] > 50){
			  Temp_Dis[US_L][0] = Temp_Dis[US_L][1];
			  //Last_ACT = LEFT;
		  }

		  for(uint8_t i = 5; i > 0; i--){
			  Temp_Dis[US_L][i] = Temp_Dis[US_L][i-1];
		  }
		  Distance[US_L] = 0;
		  for(uint8_t u = 1; u <= 5; u++){
			  Distance[US_L] += Temp_Dis[US_L][u];
		  }
		  Distance[US_L] /= 5;
		  Capture_Flag[US_L] = 0;
		  US_F[US_L] = 1;
		  //__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC1);
	  }
  }
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){	// 중앙 ECHO 입력
  	  if(Capture_Flag[US_C] == 0){ 	// 상승 신호 받음
  		  IC_Value_1[US_C] = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2);
  		  Capture_Flag[US_C] = 1; // 캡쳐 플래그 세움 ( 캡쳐했음 )
  		  __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
  	  }
  	  else if(Capture_Flag[US_C] == 1){	// 하강 신호 받음
  		  IC_Value_2[US_C] = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2);
  		  __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);

  		  if(IC_Value_2[US_C] > IC_Value_1[US_C]){
  			  Echo_Time[US_C] = IC_Value_2[US_C] - IC_Value_1[US_C];	// 이상적인 case
  		  }
  		  else if(IC_Value_2[US_C] < IC_Value_1[US_C]){			// 운이 나쁘게 타이머가 끝나버린 뒤 두번째 캡쳐 받음
  			  Echo_Time[US_C] = (0xffff)-IC_Value_1[US_C] + IC_Value_2[US_C];
  		  }

		  Temp_Dis[US_C][0] = Echo_Time[US_C] / 58;
		  if(Temp_Dis[US_C][0] - Temp_Dis[US_C][1] > 50) Temp_Dis[US_C][0] = Temp_Dis[US_C][1];
		  for(uint8_t i = 5; i > 0; i--){
			  Temp_Dis[US_C][i] = Temp_Dis[US_C][i-1];
		  }
		  Distance[US_C] = 0;
		  for(uint8_t u = 1; u <= 5; u++){
			  Distance[US_C] += Temp_Dis[US_C][u];
		  }
		  Distance[US_C] /= 5;
  		  Capture_Flag[US_C] = 0;
  		US_F[US_C] = 1;
  		  //__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC1);
  	  }
    }
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4){	// 우측 ECHO 입력
  	  if(Capture_Flag[US_R] == 0){ 	// 상승 신호 받음
  		  IC_Value_1[US_R] = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_4);
  		  Capture_Flag[US_R] = 1; // 캡쳐 플래그 세움 ( 캡쳐했음 )
  		  __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
  	  }
  	  else if(Capture_Flag[US_R] == 1){	// 하강 신호 받음
  		  IC_Value_2[US_R] = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_4);
  		  __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);

  		  if(IC_Value_2[US_R] > IC_Value_1[US_R]){
  			  Echo_Time[US_R] = IC_Value_2[US_R] - IC_Value_1[US_R];	// 이상적인 case
  		  }
  		  else if(IC_Value_2[US_R] < IC_Value_1[US_R]){			// 운이 나쁘게 타이머가 끝나버린 뒤 두번째 캡쳐 받음
  			  Echo_Time[US_R] = (0xffff)-IC_Value_1[US_R] + IC_Value_2[US_R];
  		  }

  		Temp_Dis[US_R][0] = Echo_Time[US_R] / 58;
  		if(Temp_Dis[US_R][0] - Temp_Dis[US_R][1] > 50){
  			Temp_Dis[US_R][0] = Temp_Dis[US_R][1];
  			//Last_ACT = RIGHT;
  		}
  		for(uint8_t i = 5; i > 0; i--){
  				Temp_Dis[US_R][i] = Temp_Dis[US_R][i-1];
  		}
  		Distance[US_R] = 0;
  		for(uint8_t u = 1; u <= 5; u++){
  				Distance[US_R] += Temp_Dis[US_R][u];
  		}
  			Distance[US_R] /= 5;
  			Capture_Flag[US_R] = 0;
  			US_F[US_R] = 1;
  		  //__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC1);
  	  }
    }
}


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_DMA(&huart1, &Remote_Data, sizeof(Remote_Data));
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);								//모터 듀티 생성용
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);							//초음파 센서용
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
  HAL_TIM_Base_Start(&htim11);											//us 딜레이 생성용
  printf("----- boot -----\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1){


	  //Remote_Data = 100;  // 테스트용
	  // 조이스틱에서 값을 Remote 데이터로 통째로 가지고옴
	  // 백의 자리 < auto // 십의 자리 <  gear // 일의 자리 < dir

	  Auto_Shift = Remote_Data / 100;
	  Gear_Shift = (Remote_Data % 100) / 10;
	  Direction = Remote_Data % 10;
	  	  if(Auto_Shift == 1){ // 자율주행 모드

	  		  if(Last_Auto_Shift == 0){ // 직전 상태가 자율주행 모드가 아닌경우
	  			  HC_SR04_TRIGGER();	// 거리 트리거를 쏴줌 > 자율주행 판단시작 기준이 초음파 센서 수신 완료가 조건임
	  		  }
	  		  LED_CON(4);				// LED W 색상 출력
	  		  Last_ACT = Auto_Mode(US_F, Distance, Last_ACT, Throttle);		// Auto 모드 실행

	  		  Last_Auto_Shift = 1;		// 자율주행 모드였다고 업데이트
	  	  }
	  	  else{	// 메뉴얼 모드
	  		  	LED_CON(Gear_Shift);	// gear 단계별 LED 출력 > 빨라질 수록 G B R 순으로 변경
	  		  	Last_Auto_Shift = 0;	// 자율주행 모드가 아니였다고 업데이트

	  			switch (Gear_Shift) {	// 기어 단계별 PWM 듀티비 조절 // 4096이 MAX라 생각하면됨 TIM2 ARR 참고
	  			case 0:
	  				Throttle[0] = 0;
	  				Throttle[1] = 0;
	  				break;
	  			case 1:
	  				Throttle[0] = 2000;
	  				Throttle[1] = 2000;
	  				break;
	  			case 2:
	  				Throttle[0] = 3000;
	  				Throttle[1] = 3000;
	  				break;
	  			case 3:
	  				Throttle[0] = 4000;
	  				Throttle[1] = 4000;
	  				break;
	  			case 4:
	  				Throttle[0] = 4000;
	  				Throttle[1] = 4000;
	  				break;
	  			default:
	  				Throttle[0] = 0;
	  				Throttle[1] = 0;
	  				break;
	  			}

	  			switch (Direction) {	// 이동방향 출력
	  			case 1:
	  				Motor_E_Turn(Motor, Throttle);
	  				break;
	  			case 2:
	  				Motor_Front(Motor, Throttle);
	  				break;
	  			case 3:
	  				Motor_Q_Turn(Motor, Throttle);
	  				break;
	  			case 4:
	  				Motor_D_Turn(Motor, Throttle);
	  				break;
	  			case 5:
	  				Motor_Stop(Motor, Throttle);
	  				break;
	  			case 6:
	  				Motor_A_Turn(Motor, Throttle);
	  				break;
	  			case 7:
	  				Motor_C_Turn(Motor, Throttle);
	  				break;
	  			case 8:
	  				Motor_Reverse(Motor, Throttle);
	  				break;
	  			case 9:
	  				Motor_Z_Turn(Motor, Throttle);
	  				break;
	  			default:
	  				Motor_Stop(Motor, Throttle);
	  				break;
	  		}

	  	}		// 메뉴얼 모드 끝

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  } // while
  /* USER CODE END 3 */
} // main

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
