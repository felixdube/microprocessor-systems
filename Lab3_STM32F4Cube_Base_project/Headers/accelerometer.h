/**
  ******************************************************************************
  * File Name          : accelerometer.h
  * Description        : Header for accelerometer.c
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */
	
	
#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#define accPin GPIO_PIN_0

#define PI 3.1415926535

extern float accValue[3];
extern float pitch;

void Accelerometer_Config(void);
void Accelerometer_Interrupt_Config(void);
void Accelerometer_GPIO_Config(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void EXTI0_IRQHandler (void);



#endif
