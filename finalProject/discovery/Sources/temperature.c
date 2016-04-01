/**
  ******************************************************************************
	* @file    temperature.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    February-2016
  * @brief   Implement methods for working with the internal temperature sensor
  ******************************************************************************
  */

#include "temperature.h"

/**
	* @brief Convert the output of the ADC to a temperature in °C
	* @param adc_ouput: value sampled by the ADC
	* @retval temperature: temperature in °C
	*/
float convertTemp(int adc_output) {
	float voltage = VREF * adc_output / MAX_12_BIT;
	float temperature = 400 * voltage - 279;
	return temperature;
}
