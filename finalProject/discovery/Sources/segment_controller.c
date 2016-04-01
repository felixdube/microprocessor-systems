/**
  ******************************************************************************
	* @file    segment_controller.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    February-2016
  * @brief   Implements methods for controlling a multiplexed 7-segment display array
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "segment_controller.h"

volatile int digitTimer = 0;			// timer to change the digit to be updated
int tmp;													// temp value to be displayed
int dotPosition = 1;							// decimal position
int flash = 0;										// timer for flashing a digit on the 7-segment display
const uint8_t patterns[10] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
const int segments[7] = {segA, segB, segC, segD, segE, segF, segG};
int flash_counter = 0;


/**
	* @brief GPIO for 7-segments display init
	* @param None
	* @retval None
	*/
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
	digitTimer %= 3;
	setPins(getDigit(value, digitTimer));
}

/**
	* @brief Calculate what is the digit to be display on a particuliar display for a particuliar value
	* @param value: value to be displayed
	* @param place: for which display the digitvalue is being calculated
	* @retval value of the digit
	*/
int getDigit(float value, int place) {
	if(value >= 100){
		tmp = (int) (value);
		dotPosition = 2;
	}
	else if(value >= 10){
		tmp = (int) (value * 10);
		dotPosition = 1;
	}
	else {
		tmp = (int) (value * 100);
		dotPosition = 0;
	}

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
	int i;
	uint8_t pattern;
	pattern = patterns[digit];

	/* select which display will be updated according to the systick */
	switch (digitTimer) {
		case 0:
			HAL_GPIO_WritePin(GPIOB, sel1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, sel2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, sel3, GPIO_PIN_RESET);
			if (dotPosition == 0){
				HAL_GPIO_WritePin(GPIOB, segDP, GPIO_PIN_SET);
			}
			else {
				HAL_GPIO_WritePin(GPIOB, segDP, GPIO_PIN_RESET);
			}
		break;
		case 1:
			HAL_GPIO_WritePin(GPIOB, sel1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, sel2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, sel3, GPIO_PIN_RESET);
			if (dotPosition == 1){
				HAL_GPIO_WritePin(GPIOB, segDP, GPIO_PIN_SET);
			}
			else {
				HAL_GPIO_WritePin(GPIOB, segDP, GPIO_PIN_RESET);
			}
		break;
		case 2:
			HAL_GPIO_WritePin(GPIOB, sel1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, sel2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, sel3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, segDP, GPIO_PIN_RESET);
	}

	/* update the 7-segment based on the digit value and the preset pattern */
	for (i = 0; i < 7; i++) {
		if ((pattern & (1 << (6 - i))) >> (6 - i) == 1) {
			HAL_GPIO_WritePin(GPIOB, segments[i], GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, segments[i], GPIO_PIN_RESET);
		}
	}
}


/**
	* @brief Turn on and off the 7-segment display with a specific period
	* @param flas_period: flasshing period of the display
	* @retval None
	*/
void flash_segment(int flash_period) {
	if (flash_counter < flash_period / 3){
		HAL_GPIO_WritePin(GPIOB, sel1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, sel2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, sel3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, segDP, GPIO_PIN_RESET);
		__HAL_RCC_GPIOB_CLK_DISABLE();
	}
	else {
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}

	flash_counter++;
	flash_counter %= flash_period;
}


/**
	* @brief Turn on or off the degree sign on the display
	* @param on: 1 means to turn it on, 0 to turn it off
	* @retval None
	*/
void display_degree(int on) {
	if (on) {
		HAL_GPIO_WritePin(GPIOB, segDegree, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOB, segDegree, GPIO_PIN_RESET);
	}
}
