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

/* initialize flags */
volatile int accFlag = 0;

/**
* @brief Initialise the Accelerometer
* @param None
* @retval None
*/
void Accelerometer_Config(void) {
	
	/* initialise accelerometer */
	LIS3DSH_InitTypeDef Acc_InitDef;
	
	/* define field of the accelerometer initialisation structure */
	Acc_InitDef.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;      								/* 25Hz */
	Acc_InitDef.Axes_Enable = LIS3DSH_XYZ_ENABLE;                     									/* XYZ */
	Acc_InitDef.Continous_Update = LIS3DSH_ContinousUpdate_Enabled;											/* continuous update */
	Acc_InitDef.AA_Filter_BW = LIS3DSH_AA_BW_50;																				/* 50Hz */				
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
	
	/* Enable clock for GPOIB */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	 
	/* All will have same mode */
	Acc_GPIO_InitDef.Pin = accPin;
	Acc_GPIO_InitDef.Mode = GPIO_MODE_IT_RISING;   			/* push pull */
	Acc_GPIO_InitDef.Pull = GPIO_NOPULL;
	Acc_GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		/* max frequency for our processor is 84MHz */
	 
	HAL_GPIO_Init(GPIOB, &Acc_GPIO_InitDef);
}
	
void EXTI0_IRQHandler (void) {
	accFlag = 1;
	HAL_GPIO_EXTI_IRQHandler(accPin);
}
	
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
}
