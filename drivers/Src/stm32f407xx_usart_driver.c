/*
 * stm32f407xx_usart_driver.c
 *
 *  Created on: Sep 25, 2026
 *      Author: OLINDAM SARMA
 */

#include "stm32f407xx_usart_driver.h"

static const uint16_t AHB_PreScaler[8]  = {2, 4, 8, 16, 64, 128, 256, 512};
static const uint8_t  APB1_PreScaler[4] = {2, 4, 8, 16};
static const uint8_t  APB2_PreScaler[4] = {2, 4, 8, 16};

static uint32_t RCC_GetPCLK1Value(void)
{
    uint32_t pclk1, SystemClk;
    uint8_t clksrc, temp, ahbp, apb1p;

    clksrc = (RCC->CFGR >> 2) & 0x3;
    if(clksrc == 0)
    {
        SystemClk = 16000000;
    }
    else if(clksrc == 1)
    {
        SystemClk = 8000000;
    }
    else
    {
        SystemClk = 16000000;
    }

    temp = (RCC->CFGR >> 4) & 0xF;
    if(temp < 8)
    {
        ahbp = 1;
    }
    else
    {
        ahbp = AHB_PreScaler[temp - 8];
    }

    temp = (RCC->CFGR >> 10) & 0x7;
    if(temp < 4)
    {
        apb1p = 1;
    }
    else
    {
        apb1p = APB1_PreScaler[temp - 4];
    }

    pclk1 = (SystemClk / ahbp) / apb1p;
    return pclk1;
}

static uint32_t RCC_GetPCLK2Value(void)
{
    uint32_t pclk2, SystemClk;
    uint8_t clksrc, temp, ahbp, apb2p;

    clksrc = (RCC->CFGR >> 2) & 0x3;
    if(clksrc == 0)
    {
        SystemClk = 16000000;
    }
    else if(clksrc == 1)
    {
        SystemClk = 8000000;
    }
    else
    {
        SystemClk = 16000000;
    }

    temp = (RCC->CFGR >> 4) & 0xF;
    if(temp < 8)
    {
        ahbp = 1;
    }
    else
    {
        ahbp = AHB_PreScaler[temp - 8];
    }

    temp = (RCC->CFGR >> 13) & 0x7;
    if(temp < 4)
    {
        apb2p = 1;
    }
    else
    {
        apb2p = APB2_PreScaler[temp - 4];
    }

    pclk2 = (SystemClk / ahbp) / apb2p;
    return pclk2;
}

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
    // Peripheral clock enable
    USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

    uint32_t tempreg = 0;

    /******************************** Configuration of CR1 *********************************/

    // 1. Configure the Mode of operation (TX, RX, or TXRX)
    if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
    {
        tempreg |= (1 << USART_CR1_TE);
    }
    else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
    {
        tempreg |= (1 << USART_CR1_RE);
    }
    else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
    {
        tempreg |= ((1 << USART_CR1_TE) | (1 << USART_CR1_RE));
    }

    // 2. Configure Word Length
    if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
    {
        tempreg |= (1 << USART_CR1_M);
    }
    else if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_8BITS)
    {
        tempreg &= ~(1 << USART_CR1_M);
    }       

    // 3. Configure Parity Control
    if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
    {
        tempreg |= (1 << USART_CR1_PCE);
        tempreg &= ~(1 << USART_CR1_PS); // 0 = Even parity
    }
    else if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD)
    {
        tempreg |= (1 << USART_CR1_PCE);
        tempreg |= (1 << USART_CR1_PS);  // 1 = Odd parity
    }
    else if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
    {
        tempreg &= ~(1 << USART_CR1_PCE);
    }

    // Program the CR1 register
    pUSARTHandle->pUSARTx->CR1 = tempreg;

    /******************************** Configuration of CR2 *********************************/
    tempreg = 0;

    // Configure number of STOP bits
    if(pUSARTHandle->USART_Config.USART_NoOfStopBits == USART_STOPBITS_1)
    {
        tempreg &= ~(0x3 << USART_CR2_STOP); // 00: 1 Stop bit
    }
    else if(pUSARTHandle->USART_Config.USART_NoOfStopBits == USART_STOPBITS_0_5)
    {
        tempreg |= (0x1 << USART_CR2_STOP);  // 01: 0.5 Stop bit
    }
    else if(pUSARTHandle->USART_Config.USART_NoOfStopBits == USART_STOPBITS_2)
    {
        tempreg |= (0x2 << USART_CR2_STOP);  // 10: 2 Stop bits
    }   
    else if(pUSARTHandle->USART_Config.USART_NoOfStopBits == USART_STOPBITS_1_5)
    {
        tempreg |= (0x3 << USART_CR2_STOP);  // 11: 1.5 Stop bits
    }

    // Program the CR2 register
    pUSARTHandle->pUSARTx->CR2 = tempreg;

    /******************************** Configuration of CR3 *********************************/
    tempreg = 0;

    // Configure Hardware Flow Control
    if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_NONE)
    {
        tempreg &= ~(1 << USART_CR3_CTSE);
        tempreg &= ~(1 << USART_CR3_RTSE);
    }
    else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
    {
        tempreg |= (1 << USART_CR3_CTSE);
        tempreg &= ~(1 << USART_CR3_RTSE);
    }
    else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
    {
        tempreg |= (1 << USART_CR3_RTSE);
        tempreg &= ~(1 << USART_CR3_CTSE);
    }
    else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
    {
        tempreg |= (1 << USART_CR3_CTSE);
        tempreg |= (1 << USART_CR3_RTSE);
    }
    
    // Program the CR3 register
    pUSARTHandle->pUSARTx->CR3 = tempreg;

    /******************************** Configuration of Baud Rate *********************************/
    USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);
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
    if(pUSARTx == USART1)
    {
        // Reset USART1 peripheral
        RCC->APB2RSTR |= (1 << 4); // Set the reset bit for USART1
        RCC->APB2RSTR &= ~(1 << 4); // Clear the reset bit for USART1
    }
    else if(pUSARTx == USART2)
    {
        // Reset USART2 peripheral
        RCC->APB1RSTR |= (1 << 17); // Set the reset bit for USART2
        RCC->APB1RSTR &= ~(1 << 17); // Clear the reset bit for USART2
    }
    else if(pUSARTx == USART3)
    {
        // Reset USART3 peripheral
        RCC->APB1RSTR |= (1 << 18); // Set the reset bit for USART3
        RCC->APB1RSTR &= ~(1 << 18); // Clear the reset bit for USART3
    }
    else if(pUSARTx == USART5)
    {
        // Reset USART5 peripheral
        RCC->APB1RSTR |= (1 << 20); // Set the reset bit for USART5
        RCC->APB1RSTR &= ~(1 << 20); // Clear the reset bit for USART5
    }
    else if(pUSARTx == USART6)
    {
        // Reset USART6 peripheral 
        RCC->APB2RSTR |= (1 << 5); // Set the reset bit for USART6
        RCC->APB2RSTR &= ~(1 << 5); // Clear the reset bit for USART6
    }
}

/*********************************************************************
 * @fn                - USART_SendData
 *
 * @brief             - Transmits data over USART in blocking (polling) mode
 *
 * @param[in]         - pUSARTHandle: Pointer to USART handle structure
 * @param[in]         - pTxBuffer: Pointer to transmit data buffer
 * @param[in]         - Len: Number of bytes to transmit
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    // Write your code here
    for(uint32_t i = 0; i < Len; i++)
    {
        // Wait until TXE (Transmit Data Register Empty) flag is set
        while(!(pUSARTHandle->pUSARTx->SR & USART_FLAG_TXE));

        // Check word length and parity settings
        if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            // 9-bit word length
            pUSARTHandle->pUSARTx->DR = (*((uint16_t*)pTxBuffer) & (uint16_t)0x01FF); // Mask to 9 bits
            pTxBuffer += 2; // Move to next 2 bytes in buffer
        }
        else
        {
            // 8-bit word length
            pUSARTHandle->pUSARTx->DR = (*pTxBuffer & (uint8_t)0xFF); // Mask to 8 bits
            pTxBuffer++; // Move to next byte in buffer
        }
    }

    //wait until TC (Transmission Complete) flag is set
    while(!(pUSARTHandle->pUSARTx->SR & USART_FLAG_TC));
}

/*********************************************************************
 * @fn                - USART_ReceiveData
 *
 * @brief             - Receives data over USART in blocking (polling) mode
 *
 * @param[in]         - pUSARTHandle: Pointer to USART handle structure
 * @param[out]        - pRxBuffer: Pointer to receive data buffer
 * @param[in]         - Len: Number of bytes to receive
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    // Write your code here
}

/*********************************************************************
 * @fn                - USART_SendDataIT
 *
 * @brief             - Initiates non-blocking data transmission over USART using interrupts
 *
 * @param[in]         - pUSARTHandle: Pointer to USART handle structure
 * @param[in]         - pTxBuffer: Pointer to transmit data buffer
 * @param[in]         - Len: Number of bytes to transmit
 *
 * @return            - State of USART Tx (e.g. USART_BUSY_IN_TX, USART_READY)
 *
 * @Note              - None
 *********************************************************************/
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    uint8_t txstate = pUSARTHandle->TxBusyState;

    // Write your code here

    return txstate;
}

/*********************************************************************
 * @fn                - USART_ReceiveDataIT
 *
 * @brief             - Initiates non-blocking data reception over USART using interrupts
 *
 * @param[in]         - pUSARTHandle: Pointer to USART handle structure
 * @param[out]        - pRxBuffer: Pointer to receive data buffer
 * @param[in]         - Len: Number of bytes to receive
 *
 * @return            - State of USART Rx (e.g. USART_BUSY_IN_RX, USART_READY)
 *
 * @Note              - None
 *********************************************************************/
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    uint8_t rxstate = pUSARTHandle->RxBusyState;

    // Write your code here

    return rxstate;
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
    if(EnorDi == ENABLE)
    {
        pUSARTx->CR1 |= (1 << 13); // Set UE bit in CR1 to enable USART
    }
    else
    {
        pUSARTx->CR1 &= ~(1 << 13); // Clear UE bit in CR1 to disable USART
    }
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
/*********************************************************************
 * @fn                - USART_setBaudRate
 *
 * @brief             - Configures the baud rate for the given USART peripheral
 *
 * @param[in]         - pUSARTx: Base address of the USART peripheral
 * @param[in]         - BaudRate: Desired baud rate (e.g., 9600, 115200)
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{
    uint32_t PCLKx;
    uint32_t usartdiv;
    uint32_t M_part, F_part;
    uint32_t tempreg = 0;

    // 1. Get the APB bus clock for this USART
    if(pUSARTx == USART1 || pUSARTx == USART6)
    {
        // USART1 and USART6 are on APB2 bus
        PCLKx = RCC_GetPCLK2Value();
    }
    else
    {
        // USART2, USART3, USART4, USART5 are on APB1 bus
        PCLKx = RCC_GetPCLK1Value();
    }

    // 2. Check for OVER8 configuration
    if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
    {
        // OVER8 = 1 (Oversampling by 8)
        usartdiv = ((25 * PCLKx) / (2 * BaudRate));
    }
    else
    {
        // OVER8 = 0 (Oversampling by 16)
        usartdiv = ((25 * PCLKx) / (4 * BaudRate));
    }

    // 3. Calculate Mantissa Part
    M_part = usartdiv / 100;

    // Place Mantissa into tempreg
    tempreg |= (M_part << 4);

    // 4. Calculate Fractional Part
    F_part = usartdiv - (M_part * 100);

    if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
    {
        // OVER8 = 1: Multiply fraction by 8, add 50 for rounding, divide by 100
        F_part = (((F_part * 8) + 50) / 100) & ((uint8_t)0x07);
    }
    else
    {
        // OVER8 = 0: Multiply fraction by 16, add 50 for rounding, divide by 100
        F_part = (((F_part * 16) + 50) / 100) & ((uint8_t)0x0F);
    }

    // Place Fraction into tempreg
    tempreg |= F_part;

    // 5. Write to BRR register
    pUSARTx->BRR = tempreg;
}   