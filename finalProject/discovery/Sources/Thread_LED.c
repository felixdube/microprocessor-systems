/*******************************************************************************
  * @file    Thread_LED.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    8-April-2016
  * @brief   LED thread
  ******************************************************************************
  */

#include <stdlib.h>
#include "Thread_LED.h" 
#include "LED.h"
#include "cmsis_os.h" 
#include "stm32f4xx_hal.h"
#include "Thread_SPI.h"


osThreadId tid_Thread_LED;                             // thread id
osThreadDef(Thread_LED, osPriorityLow, 1, 0);

/**
* @brief Configures the LED GPIO, the TIM, the PWM and starts the Thread_Acc
* @param None
* @retval 0 upon success
*/
int start_Thread_LED (void) {
	LED_GPIO_Config();
	LED_TIM_Config();
	LED_PWM_Config();
	//set the default LED pattern
	LED_pattern = 2;
	LED_speed = 8;
	
  tid_Thread_LED = osThreadCreate(osThread(Thread_LED ), NULL); // Start LED_Thread
  if (!tid_Thread_LED) return(-1);
  return(0);
}

/**
* @brief Thread that controls the LED
* 			 LED_pattern = 0 => ON, 1 => OFF, 2 => Rotate
* @param None
* @retval None
*/
void Thread_LED (void const *argument) {
	//adjusts the brightness of all LED
	//could control LED individually if we set it up on the Android side
	Adjust_Brightness_Level(LED_brightness, ALL_LED);
	while(1) {		
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
		
		//note that there is no Delay in this thread directly, but all the pattern functions
		//have some osDelay to save processor time
	}
}

