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

extern volatile float display_value;
extern volatile int degree_on;
extern volatile int flash_alarm;

int start_Thread_Segment(void);
void Thread_Segment(void const *argument);

#endif
