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

#define TAP_STRENGTH_THRESH 10
//all these times are divided by 400 hertz (the speed of the accelerometer polling)
#define TAP_MAX_TIME_BEFORE_2ND_TAP 250
#define TIME_DELAY_AFTER_SPIKE 20
#define MIN_DELAY_AFTER_TAP1 50
#define MIN_DELAY_AFTER_TAP2 120	

#define UP_SPIKE 1
#define DOWN_SPIKE 2

extern osThreadId tid_Thread_Acc;   

int start_Thread_Acc (void);
void Thread_Acc (void const *argument);
int verifyDoubleTap(void);
int detectBigVariation(void);
int DetectSpike(void);

#endif
