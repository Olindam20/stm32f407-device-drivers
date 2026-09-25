/*
 * stm32f407xx_usart_driver.h
 *
 *  Created on: Sep 25, 2026
 *      Author: OLINDAM SARMA
 */

#ifndef INC_STM32F407XX_USART_DRIVER_H_
#define INC_STM32F407XX_USART_DRIVER_H_



#include "stm32f407xx.h"

typedef struct
{
    uint8_t USART_Mode; // Possible values from @USART_MODE
    uint32_t USART_Baud; // Possible values from @USART_BAUD
    uint8_t USART_NoOfStopBits; // Possible values from @USART_STOPBITS
    uint8_t USART_WordLength; // Possible values from @USART_WORDLEN
    uint8_t USART_ParityControl; // Possible values from @USART_PARITY
    uint8_t USART_HWFlowControl; // Possible values from @USART_HWFLOW
}USART_Config_t;

typedef struct
{
    USART_RegDef_t *pUSARTx; // Pointer to the USART peripheral
    USART_Config_t USART_Config; // USART configuration settings
}USART_Handle_t;


/*
 * ====================================================================
 *                 APIs supported by this USART driver
 * ====================================================================
 */

/* Peripheral Clock Setup */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);

/* Init and De-Init */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_RegDef_t *pUSARTx);

/* Data Send and Receive */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/* Other Peripheral Control APIs */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);

/* IRQ Configuration and Priority Setup */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

#endif /* INC_STM32F407XX_USART_DRIVER_H_ */
