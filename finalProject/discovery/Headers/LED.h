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

#define FULL_BRIGHTNESS 100
#define ZERO_BRIGHTNESS 0
#define LED1 GPIO_PIN_13
#define LED2 GPIO_PIN_14
#define LED3 GPIO_PIN_15
#define LED4 GPIO_PIN_12


void LED_GPIO_Config(void);
void LED_TIM_Config(void);
void LED_PWM_Config(void);
void Rotate_LED(int speed);
void LED_Off(void);
void LED_On(void);
void Change_PWM(int brightness, int LED_number);
void Adjust_Brightness_Level(int brightness, int LED_number);

#endif
