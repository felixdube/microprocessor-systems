/*************************************************************************************/
// File: temperature.c
// Description: Implement methods for working with the internal temperature sensor
// Date: February, 2016
// Author: August Lalande, Juan Morency Trudel, Felix Dube
/*************************************************************************************/

#include "temperature.h"

int convertTemp(int adc_output) {
	float voltage = adc_output / 4096 * VREF;
	float temp = 400 * voltage - 279;
	return temp;
}
