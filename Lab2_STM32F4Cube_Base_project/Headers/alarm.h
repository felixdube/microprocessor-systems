/**
  ******************************************************************************
  * File Name          : alarm.h
  * Description        : Implement methods for temperature alarm
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */

#ifndef ALARM_H
#define ALARM_H

#define THRESHHOLD_TEMP 34.5f

#define LED1 GPIO_PIN_13
#define LED2 GPIO_PIN_14
#define LED3 GPIO_PIN_15
#define LED4 GPIO_PIN_12

void Alarm_GPIO_Config(void);

extern volatile int alarmTick;
void trigger_alarm(void);
void shutoff_alarm(void);

#endif
