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

//thread id to for other threads to use to set flags
extern osThreadId tid_Thread_Acc;   
extern osMutexId pitchMutex; 
extern osMutexId rollMutex;  
//extern float accValue[3];
//extern kalmanState *xState;
//extern kalmanState *yState;
//extern kalmanState *zState;

int start_Thread_Acc (void);
void Thread_Acc (void const *argument);
void CreateMutexAcc (void);


#endif
