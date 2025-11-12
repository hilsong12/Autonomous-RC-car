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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "Joy.h"
#include "Gear_Button.h"
#include "LED.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Left	0
#define Right 	1
#define L_X		0
#define L_Y		2
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

uint16_t Throttle[4];
uint8_t Rx1_Data;
uint8_t Rx1_Buffer[64];
volatile uint16_t Rx1_Write_Index = 0;
uint16_t Rx1_Read_Index = 0;

uint8_t Rx2_Data;
uint8_t Rx2_Buffer[64];
volatile uint16_t Rx2_Write_Index = 0;
uint16_t Rx2_Read_Index = 0;


uint8_t Remote = 0; // 전송할 값
uint8_t Gear_Shift = 0;
uint8_t Auto_Shift = 0;
uint8_t Direction = 0;
uint8_t LED_State = 0; // LED 컨트롤할 값





/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if(huart->Instance == USART1){	// BL에서 데이터를 받아오면
		Rx1_Buffer[Rx1_Write_Index] = Rx1_Data;
		Rx1_Write_Index = (Rx1_Write_Index+1) % 64;
		//HAL_UART_Transmit_IT(&huart2, &Rx1_Data, 1); // PC로 전달
		HAL_UART_Receive_IT(&huart1, &Rx1_Data,1);	// 다시 수신 대기
	}
	if(huart->Instance == USART2){ // PC에서 데이터가 오면
		Rx2_Buffer[Rx2_Write_Index] = Rx2_Data;
		Rx2_Write_Index = (Rx2_Write_Index+1) % 64;
		//HAL_UART_Transmit_IT(&huart2, &Rx2_Data, 1);	// BL로 전달
		HAL_UART_Receive_IT(&huart2, &Rx2_Data, 1);	// 수신대기
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
  MX_ADC1_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim11, TIM_CHANNEL_1); // 필요없음
  HAL_ADC_Start_DMA(&hadc1, Throttle, 4);
  HAL_UART_Receive_IT(&huart1, &Rx1_Data, 1);
  HAL_UART_Receive_IT(&huart2, &Rx2_Data, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(Rx2_Read_Index != Rx2_Write_Index){
		  HAL_UART_Transmit(&huart1, &Rx2_Buffer[Rx2_Read_Index], 1, 50);
		  Rx2_Read_Index = (Rx2_Read_Index+1) % 64;
	  }
	  if(Rx1_Read_Index != Rx1_Write_Index){
		  HAL_UART_Transmit(&huart2, &Rx1_Buffer[Rx1_Read_Index], 1, 50);
		  Rx1_Read_Index = (Rx1_Read_Index+1) % 64;
	  	  }

//	   remote[0] < 속도
//	   remote[1] < 방향
//	   SWB = 스로틀 속도
//	   SWA = 자율주행 ON / OFF


	  // SW들 값 받아오기 자율
	  if(Auto_Get() == 1){ // 자율주행 SW값 판독
		  Auto_Shift++;
		  if(Auto_Shift == 2){
			  Auto_Shift = 0;
			  Gear_Shift = 0;
		  }
	  }

	  if(Gear_Get() == 1){	// 변속 SW값 판독
		  Gear_Shift++;
		  if(Gear_Shift == 4) Gear_Shift = 0;
	  }

	  if(Auto_Shift == 1)  LED_State = 4;
	  else if(Gear_Shift == 0) LED_State = 0;
	  else if(Gear_Shift == 1) LED_State = 1;
	  else if(Gear_Shift == 2) LED_State = 2;
	  else if(Gear_Shift == 3) LED_State = 3;

	  LED_CON(LED_State); //  상황따라 LED

	  if(Throttle[L_Y] > 3500){ 						// Y축이 앞쪽으로 전진상태
		  if(Throttle[L_X] > 3500) Direction = 1;		// Q방향이동
		  else if(Throttle[L_X] < 500) Direction = 3;	// E방향이동
		  else Direction = 2;
	  }

	  else if(Throttle[L_Y] < 500){						// Y축이 아래쪽으로 후진상태
		  if(Throttle[L_X] > 3500) Direction = 7; 		// Z방향이동
		  else if(Throttle[L_X] < 500) Direction = 9; 	// C방향이동
		  else Direction = 8;
	  }

	  else{	// Y축이 중앙쪽
		  if(Throttle[L_X] > 3500) Direction = 4;
		  else if(Throttle[L_X] < 500) Direction = 6;
		  else Direction = 5;
	  }

	  // Remote[0] gear
	  // Remote[1] direction
	  // Remote[2] auto
	  //Print_Joy(Throttle);  < 모니터링용
	  // 배열로 만들어서 던져줄라했었는데 순서를 잘못읽어서 걍 숫자로 던짐
		Remote = 100 * Auto_Shift + Gear_Shift * 10 + Direction;

		printf("Gear : %d\n", Gear_Shift);
		printf("Direction : %d\n", Direction);
		printf("auto : %d\n", Auto_Shift);
		printf("Remote : %d\n", Remote);

		HAL_UART_Transmit(&huart1, &Remote, 1, 10);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

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
