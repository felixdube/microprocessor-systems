/**
  ******************************************************************************
  * File Name          : segment_controller.c
  * Description        : Allow to display numbers on 4 7-segments displays
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "segment_controller.h"

volatile int displayTick = 0;
int patterns[10][7] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
int segments[7] = {segA, segB, segC, segD, segE, segF, segG};

/* GPIO configuration */
void Display_GPIO_Config(void) {
	//Initialize struct
	GPIO_InitTypeDef GPIO_InitDef;
	
	//Enable clock for GPOIG
	__HAL_RCC_GPIOB_CLK_ENABLE();
	 
	//All will have same mode
	GPIO_InitDef.Pin = segA | segB | segC | segD | segE | segF | segG | segDP | sel1 | sel2 | sel3 | sel4;
	//Mode output
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   //push pull
	//Without pull resistors
	GPIO_InitDef.Pull = GPIO_NOPULL;
	//pin speed
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;// max frequency for our processor is 84MHz
	 
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
}

/**
	* @brief Display a value on the 4 7-segment displays
	* @param value: value to be display
	* @retval None
	*/
void display(float value) {
	int number = getDigit(23.8, displayTick);
	setPins(number);
}

/**
	* @brief Calculate what is the digit to be display on a particuliar display for a particuliar value
	* @param value: value to be displayed
	* @param place: for which display the digitvalue is being calculated
	* @retval value of the digit
	*/
int getDigit(float value, int place) {
	int tmp = (int) value * 100;
	switch (place) {
		case 0:
			return (tmp - tmp % 1000) / 1000;
		case 1:
			return (tmp % 1000 - tmp % 100) / 100;
		case 2:
			return (tmp % 100 - tmp % 10) / 10;
		case 3:
			return tmp % 10;
		default:
			return 0;
	}
}

/**
	* @brief
	* @param number:
	* @retval None
	*/
void setPins(int number) {
	int *pattern;
	int i;
	uint16_t displayPin;
	pattern = patterns[number];
	for (i = 0; i< 7; i++){
		printf("%i", pattern[i]);
	}
	printf("\n");
	
	switch (displayTick) {
		case 0:
			displayPin = sel1;
			break;
		case 1:
			displayPin = sel2;
			break;
		case 2:
			displayPin = sel3;
			break;
	}
	
	HAL_GPIO_WritePin(GPIOB, displayPin, GPIO_PIN_SET);
	
	for (i = 0; i < 8; i++) {
		if (pattern[i] == 1) {
			HAL_GPIO_WritePin(GPIOB, segments[i], GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, segments[i], GPIO_PIN_RESET);
		}
	}
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, displayPin, GPIO_PIN_RESET);

}
