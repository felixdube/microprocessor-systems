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

#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "SPI.h"
#include "Thread_SPI.h"

osThreadId tid_Thread_SPI;
osThreadDef(Thread_SPI, osPriorityHigh, 1, 0);

/* varaibles needing to be sent */
volatile float temperature = 12.3;
volatile float pitchAngle = 34.3;
volatile float rollAngle = 89.88;
volatile int double_tap = 1;

/* variables needing to be received */
volatile int LED_pattern = 0;

/**
  * @brief Start SPI communication thread
  * @param none
  * @retval none
  */
int start_Thread_SPI(void) {
  SPI_Init_Master();

  tid_Thread_SPI = osThreadCreate(osThread(Thread_SPI), NULL);
  if (!tid_Thread_SPI) return -1;
  return 0;
}

/**
  * @brief SPI communication thread
  * @param none
  * @retval none
  */
void Thread_SPI (void const *argument) {
  while(1) {
    Exchange_Byte(EMPTY_BYTE);
    Exchange_Byte((uint8_t) temperature * 100);
    Exchange_Byte((uint8_t) pitchAngle * 100);
    Exchange_Byte((uint8_t) rollAngle * 100);
    Exchange_Byte((uint8_t) double_tap);
    osDelay(COM_DELAY);
  }
}
