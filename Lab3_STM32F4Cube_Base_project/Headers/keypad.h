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

/* from top view of the keypad, the pins are labeled 1 to 8 from left to right
hence pin 8 is unoccupied
*/

#define col1 GPIO_PIN_1 //pin 6 on keypad
#define col2 GPIO_PIN_2 //pin 7 on keypad
#define col3 GPIO_PIN_4 //pin 1 on keypad
#define row1 GPIO_PIN_5 //pin 2 on keypad
#define row2 GPIO_PIN_6 //pin 3 on keypad
#define row3 GPIO_PIN_8 //pin 4 on keypad
#define row4 GPIO_PIN_9 //pin 5 on keypad

extern int debounce;
extern int keyLock;

char convertToChar(int col, int row);
void Keypad_Config(void);
int findRow(void);
char readKeypad(void);



#endif

