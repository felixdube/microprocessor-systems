/**
  ******************************************************************************
  * File Name          : alarm.h
  * Description        : header file for alarm.c
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */

#ifndef ALARM_H
#define ALARM_H

/* The maximum temperature for the processor is 85 C for minimum power dissipation
 * and 105 C for max power dissipation */
#define THRESHHOLD_TEMP 42.5f
#define DISPLAY_TIME_1_LED_ALARM 80000

#define LED1 GPIO_PIN_13
#define LED2 GPIO_PIN_14
#define LED3 GPIO_PIN_15
#define LED4 GPIO_PIN_12

void Alarm_GPIO_Config(void);

extern volatile int alarmLedTimer;
void trigger_alarm(void);
void shutoff_alarm(void);

#endif
