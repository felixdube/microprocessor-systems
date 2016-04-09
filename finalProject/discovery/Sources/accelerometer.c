/**
  ******************************************************************************
	* @file    accelerometer.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    February-2016
  * @brief   Accelerometer initialazation and control
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "accelerometer.h"
#include "lis3dsh.h"
#include "kalmanFilter.h"
#include <stdlib.h>
#include <math.h>
#include "Thread_Acc.h"
#include "Thread_SPI.h"

float accValue[3] = {0, 0, 0};				// {AccX, AccY, AccZ}
float tmp0, tmp1, tmp2;								// Temp variable used for calibration

kalmanState xState = {INIT_q, INIT_r, INIT_x, INIT_p, INIT_k};
kalmanState yState = {INIT_q, INIT_r, INIT_y, INIT_p, INIT_k};
kalmanState zState = {INIT_q, INIT_r, INIT_z, INIT_p, INIT_k};

//variables for double tap
float zAccInitial;
int timeBetweenTaps;
int delayFalsePositives;
int lastSpikeDetected = 0;
int cleanSpike = 0;
int spike = 0;

/**
* @brief Initialise the Accelerometer
* @param None
* @retval None
*/
void Accelerometer_Config(void) {

	/* initialise accelerometer */
	LIS3DSH_InitTypeDef Acc_InitDef;

	/* define field of the accelerometer initialisation structure */
	Acc_InitDef.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_400;      								/* 25Hz */
	Acc_InitDef.Axes_Enable = LIS3DSH_XYZ_ENABLE;                     									/* XYZ */
	Acc_InitDef.Continous_Update = LIS3DSH_ContinousUpdate_Disabled;										/* continuous update */
	Acc_InitDef.AA_Filter_BW = LIS3DSH_AA_BW_50;																				/* 50Hz to filter gravity*/
	Acc_InitDef.Full_Scale = LIS3DSH_FULLSCALE_2;																				/* 2g */

	LIS3DSH_Init(&Acc_InitDef);
}

/**
* @brief Initialise the Interrupt for the Accelerometer
* @param None
* @retval None
*/
void Accelerometer_Interrupt_Config(void) {

	/* initilalize accelerometer interupt */
	LIS3DSH_DRYInterruptConfigTypeDef Acc_Interrupt_InitDef;

	/* define field of the accelerometer interupt initialisation structure */
	Acc_Interrupt_InitDef.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;   							/* enable */
	Acc_Interrupt_InitDef.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;                  	/* active high */
	Acc_Interrupt_InitDef.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;                     			/* pulse interupt */

	LIS3DSH_DataReadyInterruptConfig(&Acc_Interrupt_InitDef);

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 1);
}

/**
* @brief Initialise the GPIO for the Accelerometer
* @param None
* @retval None
*/
void Accelerometer_GPIO_Config(void) {

	/* Initialize struct */
	GPIO_InitTypeDef Acc_GPIO_InitDef;

	/* Enable clock for GPOIE */
	__HAL_RCC_GPIOE_CLK_ENABLE();

	/* All will have same mode */
	Acc_GPIO_InitDef.Pin = accPin;
	Acc_GPIO_InitDef.Mode = GPIO_MODE_IT_RISING;   			/* External Interrupt Mode with Rising edge trigger detection*/
	Acc_GPIO_InitDef.Pull = GPIO_NOPULL;
	Acc_GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		/* max frequency for our processor is 84MHz */

	HAL_GPIO_Init(GPIOE, &Acc_GPIO_InitDef);
}

/**
  * @brief  Calculate the pitch using 3D acceleration
	* @param  x:	acceleration in x axis
	* @param 	y:	acceleration in y axis
	* @param	z:	acceleration in z axis
  * @retval pitch in degrees
  */
float calcPitch (float x, float y, float z) {
	float pitch = atan2(x, (sqrt(y*y+z*z))) * 180.0 / PI;

	//Normalize the pitch to a value between 0-180
	if ( z < 0 && pitch < 0){
		pitch = 180 + pitch;
	}
	else if( z < 0 && pitch > 0) {
			pitch = 180 - pitch;
	}
	else if ( pitch < 0 ){
		pitch = -pitch;
	}
	return pitch;
}

/**
  * @brief  Calculate the roll using 3D acceleration
	* @param  x:	acceleration in x axis
	* @param 	y:	acceleration in y axis
	* @param	z:	acceleration in z axis
  * @retval pitch in degrees
  */
float calcRoll (float x, float y, float z) {
	float roll = atan2(y, (sqrt(x*x+z*z))) * 180.0 / PI;

	//Normalize the pitch to a value between 0-180
	if ( z < 0 && roll < 0){
		roll = 180 + roll;
	}
	else if( z < 0 && roll > 0) {
			roll = 180 - roll;
	}
	else if ( roll < 0 ){
		roll = -roll;
	}
	return roll;
}


/**
  * @brief  Calibrate the accelerometer data
	* @param  out[]: acceleration values to be calibrated
  * @retval None
  */
void Calibrate(float* out) {
	tmp0 = out[0]; // x
	tmp1 = out[1]; // y
	tmp2 = out[2]; // z
	out[0] = tmp0*(float)cal_X11 + tmp1*(float)cal_X21 + tmp2*(float)cal_X31 + (float)cal_X41*1000;
	out[1] = tmp0*(float)cal_X12 + tmp1*(float)cal_X22 + tmp2*(float)cal_X32 + (float)cal_X42*1000;
	out[2] = tmp0*(float)cal_X13 + tmp1*(float)cal_X23 + tmp2*(float)cal_X33 + (float)cal_X43*1000;
}


/**
  * @brief  Read Acc values, Calibrate them, filter them, and update the pitch value to be displayed
	* @param  None
  * @retval None
  */
void ReadAcc(void){
		/* Get values */
	LIS3DSH_ReadACC(accValue);
	Calibrate(accValue);

	/* Filter values */
	kalmanUpdate(&xState, accValue[0]);
	kalmanUpdate(&yState, accValue[1]);
	kalmanUpdate(&zState, accValue[2]);

	pitchAngle = calcPitch(xState.x, yState.x, zState.x);
	rollAngle = calcRoll(xState.x, yState.x, zState.x);
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
		//little buffer not to capture too much vibration
		if (delayFalsePositives <= TIME_DELAY_AFTER_SPIKE){
			//just to update the value of lastSpikeDetected
			DetectSpike();
		}
		else if ((cleanSpike == 0) && (delayFalsePositives <= MIN_DELAY_AFTER_TAP1)){
			//check for too much spikes
			if (DetectSpike() == 1) {
				printf("Vibration on tap 1\n");
				spike = 0;
				cleanSpike = 0;
				delayFalsePositives = 0;
			}	
			
		}
		else if ((cleanSpike == 1) && (delayFalsePositives <= MIN_DELAY_AFTER_TAP2)){
			//check for too much spikes
			if (DetectSpike() == 1) {
				printf("Vibration on tap 2\n");
				spike = 0;
				cleanSpike = 0;
				delayFalsePositives = 0;
			}	
		}
		//no additional spikes detected so count as a increment cleanspike
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
				printf("Time before 2nd tap too long error\n\n");
			cleanSpike = 0;
			timeBetweenTaps = 0;
		}
	}
	// check if there is 2 clean spikes and return 1 
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

