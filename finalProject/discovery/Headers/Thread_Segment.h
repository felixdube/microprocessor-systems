/**
  ******************************************************************************
  * @file    Thread_Segment.h
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
  * @version 1.0.0
  * @date    16-March-2016
  * @brief   Implement a thread for 7-segment operation
  ******************************************************************************
  */

#ifndef THREAD_SEGMENT_H
#define THREAD_SEGMENT_H

#include "cmsis_os.h"


// above 4 we get noticable flickering
#define SEGMENT_DELAY 3         /* delay (ms) to call the segment display method */

#define UPDATE_DATA_DELAY 50    /* delay update the value to display. Multiply by SEGMENT_DELAY for ms value */

#define ROLL 0
#define PITCH 1
#define ANGLE 0
#define TEMP 1

extern osThreadId tid_Thread_Segment;

extern volatile float display_value;
extern volatile int degree_on;
extern volatile int flash_alarm;
extern volatile int stateAngleTemp;
extern volatile int stateRollPitch;

int start_Thread_Segment(void);
void Thread_Segment(void const *argument);

#endif
