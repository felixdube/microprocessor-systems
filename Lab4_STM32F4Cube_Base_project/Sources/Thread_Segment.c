/**
  ******************************************************************************
	* @file    Thread_Segment.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    16-March-2016
  * @brief   Implement a thread for 7-segment operation
  ******************************************************************************
  */

#include "Thread_Segment.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "segment_controller.h"

volatile float display_value = 0;
volatile int degree_on = 1;
volatile int flash_alarm = 0;

osThreadId tid_Thread_Segment;                              // thread id
osThreadDef(Thread_Segment, osPriorityHigh, 1, 0);

/**
	* @brief Start 7-segment display thread
	* @param none
	* @retval none
	*/
int start_Thread_Segment(void) {
	Display_GPIO_Config();
	
  tid_Thread_Segment = osThreadCreate(osThread(Thread_Segment), NULL);
  if (!tid_Thread_Segment) return -1;
	return 0;
}

/**
	* @brief 7-segment display thread
	* @param none
	* @retval none
	*/
void Thread_Segment(void const *argument) {

	while(1) {
		if (flash_alarm) {
			flash_segment();
		} else {
			__HAL_RCC_GPIOB_CLK_ENABLE();
		}
		
		display(display_value);
		digitTimer++;
		
		display_degree(degree_on);
		
		osDelay(1);
	}
}
