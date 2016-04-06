/*******************************************************************************
  * @file    communicate.h
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
  * @version 1.0.0
  * @date    16-March-2016
  * @brief   implement interboard communication
  ******************************************************************************
  */

#ifndef COMMUNICATE_H
#define COMMUNICATE_H

/* variables to receive */
extern volatile float temperature;
extern volatile float pitchAngle;
extern volatile float rollAngle;
extern volatile int doubleTap;

/* variables to transmit */
extern volatile int LED_pattern;
extern volatile int LED_brightness;
extern volatile int LED_speed;
extern volatile int LED_PWM_duty_cycle;

void InitCommunication(void);
void communicate(void);
float receiveFloat(void);

#endif
