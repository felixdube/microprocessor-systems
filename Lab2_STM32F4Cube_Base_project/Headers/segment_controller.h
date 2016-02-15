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

#define segA GPIO_PIN_0
#define segB GPIO_PIN_1
#define segC GPIO_PIN_2
#define segD GPIO_PIN_4
#define segE GPIO_PIN_5
#define segF GPIO_PIN_7
#define segG GPIO_PIN_8
#define segDP GPIO_PIN_11
#define segDegree GPIO_PIN_12
#define sel1 GPIO_PIN_13
#define sel2 GPIO_PIN_14
#define sel3 GPIO_PIN_15

#define ZERO  (uint8_t)0x7E
#define ONE   (uint8_t)0x30
#define TWO   (uint8_t)0x6D
#define THREE (uint8_t)0x79
#define FOUR  (uint8_t)0x33
#define FIVE  (uint8_t)0x5B
#define SIX   (uint8_t)0x5F
#define SEVEN (uint8_t)0x70
#define EIGHT (uint8_t)0x7F
#define NINE  (uint8_t)0x7B

extern volatile int displayTick;


void display(float value);
int getDigit(float value, int place);
void setPins(int number);

void Display_GPIO_Config(void);

#endif
