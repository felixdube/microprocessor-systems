/**
  ******************************************************************************
  * File Name          : ADC_config.h
  * Description        : Header for ADC_config.c
  * Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
  * Version            : 1.0.0 
  * Date							 : February, 2016
  ******************************************************************************
  */
	
	/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_CONFIG_H
#define __ADC_CONFIG_H

/* Exported global variables ------------------------------------------------------- */
extern ADC_HandleTypeDef ADC1_Handle;


void ADC1_Config(void);

#endif /* __ADC_CONFIG_H */

