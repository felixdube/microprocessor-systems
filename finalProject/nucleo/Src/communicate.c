/*******************************************************************************
  * @file    communicate.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
  * @version 1.0.0
  * @date    16-March-2016
  * @brief   implement interboard communication
  ******************************************************************************
  */

#include <string.h>

#include "SPI.h"
#include "communicate.h"

/* variables to receive */
volatile float temperature = 0;
volatile float pitchAngle = 0;
volatile float rollAngle = 0;
volatile int doubleTap = 0;

/* variables to transmit */
volatile int LED_pattern = 4;
volatile int LED_brightness = 5;
volatile int LED_speed = 6;
volatile int LED_PWM_duty_cycle = 7;

void InitCommunication(void) {
	SPI_Init_Slave();
}

void communicate(void) {
	while (Exchange_Byte(EMPTY_BYTE) != START_BYTE);
  
	/* receive */
	temperature = receiveFloat();
	pitchAngle = receiveFloat();
	rollAngle = receiveFloat();
	doubleTap = Exchange_Byte(EMPTY_BYTE);
	
	/* transmit */
	Exchange_Byte((uint8_t) LED_pattern);
	Exchange_Byte((uint8_t) LED_brightness);
	Exchange_Byte((uint8_t) LED_speed);
	Exchange_Byte((uint8_t) LED_PWM_duty_cycle);
	
	//printf("t: %f, p: %f, r: %f, d: %d\n", temperature, pitchAngle, rollAngle, doubleTap);
}

float receiveFloat(void) {
	char data[sizeof(float)];
	data[0] = Exchange_Byte(EMPTY_BYTE);
	data[1] = Exchange_Byte(EMPTY_BYTE);
	data[2] = Exchange_Byte(EMPTY_BYTE);
	data[3] = Exchange_Byte(EMPTY_BYTE);
	
	float f;
	memcpy(&f, data, sizeof f);
	return f;
}
