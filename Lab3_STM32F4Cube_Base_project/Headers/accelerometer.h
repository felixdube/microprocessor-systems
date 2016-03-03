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

#define cal_X11 0.00101149016032271
#define cal_X12 -2.18101855048941e-05
#define cal_X13 9.29325513401108e-06
#define cal_X21 -2.00644110592771e-05
#define cal_X22 0.000959476077374820
#define cal_X23 -5.02476404512506e-06
#define cal_X31 4.15188334780502e-07
#define cal_X32 6.24569851893158e-06
#define cal_X33 0.000991368587396580
#define cal_X41 -0.0214607657991569
#define cal_X42 -0.00989724403866105
#define cal_X43 -0.0246509358276086


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
void Calibrate(float* out);



#endif
