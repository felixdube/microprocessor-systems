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
#include <string.h>

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
volatile int doubleTap = 1;

/* variables needing to be received */
volatile int LED_pattern = 0;
volatile int LED_brightness = 0;
volatile int LED_speed = 0;
volatile int LED_PWM_duty_cycle = 0;

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
    Exchange_Byte(PREFIX_BYTE);
    transmitFloat(temperature);
    transmitFloat(pitchAngle);
    transmitFloat(rollAngle);
    Exchange_Byte((uint8_t) doubleTap);
    
    osDelay(2 * BYTE_DELAY);
    
    LED_pattern = Exchange_Byte(EMPTY_BYTE);
    LED_pattern = Exchange_Byte(EMPTY_BYTE);
    osDelay(BYTE_DELAY);
    LED_brightness = Exchange_Byte(EMPTY_BYTE);
    osDelay(BYTE_DELAY);
    LED_speed = Exchange_Byte(EMPTY_BYTE);
    osDelay(BYTE_DELAY);
    LED_PWM_duty_cycle = Exchange_Byte(EMPTY_BYTE);
    
    printf("%d, %d, %d, %d\n", LED_pattern, LED_brightness, LED_speed, LED_PWM_duty_cycle);
    osDelay(COM_DELAY);
  }
}

void transmitFloat(float f) {
  char data[sizeof(float)];
  memcpy(data, &f, sizeof f);
  Exchange_Byte((uint8_t) data[0]);
  osDelay(BYTE_DELAY);
  Exchange_Byte((uint8_t) data[1]);
  osDelay(BYTE_DELAY);
  Exchange_Byte((uint8_t) data[2]);
  osDelay(BYTE_DELAY);
  Exchange_Byte((uint8_t) data[3]);
  osDelay(BYTE_DELAY);
}
