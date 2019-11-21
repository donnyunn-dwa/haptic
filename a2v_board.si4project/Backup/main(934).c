/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dw7914.h"
#include "a2v_controller.h"
#include "uart_packet.h"
#include "util.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
	STAGE_INIT = 0,
	STAGE_FFT,
	STAGE_HAPTIC,
	STAGE_TUNING,
	STAGE_IDLE,
	STAGE_MAX
} MAIN_STAGE;
MAIN_STAGE stage;

extern DW7914_RegisterTypeDef hDW7914;
extern parameterA2V a2v;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_BUFF_SIZE 0x1000

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
bool adcBusy = false;
bool fftFlag = false;
bool uartFlag = false;

__IO uint32_t adcValue;
uint16_t adcBuffer[ADC_BUFF_SIZE];
uint32_t ptr_push_cntBuffer = 0;
uint32_t ptr_pull_cntBuffer = 0;

uint16_t adcDoubleBuffer[NFFT];
uint16_t cnt_db = 0; // counter of double buffer

short	adcFFTin[NFFT];
static uint8_t tuning_factor[64];

uint8_t rxBuffer[UART_BUFF_SIZE];

extern uint8_t runFlag;

uint32_t cntLed = 0;
uint32_t cntSerial = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void ADC_pushBuffer(uint16_t value);
uint16_t ADC_pullBuffer(void);
bool ADC_isBufferEmpty(void);

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
  DW7914_WAVEFORM_ID waveId = WAVEFORM_ID_FREQMID;
  int vdClamp;

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
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	
	// led on
	HAL_GPIO_WritePin(MCU_ACT_GPIO_Port, MCU_ACT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	
	// mcu init
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_UART_Receive_DMA(&huart2, rxBuffer, UART_BUFF_SIZE);

	tuning_factor[0] = 75;				// vd_clamp (*40mV)
	tuning_factor[1] = 0;				// auto brake on-off
	tuning_factor[2] = 1;				// in_preFFTGain

	tuning_factor[3] = 0;				// LR Gap Cutting (for Android)
	tuning_factor[4] = 128;			// in_loudnessThres
	tuning_factor[5] = 5;				// cnt_freq_low (*93.75Hz)
	tuning_factor[6] = 18;			// cnt_freq_mid (*93.75Hz)
	tuning_factor[7] = 69;			// cnt_freq_high (*93.75Hz)

	tuning_factor[8] = 128;			//200
	tuning_factor[9] = 128;			//400
	tuning_factor[10] = 128;			//800
	tuning_factor[11] = 128;			//1600
	tuning_factor[12] = 128;			//3200
	tuning_factor[13] = 128;			//6400
	
	// led off
	HAL_Delay(100);
	HAL_GPIO_WritePin(MCU_ACT_GPIO_Port, MCU_ACT_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    	switch(stage) {
    		case STAGE_INIT:

    			if(!util_flash_isEmpty()) {
    				util_flash_read(tuning_factor, 14);
    			} else {
    				util_flash_write(tuning_factor, 14);
    			}

    			dw7914_init(&hDW7914);

    			dw7914_go();
    			HAL_Delay(10);

    			DW_A2V_Init(&a2v);

    			stage = STAGE_IDLE;
    			break;
    		
    		case STAGE_FFT:

    			DW_A2V_Ctrl(&a2v, adcFFTin, tuning_factor);

    			tuning_factor[14] = (uint8_t)(a2v.loudnessOut_R >> 8);
    			tuning_factor[15] = (uint8_t)(a2v.loudnessOut_R & 0xff);
    			tuning_factor[16] = (uint8_t)(a2v.loudnessOut_Base >> 8);
    			tuning_factor[17] = (uint8_t)(a2v.loudnessOut_Base & 0xff);
    			tuning_factor[18] = (uint8_t)((a2v.loudnessOut_R - a2v.loudnessOut_Base) >> 8);
    			tuning_factor[19] = (uint8_t)((a2v.loudnessOut_R - a2v.loudnessOut_Base) & 0xff);
    			
    			stage = STAGE_IDLE;
    			break;
    		
    		case STAGE_HAPTIC:

    			if(a2v.out_loudnessLow > a2v.out_loudnessMid) {
    				if(a2v.out_loudnessLow > a2v.out_loudnessHigh) {
    					waveId = WAVEFORM_ID_FREQLOW;
    					vdClamp = a2v.in_vdClamp*2;
    				} else {
    					waveId = WAVEFORM_ID_FREQHIGH;
    					vdClamp = a2v.in_vdClamp*2;
    				}
    			} else {
    				if(a2v.out_loudnessMid > a2v.out_loudnessHigh) {
    					waveId = WAVEFORM_ID_FREQMID;
    					vdClamp = a2v.in_vdClamp;
    				} else {
    					waveId = WAVEFORM_ID_FREQHIGH;
    					vdClamp = a2v.in_vdClamp*2;
    				}
    			}
    			
				dw7914_set_wave_seq(waveId);
				dw7914_set_vdClamp(waveId, vdClamp);
				dw7914_go();

    			stage = STAGE_IDLE;
    			break;

    		case STAGE_TUNING:

    			uart_packet_Decode((char*)rxBuffer, tuning_factor);

				stage = STAGE_IDLE;
    			break;
    		
    		case STAGE_IDLE:

    			if(uartFlag) {
    				uartFlag = false;
    				stage = STAGE_TUNING;
    				
    			} else if(fftFlag) {
    				fftFlag = false;
    				stage = STAGE_FFT;
    				
    			} else if(runFlag) {				
					if(!(cntLed&0x10)) {
						HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
					}
					stage = STAGE_HAPTIC;
					
				}
				
    			break;
    	}

    	// adc buffering always
    	if(!ADC_isBufferEmpty()) {
    		adcDoubleBuffer[cnt_db++] = ADC_pullBuffer();
    		if(cnt_db >= NFFT) {
    			for(int i = 0; i < NFFT; i++) {
    				adcFFTin[i] = (int)adcDoubleBuffer[i] - 2048;
    			}
    			for(int i = 0; i < NOVLP; i++) {
    				adcDoubleBuffer[i] = adcDoubleBuffer[i+NFFT-NOVLP];
    			}
    			cnt_db = NOVLP;
    			fftFlag = true;
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
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
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 400000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1250-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 300-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 20000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 921600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED1_Pin|MCU_ACT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EN2_Pin|EN1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA15 PA10 PA8 PA9 
                           PA7 PA6 PA5 PA4 
                           PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_8|GPIO_PIN_9 
                          |GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 PB15 PB12 
                           PB14 PB13 PB2 PB1 
                           PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_15|GPIO_PIN_12 
                          |GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_2|GPIO_PIN_1 
                          |GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin MCU_ACT_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|MCU_ACT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : EN2_Pin EN1_Pin */
  GPIO_InitStruct.Pin = EN2_Pin|EN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PC15 PC14 PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_14|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PH1 PH0 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void ADC_pushBuffer(uint16_t value)
{
	adcBuffer[ptr_push_cntBuffer++] = value;
	if(ptr_push_cntBuffer >= ADC_BUFF_SIZE) {
		ptr_push_cntBuffer = 0;
	}
}

uint16_t ADC_pullBuffer(void)
{
	uint16_t ret;

	ret = adcBuffer[ptr_pull_cntBuffer++];
	if(ptr_pull_cntBuffer >= ADC_BUFF_SIZE) {
		ptr_pull_cntBuffer = 0;
	}

	return ret;
}

bool ADC_isBufferEmpty(void)
{
	if(ptr_push_cntBuffer != ptr_pull_cntBuffer) {
		return false;
	} else {
		return true;
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	ADC_pushBuffer((uint16_t)adcValue);
	adcBusy = false;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
	if(htim->Instance == TIM2) { // 24kHz Sampling
		if(!adcBusy) {
			HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&adcValue, 1);
			adcBusy = true;
		}

		// led alive
		if(!(++cntLed%0x8000)) {
			HAL_GPIO_WritePin(MCU_ACT_GPIO_Port, MCU_ACT_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_TIM_Base_Start_IT(&htim3);
		}
	}
	if(htim->Instance == TIM3) {
		HAL_GPIO_WritePin(MCU_ACT_GPIO_Port, MCU_ACT_Pin, GPIO_PIN_SET);
		HAL_TIM_Base_Stop_IT(&htim3);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
	uartFlag = true;
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
	HAL_GPIO_WritePin(MCU_ACT_GPIO_Port, MCU_ACT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	while(1) HAL_Delay(1);

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
