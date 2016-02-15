/**
  ******************************************************************************
  * File Name          : lcd.c
  * Description        : Implement methods for controlling the lcd
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "lcd.h"

const int dataPin[8] =	{DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7};


void initLCD(void){
	
	//Initialize struct
	GPIO_InitTypeDef GPIO_InitDef;
	
	//Enable clock for GPOIE
	__HAL_RCC_GPIOE_CLK_ENABLE();
	 
	//All will have same mode
	GPIO_InitDef.Pin = RS | RW | E | DB0 | DB1 | DB2 | DB3 | DB4 | DB5 | DB6 | DB7;
	//Mode output
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   //push pull
	//Without pull resistors
	GPIO_InitDef.Pull = GPIO_NOPULL;
	//pin speed
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;// max frequency for our processor is 84MHz
	 
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef);
	
	functionSet();
	turnOn();
	entryMode();
	clearDisplay();
	HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
	
	
}

void enable(void){
	HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
}

void LCD_WriteChar(char c){
	int i;
	for (i = 0; i < 8; i++) {
		if ((c & (1 << i)) >> i == 1) {
			HAL_GPIO_WritePin(GPIOE, dataPin[i], GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOE, dataPin[i], GPIO_PIN_RESET);
		}
	}
	enable();
}

void LCD_WriteString(char * string){
	int i;
	turnOn();
	clearDisplay();
	returnHome();
	inputMode();
	printf("%s\n",string);
	for (i = 0; i < 4; i++){
		LCD_WriteChar(string[i]);
	}
}

void clearDisplay(void){
	HAL_GPIO_WritePin(GPIOE, RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, RW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB0, GPIO_PIN_SET);

	enable();
}

void returnHome(void){
	HAL_GPIO_WritePin(GPIOE, RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, RW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, DB0, GPIO_PIN_RESET);

	enable();
}

void inputMode(void){
	HAL_GPIO_WritePin(GPIOE, RS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, RW, GPIO_PIN_RESET);
}
	
void turnOn(void){
	HAL_GPIO_WritePin(GPIOE, RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, RW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, DB2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, DB1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, DB0, GPIO_PIN_SET);

	enable();
}

void functionSet(void){
	HAL_GPIO_WritePin(GPIOE, RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, RW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, DB4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, DB3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, DB2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB0, GPIO_PIN_RESET);

	enable();
}

void entryMode(void){
	HAL_GPIO_WritePin(GPIOE, RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, RW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, DB1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, DB0, GPIO_PIN_RESET);

	enable();
}
