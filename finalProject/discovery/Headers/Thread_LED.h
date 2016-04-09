/**
  ******************************************************************************
  * File Name          : Thread_LED.h
  * Description        : header file for Thread_LED.c
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */

#ifndef THREAD_LED_H
#define THREAD_LED_H


#include "cmsis_os.h"  

#define LED1 GPIO_PIN_13
#define LED2 GPIO_PIN_14
#define LED3 GPIO_PIN_15
#define LED4 GPIO_PIN_12

#define ALL_LED 5

extern osThreadId tid_Thread_LED;

int start_Thread_LED (void);
void Thread_LED (void const *argument);

#endif
