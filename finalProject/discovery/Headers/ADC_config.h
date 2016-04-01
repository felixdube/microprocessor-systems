/**
  ******************************************************************************
	* @file    ADC_config.h
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
	* @version 1.0.0
  * @date    February-2016
  * @brief   Configure adc for temperature monitoring
  ******************************************************************************
  */
	
#ifndef __ADC_CONFIG_H
#define __ADC_CONFIG_H

/* Exported global variables ------------------------------------------------------- */
extern ADC_HandleTypeDef ADC1_Handle;

void ADC1_Config(void);

#endif /* __ADC_CONFIG_H */
