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

////////***************************************
//				Can probably not avoid using HAL_TIM_PWM_Start in brightness control
//				To test
//////////***********************



/* Initialize struct */
TIM_Base_InitTypeDef TIM_Base_InitDef;
TIM_HandleTypeDef TIM_HandleDef;
TIM_OC_InitTypeDef TIM_OC_InitDef;

int delay;
int counter;
int LED_1_brightness,LED_2_brightness,LED_3_brightness,LED_4_brightness;

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
		printf("Error in timer initialization\n");
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

	TIM_OC_InitDef.Pulse = 10000; //100% duty cycle
	
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
}




/**
	* @brief cycle throught the leds
	* @param speed: number ranging from -10 to 10 for the speed of the rotation
	* where negative number make the rotation go counterclockwise
	* @retval None
	*/
void Rotate_LED(int speed) {
	if (speed == 0){
		LED_On();
		osDelay(400);
	}
	else {
		//sets the delay between led switch, the higher the speed, the lower the delay
		delay = (int)(1000/abs(speed)^3);
		switch(counter) {
			case 0:
				Change_PWM(LED_1_brightness,1);
				Change_PWM(ZERO_BRIGHTNESS,2);
				Change_PWM(ZERO_BRIGHTNESS,3);
				Change_PWM(ZERO_BRIGHTNESS,4);
			break;
			case 1:
				Change_PWM(ZERO_BRIGHTNESS,1);
				if (LED_speed >= 0) Change_PWM(LED_2_brightness,2);
				else Change_PWM(ZERO_BRIGHTNESS,2);
				Change_PWM(ZERO_BRIGHTNESS,3);
				if (LED_speed >= 0) Change_PWM(ZERO_BRIGHTNESS,4);
				else Change_PWM(LED_4_brightness,4);
			break;
			case 2:
				Change_PWM(ZERO_BRIGHTNESS,1);
				Change_PWM(ZERO_BRIGHTNESS,2);
				Change_PWM(LED_3_brightness,3);
				Change_PWM(ZERO_BRIGHTNESS,4);
			break;
			case 3:
				Change_PWM(ZERO_BRIGHTNESS,1);
				if (LED_speed >= 0) Change_PWM(ZERO_BRIGHTNESS,2);
				else Change_PWM(LED_2_brightness,2);
				Change_PWM(ZERO_BRIGHTNESS,3);
				if (LED_speed >= 0) Change_PWM(LED_4_brightness,4);
				else Change_PWM(ZERO_BRIGHTNESS,4);
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
	* Note there is an osDelay of 100ms to make the thread use less processor time
	* @param None
	* @retval None
	*/
void LED_On(void) {
	Change_PWM(LED_1_brightness,1);
	Change_PWM(LED_2_brightness,2);
	Change_PWM(LED_3_brightness,3);
	Change_PWM(LED_4_brightness,4);
	osDelay(100);
}

/**
	* @brief Turn off all the leds of the alarm 
	* Note there is an osDelay of 100ms to make the thread use less processor time
	* @param None
	* @retval None
	*/
void LED_Off(void) {
	Change_PWM(ZERO_BRIGHTNESS,1);
	Change_PWM(ZERO_BRIGHTNESS,2);
	Change_PWM(ZERO_BRIGHTNESS,3);
	Change_PWM(ZERO_BRIGHTNESS,4);
	osDelay(100);
}
/**
	* @brief Adjusts the PWM duty cycle of a single LED
	* @param brightness: duty cycle from 0 to 100
	* @retval None
	*/
void Change_PWM(int brightness, int LED_number){
		TIM_OC_InitDef.Pulse = brightness*100; /* duty cycle */
	switch(LED_number) {
		case 1:
			//LED 1
			HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef,&TIM_OC_InitDef,TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&TIM_HandleDef,TIM_CHANNEL_1);
		break;
		case 2:
			//LED 2
			HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef,&TIM_OC_InitDef,TIM_CHANNEL_2);
			HAL_TIM_PWM_Start(&TIM_HandleDef,TIM_CHANNEL_2);
		break;
		case 3:
			//LED 3
			HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef,&TIM_OC_InitDef,TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&TIM_HandleDef,TIM_CHANNEL_3);
		break;
		case 4:
			//LED 4
			HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef,&TIM_OC_InitDef,TIM_CHANNEL_4);
			HAL_TIM_PWM_Start(&TIM_HandleDef,TIM_CHANNEL_4);
		break;
	}
}
/**
	* @brief changes the brightness level of all patterns of a single LED
	* @param brightness: duty cycle from 0 to 100
	* @param LED_number: number of the LED, 5 for all
	* @retval None
	*/
void Adjust_Brightness_Level(int brightness, int LED_number){
	switch(LED_number) {
		case 1:
			LED_1_brightness = brightness;
		break;
		case 2:
			LED_2_brightness = brightness;
		break;
		case 3:
			LED_3_brightness = brightness;
		break;
		case 4:
			LED_4_brightness = brightness;
		break;
		case 5:
			LED_1_brightness = brightness;
			LED_2_brightness = brightness;
			LED_3_brightness = brightness;
			LED_4_brightness = brightness;
		break;
		default:
			LED_1_brightness = FULL_BRIGHTNESS;
			LED_2_brightness = FULL_BRIGHTNESS;
			LED_3_brightness = FULL_BRIGHTNESS;
			LED_4_brightness = FULL_BRIGHTNESS;
		break;
	}
}

