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

#include "kalmanFilter.h"

#define accPin GPIO_PIN_0

#define PI 3.1415926535

extern float accValue[3];
extern float pitch;
extern kalmanState *xState;
extern kalmanState *yState;
extern kalmanState *zState;

void Accelerometer_Config(void);
void Accelerometer_Interrupt_Config(void);
void Accelerometer_GPIO_Config(void);
void EXTI0_IRQHandler (void);
float calcPitch (float x, float y, float z);



#endif
