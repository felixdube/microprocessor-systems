/**
  ******************************************************************************
	* @file    Thread_Temp.h
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    16-March-2016
  * @brief   Implement a thread for temperature monitoring
  ******************************************************************************
  */

#ifndef THREAD_TEMP_H
#define THREAD_TEMP_H

#include "cmsis_os.h"  

#define TEMP_DELAY 50 //temperature polling period (ms)

/* kalman parameters */
#define INIT_TEMP_q 0.001 			/* this parameter controls the speed of convergence */
#define INIT_TEMP_r 2 					/* this parameter controls the speed of convergence */
#define INIT_TEMP_x 1075 			/* this is approximetely 35 C as a starting value */
#define INIT_TEMP_p 0.044224 	/* this was set to the value it converges to when the filter runs for a while */
#define INIT_TEMP_k 0.022112 	/* this was set to the value it converges to when the filter runs for a while */

#define THRESHHOLD_TEMP 40
#define THRESHHOLD_TEMP_URGENT 45

extern osThreadId tid_Thread_Temperature; 

extern volatile float temperature;

int start_Thread_Temperature(void);
void Thread_Temperature(void const *argument);
void CreateMutexTemp (void);

#endif
