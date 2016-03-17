/*******************************************************************************
  * @file    main.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    16-March-2016
  * @brief   
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection
#include "system_clock.h"
<<<<<<< HEAD
#include "Thread_Temp.h"
#include "Thread_Segment.h"
=======
#include "Thread_Acc.h"
#include "main.h"
>>>>>>> 87650f9d1b334c1599e6c236c9aedd7c330c4996

extern void initializeLED_IO			(void);
extern void start_Thread_LED			(void);
extern void Thread_LED(void const *argument);
extern osThreadId tid_Thread_LED;

/**
	These lines are mandatory to make CMSIS-RTOS RTX work with te new Cube HAL
*/
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif


int main (void) {
  osKernelInitialize();                     /* initialize CMSIS-RTOS          */

  HAL_Init();                               /* Initialize the HAL Library     */

  SystemClock_Config();                     /* Configure the System Clock     */

<<<<<<< HEAD
	start_Thread_Temperature();
	start_Thread_Segment();
	
=======
	/* User codes goes here*/
  start_Thread_Acc();                       /* Create LED thread              */
	/* User codes ends here*/
  
>>>>>>> 87650f9d1b334c1599e6c236c9aedd7c330c4996
	osKernelStart();                          /* start thread execution         */
	while(1) {
		display_value = temp;
		osDelay(500);
	}
}

