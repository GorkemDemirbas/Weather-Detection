/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t adcval;
float temp;
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
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  // Lcd_PortType ports[] = { D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port };
	Lcd_PortType ports[] = { GPIOC, GPIOB, GPIOA, GPIOA };
	// Lcd_PinType pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};
	Lcd_PinType pins[] = {GPIO_PIN_7, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_6};
	Lcd_HandleTypeDef lcd;
	// Lcd_create(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE);
	lcd = Lcd_create(ports, pins, GPIOB, GPIO_PIN_5, GPIOB, GPIO_PIN_4, LCD_4_BIT_MODE);
	/*Lcd_cursor(&lcd, 0,1);
	Lcd_string(&lcd, "Hava Durumu:");
	for ( int x = 1; x <= 200 ; x++ )
	{
	Lcd_cursor(&lcd, 1,7);
	Lcd_int(&lcd, x);
	HAL_Delay (1000);
	}*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_ADC_Start(&hadc1);
	  if(HAL_ADC_PollForConversion(&hadc1, 5) == HAL_OK)
	  {
		  /*ADC INITIALIZATION*/
		  adcval = HAL_ADC_GetValue(&hadc1);
		  temp = (adcval * 330.0) / 1023.0;

		  /*ALGORITMA KISMI*/
		  if((temp >= 28) & (temp < 30))
		  {
			  /*Hava Sıcaktır, buzzer uyarı versin ve LCD ekranında hava durumu sıcak yazsın !!!*/

			  /*Normal durumu geçtiği için yanan yeşil ledi söndür*/
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);

			  /*Çok sıcak durumu geçtiği için yanan kırmızı ledi söndür*/
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);

			  /*BUZZER KISMI*/
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
			  HAL_Delay(1000);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
			  HAL_Delay(1000);

			  /*LCD YAZISI*/
			  /*Başlangıçta ekranı bir temizle*/
			  Lcd_clear(&lcd);
			  Lcd_cursor(&lcd, 0,3);
			  Lcd_int(&lcd, temp);
			  Lcd_cursor(&lcd, 0,6);
			  Lcd_string(&lcd, "Derece");
			  Lcd_cursor(&lcd, 1,5);
			  Lcd_string(&lcd, "Sicak");

			  //HAL_Delay (1000);
		  }
		  else if(temp >= 30)
		  {
			  /*BUZZER KISMI*/
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
			  HAL_Delay(250);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
			  HAL_Delay(250);

			  /*LCD YAZISI*/
			  /*Başlangıçta ekranı bir temizle*/
			  Lcd_clear(&lcd);
			  Lcd_cursor(&lcd, 0,3);
			  Lcd_int(&lcd, temp);
			  Lcd_cursor(&lcd, 0,6);
			  Lcd_string(&lcd, "Derece");
			  Lcd_cursor(&lcd, 1,4);
			  Lcd_string(&lcd, "Cok Sicak");

			  /*ÇOK SICAK DURUMU Board üzerinde Kırmızı LED yak!!!*/
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);



		  }
		  else if((temp < 20) & (temp >= 15))
		  {
			  /*Hava Soğuktur, buzzer uyarı versin ve LCD ekranında hava durumu soğuk yazsın !!!*/
			  /*BUZZER KISMI*/
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
			  HAL_Delay(1000);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
			  HAL_Delay(1000);

			  /*LCD YAZISI (Hava Durumu SOGUK)*/
			  /*Başlangıçta ekranı bir temizle*/
			  Lcd_clear(&lcd);
			  Lcd_cursor(&lcd, 0,3);
			  Lcd_int(&lcd, temp);
			  Lcd_cursor(&lcd, 0,6);
			  Lcd_string(&lcd, "Derece");
			  Lcd_cursor(&lcd, 1,5);
			  Lcd_string(&lcd, "Soguk");

		  }
		  else if(temp < 15)
		  {
			  /*(HAVA ÇOK SOGUK durumu)*/
			  /*Normal durumu geçtiği için yanan yeşil ledi söndür*/
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);

			  /*BUZZER KISMI*/
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
			  HAL_Delay(250);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
			  HAL_Delay(250);

			  /*LCD YAZISI*/
			  /*Başlangıçta ekranı bir temizle*/
			  Lcd_clear(&lcd);
			  Lcd_cursor(&lcd, 0,3);
			  Lcd_int(&lcd, temp);
			  Lcd_cursor(&lcd, 0,6);
			  Lcd_string(&lcd, "Derece");
			  Lcd_cursor(&lcd, 1,4);
			  Lcd_string(&lcd, "Cok Soguk");

		  }
		  else
		  {
			  /*Hava durumu normaldir (20-27 derece arası), buzzer çalışmasın !, LCD ekranında hava durumu normal yazsın !!!*/

			  /*LCD YAZISI (Hava Durumu NORMAL)*/
			  Lcd_cursor(&lcd, 0,3);
			  Lcd_int(&lcd, temp);
			  Lcd_cursor(&lcd, 0,6);
			  Lcd_string(&lcd, "Derece");
			  Lcd_cursor(&lcd, 1,5);
			  Lcd_string(&lcd, "Normal");

			  /*Normal durumunu belirten YEŞİL Led yansın !*/
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);

		  }
	  }
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_10B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6|GPIO_PIN_7|LedA11_Pin|Buzzer_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Led_Pin|GPIO_PIN_7|LedC8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA6 PA7 LedA11_Pin Buzzer_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|LedA11_Pin|Buzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Led_Pin PC7 LedC8_Pin */
  GPIO_InitStruct.Pin = Led_Pin|GPIO_PIN_7|LedC8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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

#ifdef  USE_FULL_ASSERT
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
