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
float adc_val = 0;
float temp = 0;
float displayTemp = 0;
kalmanState *adcState; 
//int cycles_since_start = 0;
int display_slower = 0;
int filterAlarmCounter = 0;

/* Global variables ----------------------------------------------------------*/
volatile int adcTick = 0;
volatile int displayTimer = 50;
char tempToLCD[20];
char tempToLCD2[5];


int main(void)
{
  /* MCU Configuration----------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
  /* Configure the system clock */
  SystemClock_Config();
	
	/* configure ADC1 */
	ADC1_Config();
	
	/* configure LCD */
	initLCD();
	
	/* Configure 7-Segment Displays */
	Display_GPIO_Config();
	
	/* Configure alarm pins */
	Alarm_GPIO_Config();
	
	/* Init Kalman Filter */
	adcState = malloc(sizeof(kalmanState));
	kalmanInit(adcState, INIT_q, INIT_r, INIT_x, INIT_p, INIT_k);

	
	while (1){
		if (adcTick) {
			
			// start ADC conversion
			HAL_ADC_Start(&ADC1_Handle);
			
			// wait for the conversion to be done and get data
			if (HAL_ADC_PollForConversion(&ADC1_Handle, 1000000) == HAL_OK) { 
				adc_val = HAL_ADC_GetValue(&ADC1_Handle); 
				
				// filter the data
				kalmanUpdate(adcState, adc_val); 
				
				// convert data to celsius
				temp = convertTemp(adcState->x); 
				
				// update the value to be printed at 2Hz
				if (displayTimer >= 50) {
					
					// update the value to be displayed
					displayTemp = temp;
					
					// reset the displayTimer tick
					displayTimer = 0;	
					
					//truncate to 1 decimal without rounding
					displayTemp = floor(10 * displayTemp) / 10;
					//Write Temperature on the first line. The 2 initial space are for centering
					
					returnHome();
					LCD_WriteString("  Temperature");
					// print value to the LCD display
					//go to line 2
					SetAdress(64);
					sprintf(tempToLCD, "     %.1f", displayTemp);
					LCD_WriteString(tempToLCD);
				}
				
				// reset the adcTick
				adcTick = 0;
			}
		}
		
		//Alarm triggering
		if (displayTemp > THRESHHOLD_TEMP) {
			
			if ( filterAlarmCounter > 5 ){
				trigger_alarm();
			}
			else {
				filterAlarmCounter++;
			}
		} else {
			shutoff_alarm();
			filterAlarmCounter = 0;
		}
		
		// change the digit to be viewed for the 7-segment slower to 
		display_slower++;
		if (display_slower >= 100) {
			displayTick = (displayTick + 1) % 3;
			display_slower = 0;
		}
		
		// display on 7-segment display
	  display(displayTemp);
	
	}
}

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


