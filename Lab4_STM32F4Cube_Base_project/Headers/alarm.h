/**
  ******************************************************************************
	* @file    alarm.h
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    February-2016
  * @brief   Implements a basic LED alarm
  ******************************************************************************
  */

#ifndef ALARM_H
#define ALARM_H

/* The maximum temperature for the processor is 85 C for minimum power dissipation
 * and 105 C for max power dissipation */
#define THRESHHOLD_TEMP 25

#define LED1 GPIO_PIN_13
#define LED2 GPIO_PIN_14
#define LED3 GPIO_PIN_15
#define LED4 GPIO_PIN_12

void Alarm_GPIO_Config(void);
void trigger_alarm(int LED);
void shutoff_alarm(void);

#endif
