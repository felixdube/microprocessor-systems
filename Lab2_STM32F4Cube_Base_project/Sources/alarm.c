/**
  ******************************************************************************
  * File Name          : alarm.c
  * Description        : Implement methods for temperature alarm
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "alarm.h"

volatile int alarmTick = 0;

void Alarm_GPIO_Config(void) {
	//Initialize struct
	GPIO_InitTypeDef GPIO_InitDef;
	
	//Enable clock for GPOID
	__HAL_RCC_GPIOD_CLK_ENABLE();
	 
	//All will have same mode
	GPIO_InitDef.Pin = LED1 | LED2 | LED3 | LED4;
	//Mode output
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   //push pull
	//Without pull resistors
	GPIO_InitDef.Pull = GPIO_NOPULL;
	//pin speed
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;// max frequency for our processor is 84MHz
	 
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
}

void trigger_alarm(void) {
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

void shutoff_alarm(void) {
	HAL_GPIO_WritePin(GPIOD, LED1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_RESET);
}
