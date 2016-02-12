/**
  ******************************************************************************
  * File Name          : temperature.c
  * Description        : Implement methods for working with the internal temperature sensor
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */

#include "temperature.h"

float convertTemp(int adc_output) {
	float voltage = adc_output / 4096.0 * VREF;
	float temp = 400 * voltage - 279;
	return temp;
}
