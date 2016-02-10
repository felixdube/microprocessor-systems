/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : A program which showcases ADC and TIM3 under the new firmware
	                       The ADC
	* Author						 : Ashraf Suyyagh
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
	
/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>

#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "kalmanfilter.h"
#include "temperature.h"
#include "main.h"

/* Private variables ---------------------------------------------------------*/
float adc_val = 0;
float temp = 0;
kalmanState *adcState; 

/* Global variables ----------------------------------------------------------*/
volatile int sysTick = 0;

/* Definition of ADC handle struct */
ADC_HandleTypeDef ADC1_Handle;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);
void ADC1_Config(void);
void Display_GPIO_Config(void);

int main(void)
{
  /* MCU Configuration----------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
  /* Configure the system clock */
  SystemClock_Config();
	
	/* configure ADC1 */
	ADC1_Config();
	
	/* Init Kalman Filter */
	adcState = malloc(sizeof(kalmanState));
	kalmanInit(adcState, 0.1, 0.1, 1024, 0.1, 0);
	
	while (1){
		if (sysTick) {
			HAL_ADC_Start(&ADC1_Handle);
			if (HAL_ADC_PollForConversion(&ADC1_Handle, 1000000) == HAL_OK) {
				adc_val = HAL_ADC_GetValue(&ADC1_Handle);
				printf("\nadc: %f", adc_val);
				kalmanUpdate(adcState, adc_val);
				printf("\nkstate: %f", adcState->x);
				temp = convertTemp(adcState->x);
				printf("\ntemp: %f", temp);
				
				/* reset sysTick flag */
				sysTick = 0;
			}
		}
	}
}



/** System Clock Configuration */
void SystemClock_Config(void){

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 			 	= RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 				= 8;
  RCC_OscInitStruct.PLL.PLLN 				= 336;
  RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 				= 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5)!= HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};
	
	/*Configures SysTick to provide 1ms interval interrupts.*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/100);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
   * @brief A function used to setup the ADC to sample Channel 16
   * @retval None
   */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line){
}
#endif


/* GPIO configuration */
void Display_GPIO_Config(void){
	
	/* definition of GPIO pins */
	GPIO_InitTypeDef segA;
	GPIO_InitTypeDef segB;
	GPIO_InitTypeDef segC;
	GPIO_InitTypeDef segD;
	GPIO_InitTypeDef segE;
	GPIO_InitTypeDef segF;
	GPIO_InitTypeDef segG;
	GPIO_InitTypeDef segDP;
	GPIO_InitTypeDef sel1;
	GPIO_InitTypeDef sel2;
	GPIO_InitTypeDef sel3;
	GPIO_InitTypeDef sel4;
	
	/* Configure the GPIO clock */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* initialisation of segA */
	segA.Pin = 
	segA.Mode = GPIO_MODE_OUTPUT_PP;												// push-pull
	segA.Pull = GPIO_NOPULL;
	segA.Speed = GPIO_SPEED_FREQ_MEDIUM;										// max frequency for our processor is 84MHz
	segA.Alternate = 
	
	/* initialisation of segB */
	segB.Pin = 
	segB.Mode = GPIO_MODE_OUTPUT_PP;	
	segB.Pull = GPIO_NOPULL;
	segB.Speed = GPIO_SPEED_FREQ_MEDIUM;
	segB.Alternate = 

	/* initialisation of segC */
	segC.Pin = 
	segC.Mode = GPIO_MODE_OUTPUT_PP;	
	segC.Pull = GPIO_NOPULL;
	segC.Speed = GPIO_SPEED_FREQ_MEDIUM;
	segC.Alternate = 

	/* initialisation of segD */
	segD.Pin = 
	segD.Mode = GPIO_MODE_OUTPUT_PP;	
	segD.Pull = GPIO_NOPULL;
	segD.Speed = GPIO_SPEED_FREQ_MEDIUM;
	segD.Alternate = 

	/* initialisation of segE */
	segE.Pin = 
	segE.Mode = GPIO_MODE_OUTPUT_PP;	
	segE.Pull = GPIO_NOPULL;
	segE.Speed = GPIO_SPEED_FREQ_MEDIUM;
	segE.Alternate = 

	/* initialisation of segF */
	segF.Pin = 
	segF.Mode = GPIO_MODE_OUTPUT_PP;	
	segF.Pull = GPIO_NOPULL;
	segF.Speed = GPIO_SPEED_FREQ_MEDIUM;
	segF.Alternate = 

	/* initialisation of segG */
	segG.Pin = 
	segG.Mode = GPIO_MODE_OUTPUT_PP;	
	segG.Pull = GPIO_NOPULL;
	segG.Speed = GPIO_SPEED_FREQ_MEDIUM;
	segG.Alternate = 

	/* initialisation of segDP */
	segDP.Pin = 
	segDP.Mode = GPIO_MODE_OUTPUT_PP;	
	segDP.Pull = GPIO_NOPULL;
	segDP.Speed = GPIO_SPEED_FREQ_MEDIUM;
	segDP.Alternate = 

	/* initialisation of sel1 */
	sel1.Pin = 
	sel1.Mode = GPIO_MODE_OUTPUT_PP;	
	sel1.Pull = GPIO_NOPULL;
	sel1.Speed = GPIO_SPEED_FREQ_MEDIUM;
	sel1.Alternate = 

	/* initialisation of sel2 */
	sel2.Pin = 
	sel2.Mode = GPIO_MODE_OUTPUT_PP;	
	sel2.Pull = GPIO_NOPULL;
	sel2.Speed = GPIO_SPEED_FREQ_MEDIUM;
	sel2.Alternate = 

	/* initialisation of sel3 */
	sel3.Pin = 
	sel3.Mode = GPIO_MODE_OUTPUT_PP;	
	sel3.Pull = GPIO_NOPULL;
	sel3.Speed = GPIO_SPEED_FREQ_MEDIUM;
	sel3.Alternate = 

	/* initialisation of sel4 */
	sel4.Pin = 
	sel4.Mode = GPIO_MODE_OUTPUT_PP;	
	sel4.Pull = GPIO_NOPULL;
	sel4.Speed = GPIO_SPEED_FREQ_MEDIUM;
	//sel4.Alternate = 

	HAL_GPIO_Init(GPIOB, &segA);
	HAL_GPIO_Init(GPIOB, &segB);
	HAL_GPIO_Init(GPIOB, &segC);
	HAL_GPIO_Init(GPIOB, &segD);
	HAL_GPIO_Init(GPIOB, &segE);
	HAL_GPIO_Init(GPIOB, &segF);
	HAL_GPIO_Init(GPIOB, &segG);
	HAL_GPIO_Init(GPIOB, &segDP);
	HAL_GPIO_Init(GPIOB, &sel1);
	HAL_GPIO_Init(GPIOB, &sel2);
	HAL_GPIO_Init(GPIOB, &sel3);
	HAL_GPIO_Init(GPIOB, &sel4);
	
}

/* ADC1 configuration */
void ADC1_Config(void){
	
	
	/* definition of ADC1 initialiation struct */
	ADC_InitTypeDef ADC1_Init;
	
	/* Definition of ADC1 channel struct */
	ADC_ChannelConfTypeDef ADC1_Channel;
	
	/* define ADC1 locking object */
	HAL_LockTypeDef ADC1_Lock;
	
	/* define ADC1 state struct */
	HAL_ADC_StateTypeDef ADC1_State;
	
	/* define ADC1 mode struct */
	ADC_MultiModeTypeDef ADC1_Mode;
	
	
	/* Configure the ADC clock */
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	/*  initialize ADC1 init struct */
	ADC1_Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;							// ADC Clock frequency 28MHz (168/6)
	ADC1_Init.Resolution = ADC_RESOLUTION_12B;												// 12 bit resolution, better but slower
	ADC1_Init.DataAlign = ADC_DATAALIGN_RIGHT;												// Align the 12 bits data at the right of the 32 bits words
	ADC1_Init.ScanConvMode = DISABLE;																	// single channel mode
	ADC1_Init.EOCSelection = ADC_EOC_SEQ_CONV;												// perform ADC conversions without having to read all conversion data
	ADC1_Init.ContinuousConvMode = DISABLE;														// single mode convertion
	ADC1_Init.DMAContinuousRequests = DISABLE;												// single mode DMA request
	ADC1_Init.NbrOfConversion = 1;																		// one conversion
	ADC1_Init.DiscontinuousConvMode = ENABLE;													// enable discountinuous mode
	ADC1_Init.NbrOfDiscConversion = 1;																// one conversion
	ADC1_Init.ExternalTrigConv = ADC_SOFTWARE_START;									// no external trigger
	ADC1_Init.ExternalTrigConvEdge = ADC_SOFTWARE_START;							// no external trigger
	
	/* initialize ADC1 handle struct */
	ADC1_Handle.Instance = ADC1;																			// using ADC1
	ADC1_Handle.Init = ADC1_Init;																			// initialisation structure
	ADC1_Handle.NbrOfCurrentConversionRank = 1;
	//ADC1_Handle.DMA_Handle = 																				// not needed since we are not using dma
	ADC1_Handle.Lock = ADC1_Lock;
	ADC1_Handle.State = ADC1_State;
	ADC1_Handle.ErrorCode = HAL_ADC_ERROR_NONE;												// no error
	
	/* initialize ADC1 channel struct */
	ADC1_Channel.Channel = ADC_CHANNEL_16;
	ADC1_Channel.Rank = 1;																						// use to determine the rank in which this channel is sampled
	ADC1_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;							// time for the internal capacitor to charge. longuer means more accurate
	ADC1_Channel.Offset = 0;																					// reserved for futur use
	
	/* initialize ADC1 mode struct */
	ADC1_Mode.Mode = ADC_MODE_INDEPENDENT;														
	ADC1_Mode.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
	ADC1_Mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	
	/* configure channel */
	HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC1_Channel);
	
	/* configure mode */
	HAL_ADCEx_MultiModeConfigChannel(&ADC1_Handle, &ADC1_Mode);
	
	/* start polling mode of ADC1 */
	HAL_ADC_Start(&ADC1_Handle);
}
