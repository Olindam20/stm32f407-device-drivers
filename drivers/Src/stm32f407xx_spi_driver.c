/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Sep 15, 2026
 *      Author: OLINDAM SARMA
 */

#include "stm32f4xx_spi_driver.h"

/*********************************************************************
 * @fn                - SPI_PeriClockControl
 *
 * @brief             - Enables or disables peripheral clock for the given SPI peripheral
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral (SPI1, SPI2, SPI3)
 * @param[in]         - EnOrDi: ENABLE or DISABLE macros
 *
 * @return            - None
 *
 * @Note              - SPI1 is on APB2, SPI2/SPI3 are on APB1
 *********************************************************************/
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
    // Write your code here
    if(EnOrDi == ENABLE)
    {
        if(pSPIx == SPI1)
        {
            RCC_SPI1_CLK_EN();
        }
        else if(pSPIx == SPI2)
        {
            RCC_SPI2_CLK_EN();
        }
        else if(pSPIx == SPI3)
        {
            RCC_SPI3_CLK_EN();
        }
    }
    else
    {
        if(pSPIx == SPI1)
        {
            RCC_SPI1_CLK_DI();
        }
        else if(pSPIx == SPI2)
        {
            RCC_SPI2_CLK_DI();
        }
        else if(pSPIx == SPI3)
        {
            RCC_SPI3_CLK_DI();
        }
    }
}


/*********************************************************************
 * @fn                - SPI_Init
 *
 * @brief             - Initializes the SPI peripheral registers (CR1, CR2) according to the configuration
 *
 * @param[in]         - pSPIHandle: Pointer to SPI handle structure containing SPI base address & configuration
 *
 * @return            - None
 *
 * @Note              - Configures Device Mode, Bus Config, DFF, CPOL, CPHA, SSM, and Baud Rate
 *********************************************************************/
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
    // Write your code here
    

}


/*********************************************************************
 * @fn                - SPI_DeInit
 *
 * @brief             - Resets all registers of the given SPI peripheral back to default reset state
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 *
 * @return            - None
 *
 * @Note              - Uses RCC APB1RSTR or APB2RSTR registers
 *********************************************************************/
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
    // Write your code here
}


/*********************************************************************
 * @fn                - SPI_PeripheralControl
 *
 * @brief             - Enables or disables the SPI peripheral (SPE bit in CR1)
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 * @param[in]         - EnOrDi: ENABLE or DISABLE
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
    // Write your code here
}


/*********************************************************************
 * @fn                - SPI_SSIConfig
 *
 * @brief             - Configures the Internal Slave Select (SSI) bit in CR1 when SSM is enabled
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 * @param[in]         - EnOrDi: ENABLE or DISABLE
 *
 * @return            - None
 *
 * @Note              - Avoids MODF (Mode Fault) error in Master mode when SSM = 1
 *********************************************************************/
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
    // Write your code here
}


/*********************************************************************
 * @fn                - SPI_SSOEConfig
 *
 * @brief             - Enables or disables Slave Select Output Enable (SSOE) in CR2
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 * @param[in]         - EnOrDi: ENABLE or DISABLE
 *
 * @return            - None
 *
 * @Note              - When enabled, NSS pin is automatically pulled LOW when SPE = 1
 *********************************************************************/
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
    // Write your code here
}


/*********************************************************************
 * @fn                - SPI_SendData
 *
 * @brief             - Transmits data over SPI (Blocking / Polling based)
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 * @param[in]         - pTxBuffer: Pointer to transmission data buffer
 * @param[in]         - Len: Number of bytes to transmit
 *
 * @return            - None
 *
 * @Note              - Handles both 8-bit and 16-bit DFF formats
 *********************************************************************/
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
    // Write your code here
}


/*********************************************************************
 * @fn                - SPI_ReceiveData
 *
 * @brief             - Receives data over SPI (Blocking / Polling based)
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 * @param[out]        - pRxBuffer: Pointer to buffer where received data will be stored
 * @param[in]         - Len: Number of bytes to receive
 *
 * @return            - None
 *
 * @Note              - Handles both 8-bit and 16-bit DFF formats
 *********************************************************************/
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
    // Write your code here
}


/*********************************************************************
 * @fn                - SPI_IRQInterruptConfig
 *
 * @brief             - Enables or disables the given IRQ number in ARM Cortex-M4 NVIC
 *
 * @param[in]         - IRQNumber: NVIC IRQ position number
 * @param[in]         - EnOrDi: ENABLE or DISABLE
 *
 * @return            - None
 *
 * @Note              - Configures NVIC ISER / ICER registers
 *********************************************************************/
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi)
{
    // Write your code here
}


/*********************************************************************
 * @fn                - SPI_IRQPriorityConfig
 *
 * @brief             - Sets the priority for the specified IRQ number in NVIC
 *
 * @param[in]         - IRQNumber: NVIC IRQ position number
 * @param[in]         - IRQPriority: Priority level (0 to 15)
 *
 * @return            - None
 *
 * @Note              - Configures NVIC IPR registers (upper 4 bits)
 *********************************************************************/
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    // Write your code here
}


/*********************************************************************
 * @fn                - SPI_IRQHandling
 *
 * @brief             - Handles SPI interrupt events (TXE, RXNE, ERR)
 *
 * @param[in]         - pHandle: Pointer to SPI handle structure
 *
 * @return            - None
 *
 * @Note              - Dispatches to internal ISR helper functions
 *********************************************************************/
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
    // Write your code here
}