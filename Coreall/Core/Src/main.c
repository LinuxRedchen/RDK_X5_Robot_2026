/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ESP8266_ONENET_INFO		"AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
int fputc(int ch, FILE *f)
{
	uint8_t temp = ch;
	HAL_UART_Transmit(&huart2, &temp, 1, 10);
	return ch;
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t Gray1 = 0;
uint8_t Gray2 = 0;
uint8_t Gray3 = 0;
uint8_t Gray4 = 0;
uint8_t Gray5 = 0;
uint8_t Gray6 = 0;
uint8_t Gray7 = 0;
uint8_t Gray8 = 0;
uint8_t Room_LNum = 0 ,Room_RNum = 0;
uint8_t Room_LastNum = 0;
uint8_t Room_Flag = 0;
uint8_t Goal_Room = 0;
uint8_t Actual_Room = 0;
uint8_t Room_Direction = 0;
volatile uint32_t currentMiliSeconds = 0;
uint8_t ReceiveDate[10]={0},Receive_Cnt = 0;
char rx_str[7];
uint8_t Rdata = 0;
uint8_t Cabinet_Flag = 0;
uint8_t temp=0;//温度
uint8_t humi=0;//湿度
uint8_t Step1=0,Step2=0;
uint8_t HotCode = 0;
uint8_t Fanva = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void OnenetDataUpload(void);
void SensorExecuteProc(void);
void SensorProc(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	unsigned char *dataPtr = NULL;
//	unsigned char  
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	ESP8266_Init();

	ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT");
	delay_ms(1500);
	ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT");
	delay_ms(1500);
	printf("Connect MQTT Server Success\r\n"); 
	Robot_PWMSetStart();
	Robot_Motion_PWM(45);
	DHT11_GPIO_Init();
    HAL_Delay(1000); // 模块上电稳定时间

	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
 {//高电平，继电器吸合

	 
	 SensorExecuteProc();
	HAL_UART_Receive_IT(&huart1,ReceiveDate,6);
	 OnenetDataUpload();
	Screen_Judge_Proc();
	SensorProc();
		delay_ms(3000);
		Goods_Room_Delivery_Proc();
	 SensorProc();
		Lock_Control();
		OnenetDataUpload();
	 Back_Delivery_Proc();


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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void _ttywrch(int ch)
{
    // 空实现，什么也不做
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
		memset(ReceiveDate,0,10);
		HAL_UART_Transmit(&huart1, ReceiveDate, 6, 200);
		HAL_UART_Receive_IT(&huart1, ReceiveDate, 6);
		
}

void SensorExecuteProc(void)
{
	if(temp>75)
	{
		HotCode=1;//冷
	}
	if(temp<75&&temp>45)
	{
		HotCode=0;//热
	}
	if(temp<10)
	{
		HotCode=1;//冷
	}
	if(humi>50)
	{
		Fanva=1;//开
	}
	else
	{
		Fanva=0;//关
	}
}

void SensorProc(void)
{
	if(HotCode==1)
	{
		HAL_GPIO_WritePin(L298N_IN1_GPIO_Port,L298N_IN1_Pin,HIGH);
		HAL_GPIO_WritePin(L298N_IN2_GPIO_Port,L298N_IN2_Pin,LOW);
		HAL_GPIO_WritePin(L298N_IN3_GPIO_Port,L298N_IN3_Pin,LOW);
		HAL_GPIO_WritePin(L298N_IN4_GPIO_Port,L298N_IN4_Pin,LOW);
	}
	else
	{
		HAL_GPIO_WritePin(L298N_IN1_GPIO_Port,L298N_IN1_Pin,LOW);
		HAL_GPIO_WritePin(L298N_IN2_GPIO_Port,L298N_IN2_Pin,LOW);
		HAL_GPIO_WritePin(L298N_IN3_GPIO_Port,L298N_IN3_Pin,HIGH);
		HAL_GPIO_WritePin(L298N_IN4_GPIO_Port,L298N_IN4_Pin,LOW);
	}
	if(Fanva==1)
	{
		HAL_GPIO_WritePin(Fan_GPIO_Port,Fan_Pin,HIGH);
	}
	else
	{
		HAL_GPIO_WritePin(Fan_GPIO_Port,Fan_Pin,LOW);
	}
	if(Cabinet_Flag==1)
	{
		HAL_GPIO_WritePin(Control_Lock_GPIO_Port,Control_Lock_Pin,HIGH);
	}
	else
	{
		HAL_GPIO_WritePin(Control_Lock_GPIO_Port,Control_Lock_Pin,LOW);
	}
}

void OnenetDataUpload(void)
{
	unsigned char fan_status;    // 风扇开启
	int HotColdStatusValue;             // 加热
	unsigned char WaimaiStatusValue;  // 外卖
	unsigned char temp_value;   // 温度25℃
	unsigned char humi_value;   // 湿度60%
	
	temp_value=temp;
	humi_value=humi;
//		temp_value=35;
//	humi_value=60;

	WaimaiStatusValue=Cabinet_Flag;
	HotColdStatusValue=HotCode;
	fan_status=Fanva;
	
	Onenet_Date_trans(fan_status,HotColdStatusValue,WaimaiStatusValue,temp_value,humi_value);
}
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
