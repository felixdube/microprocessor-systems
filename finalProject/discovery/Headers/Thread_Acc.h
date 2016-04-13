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
#include "cmsis_os.h"  

#define accPin GPIO_PIN_0
#define ACC_INT_FLAG 0x01

#define TAP_STRENGTH_THRESH 10
//all these times are divided by 400 hertz (the speed of the accelerometer polling)
#define TAP_MAX_TIME_BEFORE_2ND_TAP 250
#define TIME_DELAY_AFTER_SPIKE 20
#define MIN_DELAY_AFTER_TAP1 60
#define MIN_DELAY_AFTER_TAP2 150	

#define UP_SPIKE_Z 1
#define DOWN_SPIKE_Z 2
#define UP_SPIKE_Y 3
#define DOWN_SPIKE_Y 4
#define UP_SPIKE_X 5
#define DOWN_SPIKE_X 6

#define TAP_Z 1
#define TAP_Y 2
#define TAP_X 3

extern osThreadId tid_Thread_Acc;   

int start_Thread_Acc (void);
void Thread_Acc (void const *argument);

#endif
