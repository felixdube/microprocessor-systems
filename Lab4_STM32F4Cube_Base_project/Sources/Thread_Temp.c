/**
  ******************************************************************************
	* @file    Thread_Temp.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    16-March-2016
  * @brief   Implement a thread for temperature monitoring
  ******************************************************************************
  */

#include <stdlib.h>

#include "Thread_Temp.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "ADC_config.h"
#include "temperature.h"
#include "kalmanFilter.h"
#include "Thread_Segment.h"

osThreadId tid_Thread_Temperature;                              // thread id
osThreadDef(Thread_Temperature, osPriorityHigh, 1, 0);

kalmanState *adcState;

int adc_val = 0;
volatile float temp = 0;

/**
	* @brief Start temperature monitoring thread
	* @param none
	* @retval none
	*/
int start_Thread_Temperature(void) {
	/* configure temperature ADC */
	ADC1_Config();
	
	adcState = malloc(sizeof(kalmanState)); /* Init Kalman Filter */
	kalmanInit(adcState, INIT_q, INIT_r, INIT_x, INIT_p, INIT_k);
	
  tid_Thread_Temperature = osThreadCreate(osThread(Thread_Temperature), NULL);
  if (!tid_Thread_Temperature) return -1;
	return 0;
}

/**
	* @brief Temperature monitoring thread
	* @param none
	* @retval none
	*/
void Thread_Temperature (void const *argument) {

	while(1) {
		HAL_ADC_Start(&ADC1_Handle); 								/* start ADC conversion */
			
		/* wait for the conversion to be done and get data */
		if (HAL_ADC_PollForConversion(&ADC1_Handle, 10000) == HAL_OK) { 
			adc_val = HAL_ADC_GetValue(&ADC1_Handle); /* get the value */
			kalmanUpdate(adcState, adc_val); 					/* filter the data and update the filter parameters */
			temp = convertTemp(adcState->x);
			__HAL_ADC_CLEAR_FLAG(&ADC1_Handle, ADC_FLAG_EOC);
		}
				
		if (temp > THRESHHOLD_TEMP) {
			flash_alarm = 1;
		} else {
			flash_alarm = 0;
		}
				
		osDelay(50);
	}
}
