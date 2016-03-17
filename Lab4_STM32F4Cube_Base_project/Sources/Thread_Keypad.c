/*******************************************************************************
  * @file    Thread_Keypad.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    16-March-2016
  * @brief   Keypad initialization and control Thread
  ******************************************************************************
  */
#include <stdio.h>
#include "cmsis_os.h"                   									// ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "Thread_Keypad.h"
#include "keypad.h"
#include "Thread_Segment.h"


osThreadId tid_Thread_Keypad;       											// thread id
osThreadDef(Thread_Keypad, osPriorityNormal, 1, 0);
GPIO_InitTypeDef 				Keypad_configuration;


char keypadChar;
/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
/**
* @brief Configures the Accelerometer and starts the Thread_Acc
* @param None
* @retval None
*/
int start_Thread_Keypad (void) {
	Keypad_Config();
  tid_Thread_Keypad = osThreadCreate(osThread(Thread_Keypad ), NULL); // Start LED_Thread
  if (!tid_Thread_Keypad) return(-1); 
  return(0);
}

 /*----------------------------------------------------------------------------
*      Thread  'Keypad_Thread': Toggles Keypad
 *---------------------------------------------------------------------------*/
/**
* @brief Thread that poll for keypad press
* @param None
* @retval None
*/
void Thread_Keypad (void const *argument) {
	while(1){
		//Wait for the interrupt to issue a flag before reading the value
		//0 for the timeout value so that it never starts before the flag
		//osSignalWait(0x01, osWaitForever);
		osDelay(KEYPAD_DELAY);
		keypadChar	= readKeypad();
		if (keypadChar != 'n'){
			printf("%c\n",keypadChar);
		}
			
		// the readKeypad return n if Nothing is pressed
		if(keypadChar != 'n'){
			if (keypadChar == '1'){
				stateRollPitch = 1 - stateRollPitch;
			}
			else if (keypadChar == '*'){
				stateAngleTemp = 1 - stateAngleTemp;
			}
		}	
	}
}
