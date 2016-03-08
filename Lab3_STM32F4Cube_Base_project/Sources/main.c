/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program subroutine
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February 2016
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "accelerometer.h"
#include "segment_controller.h"
#include "system_clock.h"
#include "kalmanFilter.h"
#include "keypad.h"
#include "lcd.h"
#include "main.h"

/* initialize variables */
kalmanState *xState;
kalmanState *yState;
kalmanState *zState;
int debounce = 0;
int keyLock = 1;
char input_Keypad;
int flag_accPin = 0;
int system_State = startState;
char input_angle[5] = "000";
float input_angle_float = 0;
float display_number = 0;
int input_count = 0;
float delta_angle = 100; // not initialized at 0 to avoid direct win
int clear_display = 0;


/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);

int main(void)
{
  /* MCU Configuration----------------------------------------------------------*/

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize accelerometer */
  Accelerometer_Config();
  Accelerometer_GPIO_Config();
  Accelerometer_Interrupt_Config();

  /* Initialize 7-segment display */
  Display_GPIO_Config();
  Display_TIM_Config();

	/* Initialize Keypad*/
	Keypad_Config();
	
	initLCD(); 															/* configure LCD */
	clearDisplay();
	returnHome(); 													/* just makes sure that start writing at the right place */
	LCD_WriteString("     Start"); 			/* The 2 initial space are for centering */
  while (1){

		
		
		
		//start state, just display angle and wait for enter
		while (system_State == startState) {
			if(flag_accPin){
				ReadAcc();
				flag_accPin = 0;
			}
			if (displayTimer) {
					display(pitch);
					displayTimer = 0;
				}

			//if keypad interrupt ...
			input_Keypad = readKeypad();
//			if (input_Keypad != 'n') {
//				printf("%c\n",input_Keypad);
//			}
			if (input_Keypad == '#') {
				system_State = inputState;
			}
		}

		//input destination angle and press enter
		if (system_State == inputState){
			clearDisplay();
			returnHome(); 													/* just makes sure that start writing at the right place */
			LCD_WriteString("   Input Angle"); 						/* The 2 initial space are for centering */
		}
		while (system_State == inputState) {
			if (displayTimer) {
					display(input_angle_float);
					displayTimer = 0;
				}
			//if keypad interrupt ...
			input_Keypad = readKeypad();
			if (input_Keypad == '#') {
				if(input_angle_float <= 180){
					system_State = moveState;
				}
				else {
				clearDisplay();
				returnHome(); 													/* just makes sure that start writing at the right place */
				LCD_WriteString("Max Angle = 180"); 						/* The 2 initial space are for centering */
				}
			}
			else if (input_Keypad != 'n'  && input_Keypad != '*') {
				input_angle[input_count] = input_Keypad;
				input_count++;
				input_angle_float = atof(input_angle);
				//printf("%c\n",input_Keypad);
				//printf("%f\n",input_angle_float);
				if (input_count >= 3){
					input_count = 0;
				}
			}
		}
		//try to position the board at the right angle
		while (system_State == moveState) {
			if(flag_accPin){
				ReadAcc();
				flag_accPin = 0;
			}

			if (displayTimer) {
					display(pitch);
					displayTimer = 0;
				}

//			//if keypad interrupt ...
//			input_Keypad = readKeypad();
//			if (input_Keypad == '#') {
//				system_State = startState;
//			}


			//visual feedback for helping the user orient the board
			delta_angle = input_angle_float - pitch;

			if(abs(delta_angle) <= 5) {
				system_State = endState;
			}
			else if (delta_angle > 0) {
				//display something on LCD
				if(clear_display == 0){
					clearDisplay();
					clear_display = 1;
					returnHome(); 													/* just makes sure that start writing at the right place */
					LCD_WriteString("     Go up"); 						/* The 2 initial space are for centering */
				}
			}
			else {
				//display something on LCD
				if(clear_display == 0){
					clearDisplay();
					clear_display = 1;
					returnHome(); 													/* just makes sure that start writing at the right place */
					LCD_WriteString("    Go down"); 						/* The 2 initial space are for centering */
				}

			}


		}

		//display win message
		if (system_State == endState){
			clear_display = 0;
			if(clear_display == 0){
				clearDisplay();
				clear_display = 1;
			}
			clearDisplay();
			returnHome(); 													/* just makes sure that start writing at the right place */
			LCD_WriteString("   Bravo Harsh"); 						/* The 2 initial space are for centering */
		}
		while (system_State == endState) {
			if (displayTimer) {
					display(input_angle_float);
					displayTimer = 0;
					//printf("endState\n");
				}

			//returnHome(); 													/* just makes sure that start writing at the right place */
			//LCD_WriteString("WIN"); 			/* The 2 initial space are for centering */
		}

		//safety if the state gets weird
		if(system_State != 0 && system_State != 1 && system_State != 2 && system_State != 3){
				system_State = 0;
		}
  }
}



/**
  * @brief  Callback from the Timers
	* @param  Timer handler
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
		digitTimer++;
		displayTimer = 1;
}

/**
  * @brief  Callback from the external GPIO interupt
	* @param  GPIO_Pin: pin on with the interupt occurs
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == accPin) {
			flag_accPin = 1;
		}
}
