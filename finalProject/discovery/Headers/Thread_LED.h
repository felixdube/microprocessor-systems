/**
  ******************************************************************************
  * File Name          : alarm.h
  * Description        : header file for alarm.c
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */

#ifndef ALARM_H
#define ALARM_H


#include "cmsis_os.h"  

/* The maximum temperature for the processor is 85 C for minimum power dissipation
 * and 105 C for max power dissipation */
#define DISPLAY_TIME_1_LED_ALARM 40000

#define LED1 GPIO_PIN_13
#define LED2 GPIO_PIN_14
#define LED3 GPIO_PIN_15
#define LED4 GPIO_PIN_12

extern osThreadId tid_Thread_LED;   

int start_Thread_LED (void);
void Thread_LED (void const *argument);

#endif
