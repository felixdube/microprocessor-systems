/**
  ******************************************************************************
	* @file    temperature.h
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    February-2016
  * @brief   Implement methods for working with the internal temperature sensor
  ******************************************************************************
  */

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#define VREF 3.0
#define MAX_12_BIT 4096.0

float convertTemp(int adc_output);

#endif
