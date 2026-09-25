/*
 * stm32f407xx_usart_driver.c
 *
 *  Created on: Sep 25, 2026
 *      Author: OLINDAM SARMA
 */

#include "stm32f407xx_usart_driver.h"

/*********************************************************************
 * @fn                - USART_PeriClockControl
 *
 * @brief             - Enables or disables peripheral clock for the given USART peripheral
 *
 * @param[in]         - pUSARTx: Base address of the USART peripheral
 * @param[in]         - EnorDi: ENABLE or DISABLE
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
    // Write your code here
    if(EnorDi == ENABLE)
    {
        if(pUSARTx == USART1)
        {
            RCC_USART1_CLK_EN();
        }
        else if(pUSARTx == USART2)
        {
            RCC_USART2_CLK_EN();
        }
        else if(pUSARTx == USART3)
        {
            RCC_USART3_CLK_EN();
        }
        else if(pUSARTx == USART5)
        {
            RCC_USART5_CLK_EN();
        }
        else if(pUSARTx == USART6)
        {
            RCC_USART6_CLK_EN();
        }
    }
    else
    {
        if(pUSARTx == USART1)
        {
            RCC_USART1_CLK_DI();
        }
        else if(pUSARTx == USART2)
        {
            RCC_USART2_CLK_DI();
        }
        else if(pUSARTx == USART3)
        {
            RCC_USART3_CLK_DI();
        }
    }
}

/*********************************************************************
 * @fn                - USART_Init
 *
 * @brief             - Initializes the given USART peripheral according to configuration parameters
 *
 * @param[in]         - pUSARTHandle: Pointer to USART handle structure
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void USART_Init(USART_Handle_t *pUSARTHandle)
{
   
    //peripheral clock enable
    USART_PeripheralControl(pUSARTHandle->pUSARTx, ENABLE);

     // Write your code here
    uint32_t tempreg = 0;

    //mode of operation
    if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
    {
        tempreg |= (1 << 3); // Set TE bit in CR1 for transmitter  
    }
    else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
    {
        tempreg |= (1 << 2); // Set RE bit in CR1 for receiver
    }
    else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
    {
        tempreg |= (1 << 3); // Set TE bit in CR1 for transmitter
        tempreg |= (1 << 2); // Set RE bit in CR1 for receiver
    }
}

/*********************************************************************
 * @fn                - USART_DeInit
 *
 * @brief             - Resets all registers of the given USART peripheral to their default reset values
 *
 * @param[in]         - pUSARTx: Base address of the USART peripheral
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void USART_DeInit(USART_RegDef_t *pUSARTx)
{
    // Write your code here
}

/*********************************************************************
 * @fn                - USART_PeripheralControl
 *
 * @brief             - Enables or disables the USART peripheral (UE bit)
 *
 * @param[in]         - pUSARTx: Base address of the USART peripheral
 * @param[in]         - EnorDi: ENABLE or DISABLE
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
    // Write your code here
}

/*********************************************************************
 * @fn                - USART_GetFlagStatus
 *
 * @brief             - Returns the status of the specified USART status flag
 *
 * @param[in]         - pUSARTx: Base address of the USART peripheral
 * @param[in]         - StatusFlagName: Flag name/mask to check
 *
 * @return            - FLAG_SET or FLAG_RESET
 *
 * @Note              - None
 *********************************************************************/
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName)
{
    // Write your code here
    return 0;
}

/*********************************************************************
 * @fn                - USART_ClearFlag
 *
 * @brief             - Clears the specified USART status flag
 *
 * @param[in]         - pUSARTx: Base address of the USART peripheral
 * @param[in]         - StatusFlagName: Flag name/mask to clear
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName)
{
    // Write your code here
}

/*********************************************************************
 * @fn                - USART_IRQInterruptConfig
 *
 * @brief             - Enables or disables the given IRQ number in ARM Cortex-M4 NVIC
 *
 * @param[in]         - IRQNumber: NVIC IRQ position number
 * @param[in]         - EnorDi: ENABLE or DISABLE
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    // Write your code here
}

/*********************************************************************
 * @fn                - USART_IRQPriorityConfig
 *
 * @brief             - Sets the priority for the specified IRQ number in NVIC
 *
 * @param[in]         - IRQNumber: NVIC IRQ position number
 * @param[in]         - IRQPriority: Priority level (0 to 15)
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    // Write your code here
}
