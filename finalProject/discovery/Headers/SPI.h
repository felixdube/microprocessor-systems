/*******************************************************************************
  * @file    SPI.h
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
  * @version 1.0.0
  * @date    16-March-2016
  * @brief   implement SPI communication protocols
  ******************************************************************************
  */

#ifndef SPI_H
#define SPI_H

#include "stm32f4xx_hal.h"

#define BYTE_EXCHANGE_FLAG_TIMEOUT         ((uint32_t)0x1000)

void SPI_Init_Master(void);
void SPI_SendData(SPI_HandleTypeDef *hspi, uint16_t Data);
uint8_t SPI_ReceiveData(SPI_HandleTypeDef *hspi);
static uint8_t Exchange_Byte(uint8_t byte);

#endif
