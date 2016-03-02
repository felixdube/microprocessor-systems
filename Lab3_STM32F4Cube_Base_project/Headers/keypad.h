/**
  ******************************************************************************
  * File Name          : keypad.h
  * Description        : header file for keypad.c
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
#ifndef KEYPAD_H
#define KEYPAD_H

#define col1 GPIO_PIN_1
#define col2 GPIO_PIN_2
#define col3 GPIO_PIN_4
#define row1 GPIO_PIN_5
#define row2 GPIO_PIN_6
#define row3 GPIO_PIN_8
#define row4 GPIO_PIN_9


char convertToChar(int col, int row);
void Keypad_Config(void);
int findRow(void);



#endif

