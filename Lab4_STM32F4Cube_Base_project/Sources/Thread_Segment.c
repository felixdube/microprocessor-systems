/**
  ******************************************************************************
	* @file    Thread_Segment.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    16-March-2016
  * @brief   Implement a thread for 7-segment operation
  ******************************************************************************
  */

#include "Thread_Segment.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "segment_controller.h"
#include "accelerometer.h"
#include "Thread_Temp.h"

volatile float display_value = 0;
volatile int degree_on = 1;
volatile int flash_alarm = 0;
volatile int flash_alarm_urgent = 0;
volatile int stateAngleTemp = 0; //state of the angle temp selection 0 being angle
volatile int stateRollPitch = 0; //state of the roll pitch selection 0 being pitch

int updateDisplayDataCounter = 0;

osThreadId tid_Thread_Segment;                              // thread id
osThreadDef(Thread_Segment, osPriorityHigh, 1, 0);

/**
	* @brief Start 7-segment display thread
	* @param none
	* @retval none
	*/
int start_Thread_Segment(void) {
	Display_GPIO_Config();
	display_degree(degree_on);
  tid_Thread_Segment = osThreadCreate(osThread(Thread_Segment), NULL);
  if (!tid_Thread_Segment) return -1;
	return 0;
}

/**
	* @brief 7-segment display thread
	* @param none
	* @retval none
	*/
void Thread_Segment(void const *argument) {

	while(1) {
		if (flash_alarm_urgent) {
			flash_segment(FLASH_TOTAL_PERIOD_URGENT);
		}
		else if (flash_alarm) {
			flash_segment(FLASH_TOTAL_PERIOD);
		}
		else {
			__HAL_RCC_GPIOB_CLK_ENABLE();
		}

		if (updateDisplayDataCounter >= UPDATE_DATA_DELAY){
			updateDisplayDataCounter = 0;
			if(stateAngleTemp == ANGLE){
				if(stateRollPitch == PITCH){
					display_value = pitchAngle;
				}
				else if(stateRollPitch == ROLL){
					display_value = rollAngle;
				}
			}
			else if(stateAngleTemp == TEMP){
				display_value = temperature;
			}
		}
		else{
			updateDisplayDataCounter++;
		}
		display(display_value);
		printf("%f\n",display_value);
		digitTimer++;
		osDelay(SEGMENT_DELAY);
	}
}
