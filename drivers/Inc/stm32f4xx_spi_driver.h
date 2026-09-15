/*
 * stm32f4xx_spi_driver.h
 *
 *  Created on: Sep 15, 2026
 *      Author: OLINDAM SARMA
 */

#ifndef INC_STM32F4XX_SPI_DRIVER_H_
#define INC_STM32F4XX_SPI_DRIVER_H_


#include "stm32f407xx.h"

/* SPI configuration structure */
typedef struct
{
    uint8_t SPI_DeviceMode;      /* Device mode: Master or Slave */
    uint8_t SPI_BusConfig;       /* Bus configuration: Full-duplex, Half-duplex, or Simplex */
    uint8_t SPI_SclkSpeed;       /* Serial clock speed */
    uint8_t SPI_DFF;             /* Data frame format: 8-bit or 16-bit */
    uint8_t SPI_CPOL;            /* Clock polarity */
    uint8_t SPI_CPHA;            /* Clock phase */
    uint8_t SPI_SSM;             /* Software slave management */
}SPI_Config_t;

typedef struct
{
    SPI_RegDef_t *pSPIx;         /* Base address of the SPI peripheral */
    SPI_Config_t SPIConfig;      /* SPI configuration settings */
}SPI_Handle_t;


/*
 * ====================================================================
 *                 APIs supported by this SPI driver
 * ====================================================================
*/
/* Peripheral Clock Setup */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

/* Init and De-Init */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);    
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);





#endif /* INC_STM32F4XX_SPI_DRIVER_H_ */
