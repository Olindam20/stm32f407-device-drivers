/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Sep 15, 2026
 *      Author: OLINDAM SARMA
 */

#include "stm32f407xx_spi_driver.h"

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
    //enable the peripheral clock
    SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);
    

    // Write your code here
    uint32_t tempreg = 0;

    //1. Configure the device mode (Master/Slave)
    tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2; // Set the MSTR bit in CR1
    
    //2. Configure the bus configuration (Full/Half/Simplex)
    if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FULL_DUPLEX)
    {
        // Clear BIDIMODE bit for full-duplex
        tempreg &= ~(1 << 15);
    }
    else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HALF_DUPLEX)
    {
        // Set BIDIMODE bit for half-duplex
        tempreg |= (1 << 15);
    }
    else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX)
    {
        // Clear BIDIMODE bit and set RXONLY bit for simplex receive-only mode
        tempreg &= ~(1 << 15);
        tempreg |= (1 << 10);
    }

    //3. Configure the serial clock speed (Baud Rate)
    tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << 3;    

    //4. Configure the data frame format (DFF)
    tempreg |= pSPIHandle->SPIConfig.SPI_DFF << 11; // Set DFF bit in CR1
    
    //5. Configure the clock polarity (CPOL)
    tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << 1; // Set CPOL bit in CR1  

    //6. Configure the clock phase (CPHA)
    tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << 0; // Set CPHA bit in CR1

    //7. Configure the software slave management (SSM)  
    tempreg |= pSPIHandle->SPIConfig.SPI_SSM << 9; // Set SSM bit in CR1

    //8. Write the configuration to the CR1 register
    pSPIHandle->pSPIx->CR1 = tempreg;
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
    if(pSPIx==SPI1)
    {
        RCC->APB2RSTR |= (1<<12); // Set the reset bit for SPI1
        RCC->APB2RSTR &= ~(1<<12); // Clear the reset bit for SPI1
    }
    else if(pSPIx==SPI2)
    {
        RCC->APB1RSTR |= (1<<14); // Set the reset bit for SPI2
        RCC->APB1RSTR &= ~(1<<14); // Clear the reset bit for SPI2
    }
    else if(pSPIx==SPI3)
    { 
        RCC->APB1RSTR |= (1<<15); // Set the reset bit for SPI3
        RCC->APB1RSTR &= ~(1<<15); // Clear the reset bit for SPI3
    }
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
    if(EnOrDi == ENABLE)
    {
        pSPIx->CR1 |= (1 << 6); // Set SPE bit to enable SPI
    }
    else
    {
        pSPIx->CR1 &= ~(1 << 6); // Clear SPE bit to disable SPI
    }
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
    if(EnOrDi == ENABLE)
    {
        pSPIx->CR1 |= (1 << 8); // Set SSI bit
    }
    else
    {
        pSPIx->CR1 &= ~(1 << 8); // Clear SSI bit
    }
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
    if(EnOrDi == ENABLE)
    {
        pSPIx->CR2 |= (1 << 2); // Set SSOE bit
    }
    else
    {
        pSPIx->CR2 &= ~(1 << 2); // Clear SSOE bit
    }
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
    while(Len>0)
    {
        //1.Wait until TXE is set (Transmit buffer empty)
        while(!(pSPIx->SR & (1 << 1))); // Wait until TXE flag is set

        //2.Check the DFF bit in CR1 to determine 8-bit or 16-bit data frame
        if(pSPIx->CR1 & (1 << 11)) // DFF = 1, 16-bit data frame
        {
            // Load 16 bits of data into the data register
            pSPIx->DR = *((uint16_t*)pTxBuffer); // Typecast
            Len -= 2; // Decrement length by 2 bytes
            pTxBuffer += 2; // Move the buffer pointer by 2 bytes       
        }
        else // DFF = 0, 8-bit data frame
        {
            // Load 8 bits of data into the data register
            pSPIx->DR = *pTxBuffer; // Load 1 byte
            Len--; // Decrement length by 1 byte
            pTxBuffer++; // Move the buffer pointer by 1 byte
        }
    }
    return ;
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
    while(Len>0)
    {
        //1.Wait until RXNE is set (Receive buffer not empty)
        while(!(pSPIx->SR & (1 << 0))); // Wait until RXNE flag is set

        //2.Check the DFF bit in CR1 to determine 8-bit or 16-bit data frame
        if(pSPIx->CR1 & (1 << 11)) // DFF = 1, 16-bit data frame
        {
            // Read 16 bits of data from the data register
            *((uint16_t*)pRxBuffer) = pSPIx->DR; // Typecast
            Len -= 2; // Decrement length by 2 bytes
            pRxBuffer += 2; // Move the buffer pointer by 2 bytes       
        }
        else // DFF = 0, 8-bit data frame
        {
            // Read 8 bits of data from the data register
            *pRxBuffer = pSPIx->DR; // Read 1 byte
            Len--; // Decrement length by 1 byte
            pRxBuffer++; // Move the buffer pointer by 1 byte
        }
    }
    return;
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
    if(EnOrDi==ENABLE)
    {
        if(IRQNumber <= 31)
        {
            *NVIC_ISER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64)
        {
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96)
        {
            *NVIC_ISER2 |= (1 << (IRQNumber % 64));
        }
    }
    else
    {
        if(IRQNumber <= 31)
        {
            *NVIC_ICER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64)
        {
            *NVIC_ICER1 |= (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96)
        {
            *NVIC_ICER2 |= (1 << (IRQNumber % 64));
        }
    }
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
    uint8_t iprx = IRQNumber / 4; // Determine which IPR register to use
    uint8_t iprx_section = IRQNumber % 4; // Determine which section within the IPR register to use
    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED); // Calculate the shift amount based on implemented priority bits
    uint32_t priority_mask = (0xFF << shift_amount); // Create a mask for the priority bits
    *(NVIC_IPR_BASEADDR + iprx) |= (IRQPriority << shift_amount); // Set the priority in the appropriate IPR register   

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
    uint8_t temp1, temp2;
    //first check TXE flag
    temp1 = pHandle->pSPIx->SR & (1 << 1);
    temp2 = pHandle->pSPIx->CR2 & (1 << 7);
    if(temp1 && temp2)
    {
        //handle TXE
        spi_txe_interrupt_handle(pHandle);
    }

    //check RXNE flag
    temp1 = pHandle->pSPIx->SR & (1 << 0);
    temp2 = pHandle->pSPIx->CR2 & (1 << 6);
    if(temp1 && temp2)  
    {
        //handle RXNE
        spi_rxne_interrupt_handle(pHandle);
    }

    //check for overrun error
    temp1 = pHandle->pSPIx->SR & (1 << 6);
    temp2 = pHandle->pSPIx->CR2 & (1 << 5);
    if(temp1 && temp2)
    {
        //handle overrun error
        spi_ovr_err_interrupt_handle(pHandle);
    }

    
}
/*Interrupt based Send and Receive data*/
//Send data using interrupt
uint8_t SPI_SendDataIT(SPI_Handle_t *pHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    uint8_t state = pHandle->TxState;
    if(state != SPI_BUSY_IN_TX)
    {
        // Write your code here
        //1. Save the Tx buffer address and length in the SPI handle structure
        pHandle->pTxBuffer = pTxBuffer;
        pHandle->TxLen = Len;

        //2. Mark the SPI state as busy in transmission so that no other code can use it
        pHandle->TxState = SPI_BUSY_IN_TX;

        //3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
        pHandle->pSPIx->CR2 |= (1 << 7); // Set TXEIE bit in CR2

       
    }
     //4. The rest of the data transmission will be handled in the ISR when TXE flag is set
     return state;
}

uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    uint8_t state = pHandle->RxState;
    if(state != SPI_BUSY_IN_RX)
    {
        // Write your code here
        //1. Save the Rx buffer address and length in the SPI handle structure
        pHandle->pRxBuffer = pRxBuffer;
        pHandle->RxLen = Len;

        //2. Mark the SPI state as busy in reception so that no other code can use it
        pHandle->RxState = SPI_BUSY_IN_RX;

        //3. Enable the RXNEIE control bit to get interrupt whenever RXNE flag is set in SR
        pHandle->pSPIx->CR2 |= (1 << 6); // Set RXNEIE bit in CR2

       
    }
     //4. The rest of the data reception will be handled in the ISR when RXNE flag is set
     return state;
   
}
