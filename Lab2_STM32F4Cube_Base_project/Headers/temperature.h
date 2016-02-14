/**
  ******************************************************************************
  * File Name          : temperature.h
  * Description        : Implement methods for working with the internal temperature sensor
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#define VREF 3.3

float convertTemp(int adc_output);

#endif
