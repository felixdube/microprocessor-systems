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
#include "Thread_SPI.h"
#include "Thread_Temp.h"
#include "Thread_Acc.h"
#include "Thread_LED.h"

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
  osKernelInitialize();                     /* initialize CMSIS-RTOS */
  HAL_Init();                               /* Initialize the HAL Library */
  SystemClock_Config();                     /* Configure the System Clock */
  //start_Thread_SPI();
  start_Thread_Temperature();               /* Create a thread for sampling CPU temperature */
  start_Thread_Acc();                       /* Create a thread for sampling accelerometer data */
	start_Thread_LED();
  osKernelStart();                          /* start thread execution */
	
}
