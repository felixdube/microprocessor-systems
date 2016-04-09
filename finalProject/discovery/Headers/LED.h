/*******************************************************************************
  * @file    LED.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    8-April-2016
  * @brief    header for LED.c
  ******************************************************************************
  */



#ifndef LED_H
#define LED_H

#define LED_SPEED_DIVIDER 40

#define LED1 GPIO_PIN_13
#define LED2 GPIO_PIN_14
#define LED3 GPIO_PIN_15
#define LED4 GPIO_PIN_12

extern volatile int alarmLedTimer;

void LED_GPIO_Config(void);
void LED_TIM_Config(void);
void LED_PWM_Config(void);
void Rotate_LED(int speed);
void LED_Off(void);
void LED_On(void);
void Change_LED_Brightness(int brightness);

#endif
