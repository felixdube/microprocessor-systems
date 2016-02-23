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

volatile int digitToDisplay = 0;
volatile int timeDisplay1DigitTimer = 0;
const uint8_t patterns[10] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
const int segments[7] = {segA, segB, segC, segD, segE, segF, segG};

/* GPIO configuration */
void Display_GPIO_Config(void) {
	/* Initialize struct */
	GPIO_InitTypeDef GPIO_InitDef;
	
	/* Enable clock for GPOIB */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	 
	/* All will have same mode */
	GPIO_InitDef.Pin = segA | segB | segC | segD | segE | segF | segG | segDP | segDegree | sel1 | sel2 | sel3;
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			/* push pull */
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		/* max frequency for our processor is 84MHz */
	 
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
}

/**
	* @brief Display a value on the 4 7-segment displays
	* @param value: value to be display
	* @retval None
	*/
void display(float value) {
		/* change the digit to be viewed slower for the 7-segment slower for better display */
		if (timeDisplay1DigitTimer >= TIME_DISPLAY_1_DIGIT_PERIOD) {
			timeDisplay1DigitTimer = 0;
			digitToDisplay = (digitToDisplay + 1) % 3;
		}
	setPins(getDigit(value, digitToDisplay));
}

/**
	* @brief Calculate what is the digit to be display on a particuliar display for a particuliar value
	* @param value: value to be displayed
	* @param place: for which display the digitvalue is being calculated
	* @retval value of the digit
	*/
int getDigit(float value, int place) {
	int tmp = (int) (value * 10);
	switch (place) {
		case 0:
			return (tmp - tmp % 100) / 100;
		case 1:
			return (tmp % 100 - tmp % 10) / 10;
		case 2:
			return tmp % 10;
		default:
			return 0;
	}
}

/**
	* @brief set all the the segment of a display for a particuliar digit
	* @param digit: digit to be displayed input as an int
	* @retval None
	*/
void setPins(int digit) {
	uint8_t pattern;
	int i;
	uint16_t displayPin;
	pattern = patterns[digit];
	
	HAL_GPIO_WritePin(GPIOB, segDegree, GPIO_PIN_SET);
	
	/* select which display will be updated according to the systick */
	switch (digitToDisplay) {
		case 0:
			displayPin = sel1;
			HAL_GPIO_WritePin(GPIOB, segDP, GPIO_PIN_RESET);
		break;
		case 1:
			displayPin = sel2;
			HAL_GPIO_WritePin(GPIOB, segDP, GPIO_PIN_SET);
		break;
		case 2:
			displayPin = sel3;
			HAL_GPIO_WritePin(GPIOB, segDP, GPIO_PIN_RESET);
	}
	
	HAL_GPIO_WritePin(GPIOB, displayPin, GPIO_PIN_SET);
	
	
	/* update the 7-segment based on the digit value and the preset pattern */
	for (i = 0; i < 7; i++) {
		if ((pattern & (1 << (6 - i))) >> (6 - i) == 1) {
			HAL_GPIO_WritePin(GPIOB, segments[i], GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, segments[i], GPIO_PIN_RESET);
		}
	}
	HAL_GPIO_WritePin(GPIOB, displayPin, GPIO_PIN_RESET);
}
