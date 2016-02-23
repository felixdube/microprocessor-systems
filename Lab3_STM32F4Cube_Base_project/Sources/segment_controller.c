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

volatile int digitTimer = 0;
volatile int displayTimer = 1;
const uint8_t patterns[10] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
const int segments[7] = {segA, segB, segC, segD, segE, segF, segG};

/* Initialize struct */
TIM_Base_InitTypeDef TIM3_InitDef;
TIM_HandleTypeDef TIM3_HandleDef;

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

/* Display timer configuration */
void Display_TIM_Config(void) {
	/* Enable clock for TIM3 */
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	TIM3_InitDef.Prescaler = 84;
	TIM3_InitDef.Period = 100;
	TIM3_InitDef.CounterMode = TIM_COUNTERMODE_DOWN;
	
	TIM3_HandleDef.Instance = TIM3;
	TIM3_HandleDef.Init = TIM3_InitDef;
	
	HAL_TIM_Base_Start_IT(&TIM3_HandleDef);
	
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 2, 0);
}

void TIM3_IRQHandler(void) {
	digitTimer++;
	displayTimer = 1;
	HAL_TIM_IRQHandler(&TIM3_HandleDef);
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
	int i;
	uint8_t pattern;
	pattern = patterns[digit];
	
	HAL_GPIO_WritePin(GPIOB, segDegree, GPIO_PIN_SET);
	
	/* select which display will be updated according to the systick */
	switch (digitTimer) {
		case 0:
			HAL_GPIO_WritePin(GPIOB, sel1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, sel2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, sel3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, segDP, GPIO_PIN_RESET);
		break;
		case 1:
			HAL_GPIO_WritePin(GPIOB, sel1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, sel2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, sel3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, segDP, GPIO_PIN_SET);
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
