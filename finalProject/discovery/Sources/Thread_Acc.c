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
float zAccInitial;
int timeBetweenTaps;
int delayFalsePositives;
int lastSpikeDetected;
uint8_t ctrl = 0x00;
int cleanSpike;
int spike;

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
	zAccInitial = accValue[2];			// Initialize the tap reference
	lastSpikeDetected = 0, spike = 0, cleanSpike = 0;
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
		if(verifyDoubleTap() == 1){
			printf("\n\nSUCCESS DT\n");
			doubleTap = 1;
		}
		osSignalClear(tid_Thread_Acc,ACC_INT_FLAG);
	}
}

/**
* @brief verifies if the user has double tapped the board from accelerometer raw values
* @param None
* @retval 1 if a double tap has been detected, 0 otherwise
*/
int verifyDoubleTap(void){
	
	if (spike == 0){
		if (DetectSpike() == 1){
			spike = 1;
			delayFalsePositives = 0;
		}
	}
	//if spike = 1
	else {
		//little buffer not to capture vibration
		if (delayFalsePositives <= TIME_DELAY_AFTER_SPIKE){
			//just to update the value of lastSpikeDetected

			DetectSpike();
		}
		else if ((cleanSpike == 0) && (delayFalsePositives <= MIN_DELAY_AFTER_TAP1)){
			//check for too much spikes
			if (DetectSpike() == 1) {
				printf("Vibration1\n");
				spike = 0;
				cleanSpike = 0;
				delayFalsePositives = 0;
			}	
			
		}
		else if ((cleanSpike == 1) && (delayFalsePositives <= MIN_DELAY_AFTER_TAP2)){
			//check for too much spikes
			if (DetectSpike() == 1) {
				printf("Vibration2\n");
				spike = 0;
				cleanSpike = 0;
				delayFalsePositives = 0;
			}	
		}
		else {
			cleanSpike++;
			printf("clean spike\n");
			spike = 0;
			delayFalsePositives = 0;
		}
		delayFalsePositives++;
	}
	
	//makes sure the max time between taps is not higher than threshold
	if (cleanSpike == 1){
		timeBetweenTaps++;
		if (timeBetweenTaps >= TAP_MAX_TIME_BEFORE_2ND_TAP){
				printf("\n Time before 2nd Tap too long error play with TAP_MAX_TIME_BEFORE_2ND_TAP\n");
			cleanSpike = 0;
			timeBetweenTaps = 0;
		}
	}
	// checks if there is 2 clean spikes and returns 1 
	else if (cleanSpike >= 2){
		cleanSpike = 0;
		spike = 0;
		timeBetweenTaps = 0;
		return 1;
	}
		
	return 0;
}

/**
* @brief detect if the difference between 2 consecutive readings is higher than a threshold
* @param None
* @retval returns 1 if up spike and 2 if down spike, 0 otherwise
*/
int detectBigVariation(void){
	if ((accValue[2] - zAccInitial) >= TAP_STRENGTH_THRESH) {
		return 1;
	}
	else if ((zAccInitial - accValue[2]) >= TAP_STRENGTH_THRESH) {
		return 2;
	}
	return 0;
}

/**
* @brief detect if if there is either an up spike followed by a down spike, or the opposite
* @param None
* @retval returns 1 if full spike detected, 0 otherwise
*/
int DetectSpike(void){
	if (detectBigVariation() == UP_SPIKE){
		zAccInitial = accValue[2];
		if (lastSpikeDetected == DOWN_SPIKE){
			return 1;
		}
		lastSpikeDetected = UP_SPIKE;
	}
		
	else if (detectBigVariation() == DOWN_SPIKE){
		zAccInitial = accValue[2];
		if (lastSpikeDetected == UP_SPIKE){
			return 1;
		}
		lastSpikeDetected = DOWN_SPIKE;
	}
	zAccInitial = accValue[2];
	return 0;
}
