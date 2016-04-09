/*******************************************************************************
  * @file    Thread_LED.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    8-April-2016
  * @brief    LED thread
  ******************************************************************************
  */

#include <stdlib.h>
#include "Thread_LED.h" 
#include "LED.h"
#include "cmsis_os.h"                  									 	// ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "Thread_SPI.h"


osThreadId tid_Thread_LED;                             	 	// thread id
osThreadDef(Thread_LED, osPriorityLow, 1, 0);
int countertest;

/**
* @brief Configures the Accelerometer and starts the Thread_Acc
* @param None
* @retval None
*/
int start_Thread_LED (void) {
	LED_GPIO_Config();
	LED_TIM_Config();
	LED_PWM_Config();
	countertest = 0; //for testing
  tid_Thread_LED = osThreadCreate(osThread(Thread_LED ), NULL); // Start LED_Thread
  if (!tid_Thread_LED) return(-1);
  return(0);
}

/**
* @brief Thread that controls the LED
* 			 LED_pattern of 0 => ON, 1 => OFF, 2 => Rotate
* @param argument:
* @retval None
*/
void Thread_LED (void const *argument) {
	while(1) {
		LED_pattern = 2;
		LED_speed = 6;
		
		switch(LED_pattern) {
			case 0:
				LED_On();
			break;
			case 1:
				LED_Off();
			break;
			case 2:
				Rotate_LED(LED_speed);
			break;
		}
	}
}

