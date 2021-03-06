/**
  ******************************************************************************
  * File Name          : lcd.h
  * Description        : Header for lcd.c
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */

#ifndef LCD_H
#define LCD_H

#define RS 	GPIO_PIN_2
#define RW 	GPIO_PIN_4
#define E 	GPIO_PIN_5
#define DB0 GPIO_PIN_6
#define DB1 GPIO_PIN_7
#define DB2 GPIO_PIN_8
#define DB3 GPIO_PIN_9
#define DB4 GPIO_PIN_10
#define DB5 GPIO_PIN_11
#define DB6 GPIO_PIN_12
#define DB7 GPIO_PIN_13

#define MAX_NUMBER_CHAR_LCD_DISPLAY 16
#define ENABLE_TIME 50 						/* in us, don't set below ~50 or weird timing issue with display */
#define WAIT_TIME_AFTER_RESET 1000 	/* in us, don't set below ~1000 with */

/* Exported variables ------------------------------------------------------- */
extern volatile int lcdtimer;

void initLCD(void);
void enable(void);
void LCD_WriteChar(char c);
void LCD_WriteString(char * string);
void clearDisplay(void);
void returnHome(void);
void inputMode(void);
void turnOn(void);
void functionSet(void);
void entryMode(void);
void SetAdress(int address);

#endif
