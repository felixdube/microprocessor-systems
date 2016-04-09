/*******************************************************************************
  * @file    LED.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
  * @version 1.0.0
  * @date    8-April-2016
  * @brief   LED initialization and control methods
  ******************************************************************************
  */
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "LED.h"
#include "cmsis_os.h"
#include "SPI.h"
#include "Thread_SPI.h"
#include "stm32f4xx.h"

/* Initialize struct */
TIM_Base_InitTypeDef TIM_Base_InitDef;
TIM_HandleTypeDef TIM_HandleDef;

TIM_OC_InitTypeDef TIM_OC_InitDef;

volatile int alarmLedTimer = 0;
int delay;
int counter;

/**
	* @brief Configure the GPIO pins for controlling the LED with PWM
	* @param None
	* @retval None
	*/
void LED_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitDef; 									/* Initialize struct */
	__HAL_RCC_GPIOD_CLK_ENABLE(); 									/* Enable clock for GPOID  */
	GPIO_InitDef.Pin = LED1 | LED2 | LED3 | LED4; 	/* All will have same mode */
	GPIO_InitDef.Mode = GPIO_MODE_AF_PP;   			/* push pull */
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		/* max frequency for our processor is 84MHz */
	GPIO_InitDef.Alternate =  GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
}


/**
	* @brief timer configuration
	* @param None
	* @retval None
	*/
void LED_TIM_Config(void) {
	/* Enable clock for TIM4 */
	__HAL_RCC_TIM4_CLK_ENABLE();
	/* The desired frequency = 10MHz / (Prescaler*Period)*/
	TIM_Base_InitDef.Prescaler = 1;
	TIM_Base_InitDef.Period = 10000;
	TIM_Base_InitDef.CounterMode = TIM_COUNTERMODE_DOWN;

	TIM_HandleDef.Instance = TIM4;
	TIM_HandleDef.Init = TIM_Base_InitDef;
	if(HAL_TIM_Base_Start(&TIM_HandleDef) != HAL_OK){
		printf("Error in timer \n");
	}

}



void LED_PWM_Config(void) {
	//might need to put before
	HAL_TIM_PWM_MspInit(&TIM_HandleDef);

	/* PWM mode 2 = Clear on compare match */
	/* PWM mode 1 = Set on compare match */	
	TIM_OC_InitDef.OCMode = TIM_OCMODE_PWM2;
	TIM_OC_InitDef.OCIdleState = TIM_OCIDLESTATE_SET;
	TIM_OC_InitDef.OCPolarity = TIM_OCPOLARITY_LOW;
	/*
	To get proper duty cycle, you have simple equation
	
	pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
	
	where DutyCycle is in percent, between 0 and 100%
	
	25% duty cycle: 	pulse_length = ((8399 + 1) * 25) / 100 - 1 = 2099
	50% duty cycle: 	pulse_length = ((8399 + 1) * 50) / 100 - 1 = 4199
	75% duty cycle: 	pulse_length = ((8399 + 1) * 75) / 100 - 1 = 6299
	100% duty cycle:	pulse_length = ((8399 + 1) * 100) / 100 - 1 = 8399
	
	Remember: if pulse_length is larger than TIM_Period, you will have output HIGH all the time
*/
	TIM_OC_InitDef.Pulse = 10000; /* 5% duty cycle */
	
	if(HAL_TIM_PWM_Init(&TIM_HandleDef) != HAL_OK){
		printf("Error in PWM init \n");
	}
	//LED 1
	if(HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef,&TIM_OC_InitDef,TIM_CHANNEL_1) != HAL_OK){
		printf("channel 1 PWM config error\n");}
	if(HAL_TIM_PWM_Start(&TIM_HandleDef,TIM_CHANNEL_1) != HAL_OK){
		printf("channel 1 Error in PWM start\n");}
	//LED 2
	if(HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef,&TIM_OC_InitDef,TIM_CHANNEL_2) != HAL_OK){
		printf("channel 2 PWM config error\n");}
	if(HAL_TIM_PWM_Start(&TIM_HandleDef,TIM_CHANNEL_2) != HAL_OK){
		printf("channel 2 Error in PWM start\n");}
	//LED 3
	if(HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef,&TIM_OC_InitDef,TIM_CHANNEL_3) != HAL_OK){
		printf("channel 3 PWM config error\n");}
	if(HAL_TIM_PWM_Start(&TIM_HandleDef,TIM_CHANNEL_3) != HAL_OK){
		printf("channel 3 Error in PWM start\n");}
	//LED 4
	if(HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef,&TIM_OC_InitDef,TIM_CHANNEL_4) != HAL_OK){
		printf("channel 4 PWM config error\n");}
	if(HAL_TIM_PWM_Start(&TIM_HandleDef,TIM_CHANNEL_4) != HAL_OK){
		printf("channel 4 Error in PWM start\n");}	

	


//	TIM_OC1Init(TIM4, &TIM_OC_InitDef);
//	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
//	
//	TIM_OC_InitDef.Pulse = 4199; /* 50% duty cycle */
//	TIM_OC2Init(TIM4, &TIM_OC_InitDef);
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
//	
//	TIM_OC_InitDef.Pulse = 6299; /* 75% duty cycle */
//	TIM_OC3Init(TIM4, &TIM_OC_InitDef);
//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
//	
//	TIM_OC_InitDef.Pulse = 8399; /* 100% duty cycle */
//	TIM_OC4Init(TIM4, &TIM_OC_InitDef);
//	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}




/**
	* @brief cycle throught the leds as an alarm
	* @param speed: number ranging from -10 to 10 for the speed of the rotation
	* 			 where negative number make the rotation go counterclockwise
	* @retval None
	*/
void Rotate_LED(int speed) {
	/* each LED is on for DISPLAY_TIME_1_LED_ALARM */
	if (speed == 0){
		LED_On();
		osDelay(400);
	}
	else {
	delay = (int)(1000/abs(speed)^3);
	switch(counter) {
		case 0:
		  HAL_GPIO_WritePin(GPIOD, LED1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_RESET);
		break;
		case 1:
		  HAL_GPIO_WritePin(GPIOD, LED1, GPIO_PIN_RESET);
		  if (LED_speed >= 0) HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_SET);
		  else HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED3, GPIO_PIN_RESET);
		  if (LED_speed >= 0) HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_RESET);
		  else HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_SET);
		break;
		case 2:
		  HAL_GPIO_WritePin(GPIOD, LED1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_RESET);
		break;
		case 3:
		  HAL_GPIO_WritePin(GPIOD, LED1, GPIO_PIN_RESET);
		  if (LED_speed >= 0) HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_RESET);
		  else HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, LED3, GPIO_PIN_RESET);
		  if (LED_speed >= 0) HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_SET);
		  else HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_RESET);
	}
	//update the counter 
	if (counter < 3){	
		counter++;
	}
	else {
		counter = 0;
	}
	//variable delay time
	osDelay(delay);
	}
}

/**
	* @brief Turn off all the leds of the alarm
	* @param None
	* @retval None
	*/
void LED_On(void) {
	alarmLedTimer =0;
	HAL_GPIO_WritePin(GPIOD, LED1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, LED3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_SET);
}

/**
	* @brief Turn off all the leds of the alarm
	* @param None
	* @retval None
	*/
void LED_Off(void) {
	alarmLedTimer =0;
	HAL_GPIO_WritePin(GPIOD, LED1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED4, GPIO_PIN_RESET);
}
/**
	* @brief Adjusts the brightness of the LED
	* @param brightness: duty cycle from 0 to 100
	* @retval None
	*/
void Change_LED_Brightness(int brightness){
		TIM_OC_InitDef.Pulse = brightness*100; /* 5% duty cycle */
	//LED 1
	if(HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef,&TIM_OC_InitDef,TIM_CHANNEL_1) != HAL_OK){
		printf("channel 1 PWM config error\n");}
	if(HAL_TIM_PWM_Start(&TIM_HandleDef,TIM_CHANNEL_1) != HAL_OK){
		printf("channel 1 Error in PWM start\n");}
	//LED 2
	if(HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef,&TIM_OC_InitDef,TIM_CHANNEL_2) != HAL_OK){
		printf("channel 2 PWM config error\n");}
	if(HAL_TIM_PWM_Start(&TIM_HandleDef,TIM_CHANNEL_2) != HAL_OK){
		printf("channel 2 Error in PWM start\n");}
	//LED 3
	if(HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef,&TIM_OC_InitDef,TIM_CHANNEL_3) != HAL_OK){
		printf("channel 3 PWM config error\n");}
	if(HAL_TIM_PWM_Start(&TIM_HandleDef,TIM_CHANNEL_3) != HAL_OK){
		printf("channel 3 Error in PWM start\n");}
	//LED 4
	if(HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef,&TIM_OC_InitDef,TIM_CHANNEL_4) != HAL_OK){
		printf("channel 4 PWM config error\n");}
	if(HAL_TIM_PWM_Start(&TIM_HandleDef,TIM_CHANNEL_4) != HAL_OK){
		printf("channel 4 Error in PWM start\n");}	
}

