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
int system_State = 0;

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
	
  
  while (1){
		if (system_State == startState)
		
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
		
  }
}



#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line){
}
#endif
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
