/**
  ******************************************************************************
  * @file    Thread_SPI.h
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
  * @version 1.0.0
  * @date    16-March-2016
  * @brief   Implement a thread for communicating with nucleo board
  ******************************************************************************
  */

#ifndef THREAD_SPI_H
#define THREAD_SPI_H

#include "cmsis_os.h"

#define COM_DELAY 100           /* communication delay in ms */
#define BYTE_DELAY 1           /* byte delay in ms */

extern osThreadId tid_Thread_SPI;

/* varaibles needing to be sent */
extern volatile float temperature;
extern volatile float pitchAngle;
extern volatile float rollAngle;
extern volatile int doubleTapZ;
extern volatile int doubleTapY;
extern volatile int doubleTapX;

/* variables needing to be received */
extern volatile int LED_pattern;
extern volatile int LED_brightness;
extern volatile int LED_speed;

int start_Thread_SPI(void);
void Thread_SPI(void const *argument);
void transmitFloat(float f);

#endif
