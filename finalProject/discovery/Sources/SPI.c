/*******************************************************************************
  * @file    SPI.c
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
  * @version 1.0.0
  * @date    16-March-2016
  * @brief   implement SPI communication protocols
  ******************************************************************************
  */
 
#include "SPI.h"

SPI_HandleTypeDef SpiHandle;
  
void SPI_Init_Master(void) {
  __HAL_RCC_SPI2_CLK_ENABLE();
  
	HAL_SPI_DeInit(&SpiHandle);
  SpiHandle.Instance 							  = SPI2;
  SpiHandle.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_4;
  SpiHandle.Init.Direction 					= SPI_DIRECTION_2LINES;
  SpiHandle.Init.CLKPhase 					= SPI_PHASE_1EDGE;
  SpiHandle.Init.CLKPolarity 				= SPI_POLARITY_LOW;
  SpiHandle.Init.CRCCalculation			= SPI_CRCCALCULATION_DISABLED;
  SpiHandle.Init.CRCPolynomial 			= 7;
  SpiHandle.Init.DataSize 					= SPI_DATASIZE_8BIT;
  SpiHandle.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
  SpiHandle.Init.NSS 								= SPI_NSS_SOFT;
  SpiHandle.Init.TIMode 						= SPI_TIMODE_DISABLED;
  SpiHandle.Init.Mode 							= SPI_MODE_MASTER;
	if (HAL_SPI_Init(&SpiHandle) != HAL_OK) {printf ("ERROR: Error in initialising SPI1 \n");};
  
	__HAL_SPI_ENABLE(&SpiHandle);
  
  SPI_MspInit();
}

void SPI_MspInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the SPI periph */
  __SPI2_CLK_ENABLE();

  /* Enable NSS, SCK, MOSI and MISO GPIO clocks */
  __GPIOB_CLK_ENABLE();

  GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.Pin = SPI2_SCK;
  HAL_GPIO_Init(SPI2_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.Pin =  SPI2_MOSI;
  HAL_GPIO_Init(SPI2_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.Pin = SPI2_MISO;
  HAL_GPIO_Init(SPI2_PORT, &GPIO_InitStructure);

	/* SPI NSS pin configuration */
  GPIO_InitStructure.Pin = SPI2_NSS;
  HAL_GPIO_Init(SPI2_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  HAL_GPIO_WritePin(SPI2_PORT, SPI2_NSS, GPIO_PIN_SET);
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value
  */
uint8_t Exchange_Byte(uint8_t byte) {
  /* Loop while DR register in not empty */
  uint32_t timeout = BYTE_EXCHANGE_FLAG_TIMEOUT;
  while (__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE) == RESET) {
    if((timeout--) == 0) return (uint8_t) 0;
  }

  /* Send a Byte through the SPI peripheral */
  SPI_SendData(&SpiHandle,  byte);

  /* Wait to receive a Byte */
  timeout = BYTE_EXCHANGE_FLAG_TIMEOUT;
  while (__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE) == RESET) {
    if((timeout--) == 0) {return (uint8_t) 1;}
  }

  /* Return the Byte read from the SPI bus */ 
  return SPI_ReceiveData(&SpiHandle);
}

/**
  * @brief  Transmits a Data through the SPIx/I2Sx peripheral.
  * @param  *hspi: Pointer to the SPI handle. Its member Instance can point to either SPI1, SPI2 or SPI3 
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void SPI_SendData(SPI_HandleTypeDef *hspi, uint16_t Data) { 
  /* Write in the DR register the data to be sent */
  hspi->Instance->DR = Data;
}

/**
  * @brief  Returns the most recent received data by the SPIx/I2Sx peripheral. 
  * @param  *hspi: Pointer to the SPI handle. Its member Instance can point to either SPI1, SPI2 or SPI3 
  * @retval The value of the received data.
  */
uint8_t SPI_ReceiveData(SPI_HandleTypeDef *hspi) {
  /* Return the data in the DR register */
  return hspi->Instance->DR;
}
