/**
  ******************************************************************************
  * File Name          : alarm.c
  * Description        : Header for alarm.c
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "alarm.h"

volatile int alarmTick = 0;

/**
	* @brief Configure the GPIO pins for controlling the LED for the alarm
	* @param None
	* @retval None
	*/
void Alarm_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitDef; 	//Initialize struct
	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable clock for GPOID 
	//All will have same mode
	GPIO_InitDef.Pin = LED1 | LED2 | LED3 | LED4;
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			//push pull
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		// max frequency for our processor is 84MHz
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
}

/**
	* @brief cycle throught the leds as an alarm
	* @param None
	* @retval None
	*/
void trigger_alarm(void) {
	//alarmTick gets incremented with Systick and goes back to 0 to 100
	//Here the each LED is put on for 25 * systick period (in our case 1/4 second)
	switch(alarmTick / 25) {
		case 0:
		  HAL_GPIO_WritePin(GPIOD, LED1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_RESET);
		break;
		case 1:
		  HAL_GPIO_WritePin(GPIOD, LED1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, LED3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_RESET);
		break;
		case 2:
		  HAL_GPIO_WritePin(GPIOD, LED1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_RESET);
		break;
		case 3:
		  HAL_GPIO_WritePin(GPIOD, LED1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_SET);
	}
}

/**
	* @brief Turn off all the leds of the alarm
	* @param None
	* @retval None
	*/
void shutoff_alarm(void) {
	HAL_GPIO_WritePin(GPIOD, LED1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_RESET);
}
