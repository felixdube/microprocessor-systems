/**
  ******************************************************************************
  * File Name          : Thread_Acc.h
  * Description        : Header for Thread_Acc.c
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : March 16, 2016
  ******************************************************************************
  */

#ifndef THREAD_ACC_H
#define THREAD_ACC_H

#include "kalmanFilter.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"  

#define accPin GPIO_PIN_0
#define ACC_INT_FLAG 0x01

extern osThreadId tid_Thread_Acc;   

int start_Thread_Acc (void);
void Thread_Acc (void const *argument);

#endif
