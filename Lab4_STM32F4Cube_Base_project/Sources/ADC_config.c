/**
  ******************************************************************************
	* @file    ADC_config.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    February-2016
  * @brief   Configure adc for temperature monitoring
  ******************************************************************************
  */
	
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "ADC_config.h"

/* Definition of ADC handle struct */
ADC_HandleTypeDef ADC1_Handle;


/**
   * @brief A function used to setup the ADC to sample Channel 16
	 * @param None
   * @retval None
   */
void ADC1_Config(void){
	ADC_InitTypeDef ADC1_Init; 																	/* definition of ADC1 initialiation struct */
	ADC_ChannelConfTypeDef ADC1_Channel;												/* Definition of ADC1 channel struct */
	HAL_LockTypeDef ADC1_Lock; 																	/* define ADC1 locking object */
	HAL_ADC_StateTypeDef ADC1_State; 														/* define ADC1 state struct */
	ADC_MultiModeTypeDef ADC1_Mode; 														/* define ADC1 mode struct */
	__HAL_RCC_ADC1_CLK_ENABLE(); 																/* Configure the ADC clock */
	
	/*  initialize ADC1 init struct */
	ADC1_Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;				/* ADC Clock frequency 42MHz (168/4) */
	ADC1_Init.Resolution = ADC_RESOLUTION_12B;									/* 12 bit resolution, better but slower */
	ADC1_Init.DataAlign = ADC_DATAALIGN_RIGHT;									/* Align the 12 bits data at the right of the 32 bits words */
	ADC1_Init.ScanConvMode = DISABLE;														/* single channel mode */
	ADC1_Init.EOCSelection = ADC_EOC_SEQ_CONV;									/* perform ADC conversions without having to read all conversion data */
	ADC1_Init.ContinuousConvMode = DISABLE;											/* single mode convertion */
	ADC1_Init.DMAContinuousRequests = DISABLE;									/* single mode DMA request */
	ADC1_Init.NbrOfConversion = 1;															/* one conversion */
	ADC1_Init.DiscontinuousConvMode = ENABLE;										/* enable discountinuous mode */
	ADC1_Init.NbrOfDiscConversion = 1;													/* one conversion */
	ADC1_Init.ExternalTrigConv = ADC_SOFTWARE_START;						/* no external trigger */
	ADC1_Init.ExternalTrigConvEdge = ADC_SOFTWARE_START;				/* no external trigger */
	
	/* initialize ADC1 handle struct */
	ADC1_Handle.Instance = ADC1;																/* using ADC1 */
	ADC1_Handle.Init = ADC1_Init;																/* initialisation structure */
	ADC1_Handle.NbrOfCurrentConversionRank = 1;
	ADC1_Handle.Lock = ADC1_Lock;
	ADC1_Handle.State = ADC1_State;
	ADC1_Handle.ErrorCode = HAL_ADC_ERROR_NONE;									/* no error */
	
	/* initialize ADC1 channel struct */
	ADC1_Channel.Channel = ADC_CHANNEL_16;
	ADC1_Channel.Rank = 1;																			/* use to determine the rank in which this channel is sampled */
	ADC1_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;				/* time for the internal capacitor to charge. longuer means more accurate */
	ADC1_Channel.Offset = 0;																		/* reserved for futur use */
	
	/* initialize ADC1 mode struct */
	ADC1_Mode.Mode = ADC_MODE_INDEPENDENT;														
	ADC1_Mode.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
	ADC1_Mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	
	
	HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC1_Channel);					/* configure channel */
	HAL_ADCEx_MultiModeConfigChannel(&ADC1_Handle, &ADC1_Mode);	/* configure mode */
	HAL_ADC_Start(&ADC1_Handle);																/* start polling mode of ADC1 */
}
