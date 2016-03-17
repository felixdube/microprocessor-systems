/**
  ******************************************************************************
  * File Name          : Thread_Keypad.h
  * Description        : Header for Thread_Keypad.c
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : March 16, 2016
  ******************************************************************************
  */

#ifndef THREAD_KEYPAD_H
#define THREAD_KEYPAD_H

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"  

#define KEYPAD_DELAY 15 //delay (ms) to call the keypad read method
//Keep delay below 20 to be sure not to miss a quick button press

//thread id to for other threads to use to set flags
extern osThreadId tid_Thread_Keypad; 


//functions
int start_Thread_Keypad (void);
void Thread_Keypad (void const *argument);


#endif
