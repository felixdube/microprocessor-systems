/**
  ******************************************************************************
  * File Name          : accelerometer.c
  * Description        : Accelerometer initialazation and control
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */
	
#include "stm32f4xx_hal.h"
#include "accelerometer.h"
#include "lis3dsh.h"
#include "kalmanFilter.h"
#include <stdlib.h>
#include <math.h>
#include "keypad.h"

float accValue[3] = {0, 0, 0};
float pitch = 0;
float tmp0, tmp1, tmp2;


/**
* @brief Initialise the Accelerometer
* @param None
* @retval None
*/
void Accelerometer_Config(void) {
	
	/* initialise accelerometer */
	LIS3DSH_InitTypeDef Acc_InitDef;
	
	/* define field of the accelerometer initialisation structure */
	Acc_InitDef.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;      									/* 25Hz */
	Acc_InitDef.Axes_Enable = LIS3DSH_XYZ_ENABLE;                     									/* XYZ */
	Acc_InitDef.Continous_Update = LIS3DSH_ContinousUpdate_Disabled;										/* continuous update */
	Acc_InitDef.AA_Filter_BW = LIS3DSH_AA_BW_50;																				/* 50Hz to filter gravity*/				
	Acc_InitDef.Full_Scale = LIS3DSH_FULLSCALE_2;																				/* 2g */
	
	LIS3DSH_Init(&Acc_InitDef);
	

	
	/* Init Kalman Filter for the accelerometer */
	xState = {INIT_q, INIT_r, INIT_x, INIT_p, INIT_k};
	yState = {INIT_q, INIT_r, INIT_y, INIT_p, INIT_k};
	zState = {INIT_q, INIT_r, INIT_z, INIT_p, INIT_k};
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

void Calibrate(float* out) {
	tmp0 = out[0]; //x
	tmp1 = out[1]; //y
	tmp2 = out[2]; //z
	out[0] = tmp0*(float)cal_X11 + tmp1*(float)cal_X21 + tmp2*(float)cal_X31 + (float)cal_X41*1000;
	out[1] = tmp0*(float)cal_X12 + tmp1*(float)cal_X22 + tmp2*(float)cal_X32 + (float)cal_X42*1000;
	out[2] = tmp0*(float)cal_X13 + tmp1*(float)cal_X23 + tmp2*(float)cal_X33 + (float)cal_X43*1000;
	
}
