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
#define EMPTY_BYTE                 ((uint8_t)0x00)

#define SPI2_NSS GPIO_PIN_12
#define SPI2_SCK GPIO_PIN_13
#define SPI2_MISO GPIO_PIN_14
#define SPI2_MOSI GPIO_PIN_15

#define SPI2_PORT GPIOB

void SPI_Init_Master(void);
void SPI_MspInit(void);
void SPI_SendData(SPI_HandleTypeDef *hspi, uint16_t Data);
uint8_t SPI_ReceiveData(SPI_HandleTypeDef *hspi);
uint8_t Exchange_Byte(uint8_t byte);

#endif
