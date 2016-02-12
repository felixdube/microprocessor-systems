/**
  ******************************************************************************
  * File Name          : segment_controller.h
  * Description        : Allow to display numbers on 4 7-segments displays
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */
	
	
#ifndef SEGMENT_CONTROLLER_H
#define SEGMENT_CONTROLLER_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "stm32f4xx_hal_gpio.h"

#define segA GPIO_PIN_0
#define segB GPIO_PIN_1
#define segC GPIO_PIN_2
#define segD GPIO_PIN_4
#define segE GPIO_PIN_5
#define segF GPIO_PIN_7
#define segG GPIO_PIN_8
#define segDP GPIO_PIN_11
#define sel1 GPIO_PIN_12
#define sel2 GPIO_PIN_13
#define sel3 GPIO_PIN_14
#define sel4 GPIO_PIN_15

#define ZERO  {1,1,1,1,1,1,0}
#define ONE   {0,1,1,0,0,0,0}
#define TWO   {1,1,0,1,1,0,1}
#define THREE {1,1,1,1,0,0,1}
#define FOUR  {0,1,1,0,0,1,1}
#define FIVE  {1,0,1,1,0,1,1}
#define SIX   {1,0,1,1,1,1,1}
#define SEVEN {1,1,1,0,0,0,0}
#define EIGHT {1,1,1,1,1,1,1}
#define NINE  {1,1,1,1,0,1,1}

extern volatile int displayTick;


void display(float value);
int getDigit(float value, int place);
void setPins(int number);

void Display_GPIO_Config(void);

#endif
