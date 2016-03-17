/*******************************************************************************
  * @file    Thread_Acc.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    16-March-2016
  * @brief    Accelerometer initialazation and control Thread
  ******************************************************************************
  */
	
#include "cmsis_os.h"                  									 	// ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "Thread_Acc.h"
#include "lis3dsh.h"
#include "kalmanFilter.h"
#include "accelerometer.h"
#include <stdlib.h>
#include <math.h>

void Thread_Acc (void const *argument);                 	// thread function
osThreadId tid_Thread_Acc;                             	 	// thread id
osThreadDef(Thread_Acc, osPriorityHigh, 1, 0);
osMutexDef (MutexIsr1);                                   // Mutex protecting the pitch variable
osMutexId pitchMutex; 
osMutexDef (MutexIsr2);                                   // Mutex protecting the roll variable
osMutexId rollMutex;  


/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
/**
* @brief Configures the Accelerometer and starts the Thread_Acc
* @param None
* @retval None
*/
int start_Thread_Acc (void) {
	Accelerometer_Config();
	Accelerometer_GPIO_Config();
	Accelerometer_Interrupt_Config();
	CreateMutexAcc();
  tid_Thread_Acc = osThreadCreate(osThread(Thread_Acc ), NULL); // Start LED_Thread
  if (!tid_Thread_Acc) return(-1); 
  return(0);
}

 /*----------------------------------------------------------------------------
*      Thread  'LED_Thread': Toggles Acc
 *---------------------------------------------------------------------------*/

/**
* @brief Thread that read the accelerometer
* @param argument: 
* @retval None
*/
void Thread_Acc (void const *argument) {
		while(1){
			//Wait for the interrupt to issue a flag before reading the value
			//0 for the timeout value so that it never starts before the flag
				osSignalWait(ACC_INT_FLAG, osWaitForever);
				ReadAcc();
				//printf("%f\n",accValue[0]);
				osSignalClear(tid_Thread_Acc,ACC_INT_FLAG);
			}
	}

/**
* @brief Create mutex needed to protect roll and pitch variable
* @param None
* @retval None
*/
void CreateMutexAcc (void)  { 
  pitchMutex = osMutexCreate (osMutex (MutexIsr1));

  rollMutex = osMutexCreate (osMutex (MutexIsr2));    
}
