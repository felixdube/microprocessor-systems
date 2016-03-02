/**
  ******************************************************************************
  * File Name          : keypad.c
  * Description        : Function for initialisation and control of the keypad
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February 2016
  ******************************************************************************
  */
	
#include "keypad.h"
#include "stm32f4xx_hal.h"

/* Initialize struct */
GPIO_InitTypeDef Row_GPIO_InitDef;
GPIO_InitTypeDef Column_GPIO_InitDef;
	
/* keypad configuration */
void Keypad_Config(void) {
	
	/* ROW */
	/* Enable clock for GPOIB */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	 
	/* All will have same mode */
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
	 
	/* All will have same mode */
	Column_GPIO_InitDef.Pin = col1 | col2 | col3;
	Column_GPIO_InitDef.Mode = GPIO_MODE_IT_FALLING;   			/* INPUT */
	Column_GPIO_InitDef.Pull = GPIO_PULLUP;
	Column_GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		/* max frequency for our processor is 84MHz */
	 
	HAL_GPIO_Init(GPIOC, &Column_GPIO_InitDef);
	
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 2);
	
//	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
//	HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 2);
//	
//	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
//	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 2);
}

int findRow(void){
	int count = 0;
	int row = 0;

	Column_GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   
	Column_GPIO_InitDef.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOC, &Column_GPIO_InitDef);
	
	Row_GPIO_InitDef.Mode = GPIO_MODE_INPUT;
	Row_GPIO_InitDef.Pull = GPIO_PULLUP;
	
	HAL_GPIO_Init(GPIOC, &Row_GPIO_InitDef);

	HAL_GPIO_WritePin(GPIOC, col1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, col2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, col3, GPIO_PIN_RESET);
	
	
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
	
	Row_GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	Row_GPIO_InitDef.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOC, &Row_GPIO_InitDef);
	
	HAL_GPIO_WritePin(GPIOC, row1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, row2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, row3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, row4, GPIO_PIN_RESET);

	Column_GPIO_InitDef.Mode = GPIO_MODE_IT_FALLING;   
	Column_GPIO_InitDef.Pull = GPIO_PULLUP;
	
	HAL_GPIO_Init(GPIOC, &Column_GPIO_InitDef);
	
	return row;
}

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

