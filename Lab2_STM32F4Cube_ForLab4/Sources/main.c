/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : A program that read the value of the temperature of the processor using and ADC
  *											 and prints the value on the 7 segment display and a LCD
	* Author						 : Ashraf Suyyagh, Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
	
/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "kalmanfilter.h"
#include "temperature.h"
#include "segment_controller.h"
#include "alarm.h"
#include "main.h"
#include "lcd.h"
#include "system_clock.h"
#include "ADC_config.h"

/* Private variables ---------------------------------------------------------*/
float adc_val = 0;						/* Raw ADC value */
float temp = 0;               /* Store the converted ADC value in temperature */
float displayTemp = 0;				/* Same temperature but updated less often for display */
kalmanState *adcState; 				
int filterAlarmCounter = 0;		/* alarm variable to prevent false alarm */

/* Initialize struct */
TIM_Base_InitTypeDef TIM_InitDef;
TIM_HandleTypeDef TIM_HandleDef;


/* Global variables ----------------------------------------------------------*/
volatile int adcTimer = 0;
volatile int updateMeasureForDisplayTimer = 500000;
volatile int display7segTimer = 0;


/* character string to be displayed on the LCD is put in this variable */
char tempToLCD[17];						/*17 to store 16 char plus /0 */


int main(void)
{
  /* MCU Configuration----------------------------------------------------------*/

  HAL_Init();	  													/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  SystemClock_Config(); 									/* Configure the system clock */
	ADC1_Config(); 													/* configure ADC1 */
	Display_GPIO_Config(); 									/* Configure 7-Segment Displays */
	Alarm_GPIO_Config(); 										/* Configure alarm pins */	
	initLCD(); 															/* configure LCD */

	/* print temperature on the first line of the LCD */
	returnHome(); 													/* just makes sure that start writing at the right place */
	LCD_WriteString("  Temperature"); 			/* The 2 initial space are for centering */
	

	
	adcState = malloc(sizeof(kalmanState)); /* Init Kalman Filter */
	kalmanInit(adcState, INIT_q, INIT_r, INIT_x, INIT_p, INIT_k);

	/* main program to run in this infinite loop */
	while (1)	{

		
		if (adcTimer >= ADC_PERIOD) { 								/* 100Hz */
			adcTimer = 0;
			HAL_ADC_Start(&ADC1_Handle); 								/* start ADC conversion */
			
			/* wait for the conversion to be done and get data */
			if (HAL_ADC_PollForConversion(&ADC1_Handle, 1000000) == HAL_OK) { 
				adc_val = HAL_ADC_GetValue(&ADC1_Handle); /* get the value */
				
				kalmanUpdate(adcState, adc_val); 					/* filter the data and update the filter parameters */
				/* DON'T DELETE printf for matlab script */
				//printf("%f,%f,%f,%f,%f,%f\n",adc_val, adcState->q,adcState->r, adcState->x, adcState->p, adcState->k);
				
				temp = convertTemp(adcState->x); 					/* convert the filterd value of the ADC into temperature */
				
				/* Alarm triggering  */
				if (temp > THRESHHOLD_TEMP) {
					if ( filterAlarmCounter > 5 ){ 					/* 5 consecutive to avoid false positive */
						flash_segment();
					}
					else {
					filterAlarmCounter++;
					}
				} 
				else {
					__HAL_RCC_GPIOB_CLK_ENABLE();
					filterAlarmCounter = 0;
				}
		
				/* Update Measurement to Display at 2Hz */
				if (updateMeasureForDisplayTimer >= UPDATE_MEASURE_PERIOD) {
					updateMeasureForDisplayTimer = 0; 						/* reset the displayTimer tick */
					displayTemp = temp;
					displayTemp = floor(10 * displayTemp) / 10; 	/* truncate to 1 decimal without rounding */
								
					/* LCD DISPLAY */	
					SetAdress(64); 																/* go to line 2 of LCD */
					sprintf(tempToLCD, "     %.1f", displayTemp); /* convert the float to a string and formats it */
					LCD_WriteString(tempToLCD);										/* print value to the LCD display */
					/* LCD DISPLAY END */
				}
			}
		}
		
		/* here display runs at DISPLAY_7_SEGMENT_PERIOD speed, but displayTemp gets updated at 2Hz */
		if(display7segTimer >= DISPLAY_7_SEGMENT_PERIOD) {
			display7segTimer = 0;
			display(displayTemp); 														/* display on 7-segment display */
		}
	
	}
}


/**
	* @brief Display timer configuration
	* @param None
	* @retval None
	*/
void TIM_Config(void) {
	/* Enable clock for TIM4 */
	__HAL_RCC_TIM4_CLK_ENABLE();
	/* The desired frequency = 84MHz / (Prescaler*Period)*/
	TIM_InitDef.Prescaler = 84;
	TIM_InitDef.Period = 100;
	TIM_InitDef.CounterMode = TIM_COUNTERMODE_DOWN;

	TIM_HandleDef.Instance = TIM4;
	TIM_HandleDef.Init = TIM_InitDef;

	HAL_TIM_Base_Start_IT(&TIM_HandleDef);

	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 2);
}

/**
  * @brief  Callback from the Timers
	* @param  Timer handler
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
		adcTimer++;
		updateMeasureForDisplayTimer++;
		display7segTimer++;
		timeDisplay1DigitTimer++;
		alarmLedTimer++;
		lcdtimer++;
		HAL_IncTick();
}
