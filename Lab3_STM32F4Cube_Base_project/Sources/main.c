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


	
/* initialize variables */
kalmanState *xState;
kalmanState *yState;
kalmanState *zState;
int debounce = 0;
int keyLock = 1;

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
	
	if (displayTimer) {
      display(pitch);
      displayTimer = 0;
    }
	readKeypad();
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
	switch (GPIO_Pin) {
		case accPin:
			/* Get values */
			LIS3DSH_ReadACC(accValue);
			Calibrate(accValue);
			
			/* Filter values */
			kalmanUpdate(xState, accValue[0]);
			kalmanUpdate(yState, accValue[1]);
			kalmanUpdate(zState, accValue[2]);
		
			/* DON'T DELETE printf for matlab script */
			//printf("%f,%f,%f,%f,%f,%f\n",accValue[2], zState->q,zState->r, zState->x, zState->p, zState->k);
			/* Calc pitch */
			pitch = calcPitch(xState->x, yState->x, zState->x);
			//printf("%f %f %f pitch: %f\n", xState->x,yState->x,zState->x, pitch);
		  //printf("%f %f %f;\n", xState->x,yState->x,zState->x);
			break;
	}
}


