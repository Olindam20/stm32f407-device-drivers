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
    uint8_t *pTxBuffer;           /* Pointer to transmit buffer */
    uint8_t *pRxBuffer;           /* Pointer to receive buffer */
    uint32_t TxLen;               /* Length of data to transmit */
    uint32_t RxLen;               /* Length of data to receive */
    uint8_t TxState;              /* Transmission state */
    uint8_t RxState;              /* Reception state */
}SPI_Handle_t;

/*@SPI_DeviceMode*/
#define SPI_DEVICE_MODE_MASTER 1
#define SPI_DEVICE_MODE_SLAVE  0
/*@SPI_BusConfig*/
#define SPI_BUS_CONFIG_FULL_DUPLEX 1
#define SPI_BUS_CONFIG_HALF_DUPLEX 0
#define SPI_BUS_CONFIG_SIMPLEX     2
/*@SPI_DFF*/
#define SPI_DFF_8_BIT  0
#define SPI_DFF_16_BIT 1
/*@SPI_CPOL*/
#define SPI_CPOL_LOW  0
#define SPI_CPOL_HIGH 1
/*@SPI_CPHA*/
#define SPI_CPHA_FIRST_EDGE 0
#define SPI_CPHA_SECOND_EDGE 1
/*@SPI_SSM*/
#define SPI_SSM_ENABLE  1
#define SPI_SSM_DISABLE 0
/*@SPI_SSI*/
#define SPI_SSI_ENABLE  1
#define SPI_SSI_DISABLE 0
/*@SPI_SclkSpeed*/
#define SPI_SCLK_SPEED_DIV2   0
#define SPI_SCLK_SPEED_DIV4   1
#define SPI_SCLK_SPEED_DIV8   2
#define SPI_SCLK_SPEED_DIV16  3
#define SPI_SCLK_SPEED_DIV32  4
#define SPI_SCLK_SPEED_DIV64  5
#define SPI_SCLK_SPEED_DIV128 6
#define SPI_SCLK_SPEED_DIV256 7
/*@SPI applciation states*/
#define SPI_READY         0
#define SPI_BUSY_IN_RX    1
#define SPI_BUSY_IN_TX    2

/* @SPI_APPLICATION_EVENTS */
#define SPI_EVENT_TX_CMPLT   1
#define SPI_EVENT_RX_CMPLT   2
#define SPI_EVENT_OVR_ERR    3
#define SPI_EVENT_CRC_ERR    4

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

/* Peripheral Control & Pin Configurations */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

/* Blocking Data Send and Receive */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/* Non-blocking / Interrupt-based Data Send and Receive */
uint8_t SPI_SendDataIT(SPI_Handle_t *pHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pHandle, uint8_t *pRxBuffer, uint32_t Len);

/* IRQ Configuration and ISR Handling */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/* Other Peripheral Control APIs */
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmission(SPI_Handle_t *pHandle);
void SPI_CloseReception(SPI_Handle_t *pHandle);

/* Application Callback */
void SPI_ApplicationEventCallback(SPI_Handle_t *pHandle, uint8_t AppEv);

#endif /* INC_STM32F4XX_SPI_DRIVER_H_ */
