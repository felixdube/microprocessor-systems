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

/* Array to update the values of the pins easier in loops */
const int dataPin[8] =	{DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7};

volatile int lcdtimer;

/**
	* @brief initializes trhe GPIOs for the LCD and clean up the display
	* @param None
	* @retval None
	*/
void initLCD(void){
	GPIO_InitTypeDef GPIO_InitDef; 										/* Initialize struct */
	__HAL_RCC_GPIOE_CLK_ENABLE(); 										/* Enable clock for GPOIE */
	GPIO_InitDef.Pin = RS | RW | E | DB0 | DB1 | DB2 | DB3 | DB4 | DB5 | DB6 | DB7; /* All will have same mode */
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   				/* push pull */
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;			/* max frequency for our processor is 84MHz */
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef);
	clearDisplay();
	turnOn();
	functionSet();
	entryMode();
}

/**
	* @brief Toggle the enable pin to "send" command to display
	* @param None
	* @retval None
	*/
void enable(void){
	HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_SET);
	while (lcdtimer < ENABLE_TIME){/* delay for synchronization */}			
	lcdtimer = 0;
	HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
}

/**
	* @brief set the data pin to a particuliar character
  * @param c: char to be written on the LCD
	* @retval None
	*/
void LCD_WriteChar(char c){
	int i;
	for (i = 0; i < 8; i++) {
		/* extracts a bit of the char and checks if it's 1 to set the line to high */
		if ((c & (1 << i)) >> i == 1) {
			HAL_GPIO_WritePin(GPIOE, dataPin[i], GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOE, dataPin[i], GPIO_PIN_RESET);
		}
	}
	enable();
}

/**
	* @brief send one by one all the character of the string to LCD_WriteChar()
  * @param string: String to be printed on the LCD
	* @retval None
	*/
void LCD_WriteString(char * string){
	int i;
	inputMode(); /************ NOT sure we need this */
	/* Extract each char individually and calls LCD_WriteChar for each one */
	/* until it hits the MAX_NUMBER_CHAR_LCD_DISPLAY or the end char \0 */
	for (i = 0; (i < MAX_NUMBER_CHAR_LCD_DISPLAY) && (string[i] != '\0') ; i++){
		LCD_WriteChar(string[i]);
	}
}

/**
	* @brief Clear every character on the display and returns to the first address of display
	* @param None
	* @retval None
	*/
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
	/* extra wait time from the datasheet 1.53ms */
	while (lcdtimer < WAIT_TIME_AFTER_RESET){/* delay for synchronization */}	
	lcdtimer = 0;
}

/**
	* @brief set address of the LCD display to address
  * @param address: Address of in the memory of the LCD RAM to read or write next character
	*				 beware of heaxedecimal to int conversion
	* @retval None
	*	@note When 1-line display mode (N=LOW), DDRAM address is form 0x00 to 0x4F
	* 			In 2-line display mode (N=HIGH), DDRAM address is form 0x00 to 0x27 for first line
	*				and 0x40 to 0x67 for second line
	*/
void SetAdress(int address){
  int i;
	HAL_GPIO_WritePin(GPIOE, RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, RW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB7, GPIO_PIN_SET);
	/* set DB0 to DB6 to the address of the DDRAM */
	for (i = 0; i < 7; i++) {
		if ((address & (1 << i)) >> i == 1) {
			HAL_GPIO_WritePin(GPIOE, dataPin[i], GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOE, dataPin[i], GPIO_PIN_RESET);
		}
	}
	enable();
}

/**
	* @brief change the address of the LCD pointer to the first one
	* @param None
	* @retval None
	*/
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
	/* extra wait time from the datasheet 1.53ms */
	while (lcdtimer < WAIT_TIME_AFTER_RESET){/* delay for synchronization */}
	lcdtimer = 0;	
}

/**
	* @brief set up pins to input mode
	* @param None
	* @retval None
	*/
void inputMode(void){
	HAL_GPIO_WritePin(GPIOE, RS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, RW, GPIO_PIN_RESET);
}
	
/**
	* @brief Turn on the display
	* @param None
	* @retval None
	*/
void turnOn(void){
	HAL_GPIO_WritePin(GPIOE, RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, RW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, DB2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, DB1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DB0, GPIO_PIN_RESET);
	enable();
}

/**
	* @brief Set up length of data bus, and size of display
	* @param None
	* @retval None
	*/
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


/**
	* @brief set up the entry mode of the display
	* @param None
	* @retval None
	*/
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
