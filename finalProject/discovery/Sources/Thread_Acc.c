/*******************************************************************************
  * @file    Thread_Acc.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    16-March-2016
  * @brief    Accelerometer initialazation and control Thread
  ******************************************************************************
  */

#include <stdlib.h>
#include <math.h>
#include "cmsis_os.h"                  									 	// ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "Thread_Acc.h"
#include "Thread_SPI.h"
#include "lis3dsh.h"
#include "kalmanFilter.h"
#include "accelerometer.h"

void Thread_Acc (void const *argument);                 	// thread function
osThreadId tid_Thread_Acc;                             	 	// thread id
osThreadDef(Thread_Acc, osPriorityHigh, 1, 0);
int verifyDoubleTapTemp = 0;

/**
* @brief Configures the Accelerometer and starts the Thread_Acc
* @param None
* @retval None
*/
int start_Thread_Acc (void) {
	Accelerometer_Config();
	Accelerometer_GPIO_Config();
	Accelerometer_Interrupt_Config();
  tid_Thread_Acc = osThreadCreate(osThread(Thread_Acc ), NULL); // Start LED_Thread
  if (!tid_Thread_Acc) return(-1);
	ReadAcc();
	zAccInitial = accValue[2];			// Initialize the reference for doubletap
  return(0);
}

/**
* @brief Thread that read the accelerometer
* @param argument:
* @retval None
*/
void Thread_Acc (void const *argument) {
	while(1) {
		//Wait for the interrupt to issue a flag before reading the value
		//0 for the timeout value so that it never starts before the flag
		osSignalWait(ACC_INT_FLAG, osWaitForever);
		ReadAcc();
		/* DON'T DELETE printf for matlab script */
		//printf("%f,%f,%f,%f,%f,%f\n",accValue[2], zState->q,zState->r, zState->x, zState->p, zState->k);
		
		//verifies if there is a double tap
    verifyDoubleTapTemp = verifyDoubleTap();
		if(verifyDoubleTapTemp == TAP_Z){
			printf("\n\nYou Z DOUBLE TAPPED!!\n");
			doubleTapZ = 1;
		}
		else if(verifyDoubleTapTemp == TAP_Y){
			printf("\n\nYou Y DOUBLE TAPPED!!\n");
			doubleTapY = 1;
		}
 		else if(verifyDoubleTapTemp == TAP_X){
			printf("\n\nYou X DOUBLE TAPPED!!\n");
			doubleTapX = 1;
		}
		//clears the interrupt flag
		osSignalClear(tid_Thread_Acc,ACC_INT_FLAG);
	}
}
