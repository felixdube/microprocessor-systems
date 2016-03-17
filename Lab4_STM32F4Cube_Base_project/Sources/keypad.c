/**
  ******************************************************************************
	* @file    keypad.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    February-2016
  * @brief   Function for initialisation and control of the keypad
  ******************************************************************************
  */
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "keypad.h"

int debounce = 0;												// used to debounced the keypad
int keyLock = 1;												// mutex for keypressed
char charConverted;

/* Initialize struct */
GPIO_InitTypeDef Row_GPIO_InitDef;
GPIO_InitTypeDef Column_GPIO_InitDef;


/* Initialize struct */
TIM_Base_InitTypeDef TIM_keypad_InitDef;
TIM_HandleTypeDef TIM_keypad_HandleDef;

/**
	* @brief Initialise the keypad
	* @param None
	* @retval None
	*/
void Keypad_Config(void) {

	/* Enable clock for GPOIC */
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/* ROW */
	/* output */
	Row_GPIO_InitDef.Pin = row1 | row2 | row3 | row4;
	Row_GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			/* push pull */
	Row_GPIO_InitDef.Pull = GPIO_NOPULL;
	Row_GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		/* max frequency for our processor is 84MHz */

	HAL_GPIO_Init(GPIOC, &Row_GPIO_InitDef);

	HAL_GPIO_WritePin(GPIOC, row1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, row2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, row3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, row4, GPIO_PIN_RESET);

	/* COLUMN */
	/* input */
	Column_GPIO_InitDef.Pin = col1 | col2 | col3;
	Column_GPIO_InitDef.Mode = GPIO_MODE_INPUT;   				/* INPUT */
	Column_GPIO_InitDef.Pull = GPIO_PULLUP;
	Column_GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		/* max frequency for our processor is 84MHz */

	HAL_GPIO_Init(GPIOC, &Column_GPIO_InitDef);
}


/**
	* @brief Check is a button is pressed on the keypad
	* @param None
	* @retval None
	*/
char readKeypad(void) {
	int row = 0;
	int col = 0;

	/* the keyLock prevent a button to be triggered more than once when it is pushed only once */
	if(keyLock){

		/* check if a button is pressed and in which row the button is */
		if (HAL_GPIO_ReadPin(GPIOC, col1) == GPIO_PIN_RESET){
			debounce++;

			/* debounce for button pressed */
			if(debounce > DEBOUNCE_CYCLES){
				keyLock = 0;
				col = 1;
				row = findRow();
				charConverted = convertToChar(col, row);
				//printf("%c\n", a);
				return charConverted;
			}
		}

		else if (HAL_GPIO_ReadPin(GPIOC, col2) == GPIO_PIN_RESET){
			debounce++;

			/* debounce for button pressed */
			if (debounce > DEBOUNCE_CYCLES){
				keyLock = 0;
				col = 2;
				row = findRow();
				charConverted = convertToChar(col, row);
				//printf("%c\n", a);
				return charConverted;
			}
		}

		else if (HAL_GPIO_ReadPin(GPIOC, col3) == GPIO_PIN_RESET){
			debounce++;

			/* debounce for button pressed */
			if (debounce > DEBOUNCE_CYCLES) {
				keyLock = 0;
				col = 3;
				row = findRow();
				charConverted = convertToChar(col, row);
				//printf("%c\n", a);
				return charConverted;
			}
		}
	}
	else{

		/* debounce for when the button is depressed */
		if(	HAL_GPIO_ReadPin(GPIOC, col1) == GPIO_PIN_SET &&
				HAL_GPIO_ReadPin(GPIOC, col2) == GPIO_PIN_SET &&
				HAL_GPIO_ReadPin(GPIOC, col3) == GPIO_PIN_SET){
			debounce--;
		}

		/* unlock the keypad after the button press */
		if(debounce < 0){
			keyLock = 1;
		}
	}
	return 'n';
}

/**
	* @brief Find in which row a button was pressed
	* @param None
	* @retval None
	*/
int findRow(void){
	int row = 0;
	/* change the column at output */
	Column_GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	Column_GPIO_InitDef.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOC, &Column_GPIO_InitDef);


	/* change row as input */
	Row_GPIO_InitDef.Mode = GPIO_MODE_INPUT;
	Row_GPIO_InitDef.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOC, &Row_GPIO_InitDef);

	HAL_GPIO_WritePin(GPIOC, col1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, col2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, col3, GPIO_PIN_RESET);

	/* check which row have a button pressed */
	if (HAL_GPIO_ReadPin(GPIOC, row1) == GPIO_PIN_RESET){
		row = 1;
	}

	else if (HAL_GPIO_ReadPin(GPIOC, row2) == GPIO_PIN_RESET){
		row = 2;
	}

	else if (HAL_GPIO_ReadPin(GPIOC, row3) == GPIO_PIN_RESET){
		row = 3;
	}
	else if (HAL_GPIO_ReadPin(GPIOC, row4) == GPIO_PIN_RESET){
		row = 4;
	}

	/* set back the row and column as output and input */
	Row_GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	Row_GPIO_InitDef.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOC, &Row_GPIO_InitDef);

	HAL_GPIO_WritePin(GPIOC, row1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, row2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, row3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, row4, GPIO_PIN_RESET);

	Column_GPIO_InitDef.Mode = GPIO_MODE_INPUT;
	Column_GPIO_InitDef.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOC, &Column_GPIO_InitDef);

	return row;
}

/**
	* @brief Convert a keypad position to a character
	* @param col: Column of the button
	* @param row: Row of the button
	* @retval character corresponding the the column and row
	*/
char convertToChar(int col, int row){

	switch(col) {
		case 1:
			switch(row){
				case 1:
					return '1';
				case 2:
					return '4';
				case 3:
					return  '7';
				case 4:
					return '*';
			}


		case 2:
			switch(row){
				case 1:
					return '2';
				case 2:
					return '5';
				case 3:
					return '8';
				case 4:
					return '0';
			}


		case 3:
			switch(row){
				case 1:
					return '3';
				case 2:
					return '6';
				case 3:
					return '9';
				case 4:
					return '#';
			}
	}

	return 'E';

}

