/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Sep 21, 2026
 *      Author: OLINDAM SARMA
 */

#include "stm32f407xx_i2c_driver.h"


uint32_t RCC_GetPLLOutputClock(void)
{
    // Write your code here to calculate the PLL output clock based on the PLL configuration
    return 0; // Placeholder return value
}
static const uint16_t AHB_PreScaler[8]  = {2, 4, 8, 16, 64, 128, 256, 512};
static const uint8_t  APB1_PreScaler[4] = {2, 4, 8, 16};
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
    pI2Cx->CR1 |= (1 << 8); // Set the START bit in CR1 to generate a start condition
}
uint32_t RCC_GetPCLK1Value(void)
{
    uint32_t pclk1, SystemClk;
    uint8_t clksrc, temp, ahbp, apb1p;

    // 1. Determine System Clock Source
    clksrc = (RCC->CFGR >> 2) & 0x3;
    if(clksrc == 0)
    {
        SystemClk = 16000000; // HSI
    }
    else if(clksrc == 1)
    {
        SystemClk = 8000000;  // HSE
    }
    else if(clksrc == 2)
    {
        SystemClk = RCC_GetPLLOutputClock(); // PLL
    }
    else
    {
        SystemClk = 16000000;
    }

    // 2. Calculate AHB division factor
    temp = (RCC->CFGR >> 4) & 0xF;
    if(temp < 8)
    {
        ahbp = 1; // No division
    }
    else
    {
        ahbp = AHB_PreScaler[temp - 8];
    }

    // 3. Calculate APB1 division factor
    temp = (RCC->CFGR >> 10) & 0x7;
    if(temp < 4)
    {
        apb1p = 1; // No division
    }
    else
    {
        apb1p = APB1_PreScaler[temp - 4];
    }

    pclk1 = (SystemClk / ahbp) / apb1p;

    return pclk1;
}
/*********************************************************************
 * @fn                - I2C_PeriClockControl
 *
 * @brief             - Enables or disables peripheral clock for the given I2C peripheral
 *
 * @param[in]         - pI2Cx: Base address of the I2C peripheral (I2C1, I2C2, I2C3)
 * @param[in]         - EnorDi: ENABLE or DISABLE macros
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(pI2Cx == I2C1)
        {
            RCC_I2C1_CLK_EN();
        }
        else if(pI2Cx == I2C2)
        {
            RCC_I2C2_CLK_EN();
        }
        else if(pI2Cx == I2C3)
        {
            RCC_I2C3_CLK_EN();
        }
    }
    else
    {
        if(pI2Cx == I2C1)
        {
            RCC_I2C1_CLK_DI();
        }
        else if(pI2Cx == I2C2)
        {
            RCC_I2C2_CLK_DI();
        }
        else if(pI2Cx == I2C3)
        {
            RCC_I2C3_CLK_DI();
        }
    }
}

/*********************************************************************
 * @fn                - I2C_Init
 *
 * @brief             - Initializes the I2C peripheral with configured settings
 *
 * @param[in]         - pI2CHandle: Pointer to I2C handle structure
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
    // Write your code here
    //Peripheral clock enable
    I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

    uint32_t tempreg = 0;
    //ACK
    tempreg |= (pI2CHandle->I2C_Config.I2C_ACKControl << 10);
    pI2CHandle->pI2Cx->CR1 = tempreg;

    uint32_t pclk1 = RCC_GetPCLK1Value();
    tempreg |= (pclk1 / 1000000U) << 0; // Set FREQ bits in CR2 //pclk1 is in MHz, we need it in Hz
    pI2CHandle->pI2Cx->CR2 = tempreg & 0x3F; // Only lower 6 bits are valid for FREQ

    //device own address
    tempreg = 0;
    tempreg |= (pI2CHandle->I2C_Config.I2C_DeviceAddress << 1); // Set device address in OAR1
    pI2CHandle->pI2Cx->OAR1 = tempreg | (1 << 14); // Set ADDMODE bit for 7-bit addressing
    
    //CCR calculation
    uint16_t ccr_value = 0;
    if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCLSpeed_100KHz)
    {
        // Standard mode
        ccr_value = (pclk1 / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
    }
    else
    {
        // Fast mode
        if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FMDutyCycle_2)
        {
            ccr_value = (pclk1 / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
        }
        else
        {
            ccr_value = (pclk1 / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
        }
        ccr_value |= (1 << 15); // Set FM bit for fast mode
        if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FMDutyCycle_16_9)
        {
            ccr_value |= (1 << 14); // Set DUTY bit for 16/9 duty cycle
        }
    }
    pI2CHandle->pI2Cx->CCR = ccr_value; 

    //Trise calculation
    if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCLSpeed_100KHz)
    {
        // Standard mode
        tempreg = (pclk1 / 1000000U) + 1;
    }
    else
    {
        // Fast mode
        tempreg = (pclk1 / 1000000U) * 600 + 1;
    }
    pI2CHandle->pI2Cx->TRISE = tempreg & 0x3F; // Only lower 6 bits are valid for TRISE
}

/*********************************************************************
 * @fn                - I2C_DeInit
 *
 * @brief             - Resets all registers of the given I2C peripheral
 *
 * @param[in]         - pI2Cx: Base address of the I2C peripheral
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
    // Write your code here
    if(pI2Cx == I2C1)
    {
        // Reset I2C1 peripheral
        RCC->APB1RSTR |= (1 << 21); // Set the reset bit for I2C1
        RCC->APB1RSTR &= ~(1 << 21); // Clear the reset bit for I2C1    
    }
    else if(pI2Cx == I2C2)
    {
        // Reset I2C2 peripheral
        RCC->APB1RSTR |= (1 << 22); // Set the reset bit for I2C2   
        RCC->APB1RSTR &= ~(1 << 22); // Clear the reset bit for I2C2
    }
    else if(pI2Cx == I2C3)
    {
        // Reset I2C3 peripheral
        RCC->APB1RSTR |= (1 << 23); // Set the reset bit for I2C3
        RCC->APB1RSTR &= ~(1 << 23); // Clear the reset bit for I2C3
    }
}

/*********************************************************************
 * @fn                - I2C_MasterTransmit
 *
 * @brief             - Transmits data to a slave device over I2C in Master mode
 *
 * @param[in]         - pI2CHandle: Pointer to I2C handle structure
 * @param[in]         - pTxData: Pointer to transmit data buffer
 * @param[in]         - len: Number of bytes to transmit
 * @param[in]         - slaveAddr: 7-bit slave address
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void I2C_MasterTransmit(I2C_Handle_t *pI2CHandle, uint8_t *pTxData, uint32_t len, uint8_t slaveAddr)
{
    // Write your code here
    // Implementation for I2C master transmit
    //1.Generate start condition
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

    //2.Confirm SB is set in SR1
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

    //3.Send slave address with write bit (0)
    slaveAddr = slaveAddr << 1; // Shift left to make space for R/W
    pI2CHandle->pI2Cx->DR = slaveAddr & ~(1 << 0); // Clear LSB for write operation

    //4. Wait for ADDR flag to be set in SR1
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

    //5. Clear ADDR flag by reading SR1 and SR2
    uint32_t temp;
    temp = pI2CHandle->pI2Cx->SR1;
    temp = pI2CHandle->pI2Cx->SR2;

    //6. Send data bytes
    for(uint32_t i = 0; i < len; i++)
    {
        // Wait until TXE is set
        while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
        pI2CHandle->pI2Cx->DR = pTxData[i];
    }

    //7. Wait until BTF is set
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

    //8. Generate stop condition
    pI2CHandle->pI2Cx->CR1 |= (1 << 9); // Set STOP bit in CR1  

}


/*********************************************************************
 * @fn                - I2C_MasterReceive
 *
 * @brief             - Receives data from a slave device over I2C in Master mode
 *
 * @param[in]         - pI2CHandle: Pointer to I2C handle structure
 * @param[out]        - pRxData: Pointer to buffer where received data will be stored
 * @param[in]         - len: Number of bytes to receive
 * @param[in]         - slaveAddr: 7-bit slave address
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void I2C_MasterReceive(I2C_Handle_t *pI2CHandle, uint8_t *pRxData, uint32_t len, uint8_t slaveAddr)
{
    // Write your code here
}

/*********************************************************************
 * @fn                - I2C_SlaveTransmit
 *
 * @brief             - Transmits 1 byte of data from Slave to Master
 *
 * @param[in]         - pI2Cx: Base address of the I2C peripheral
 * @param[in]         - data: Data byte to transmit
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void I2C_SlaveTransmit(I2C_RegDef_t *pI2Cx, uint8_t data)
{
    // Write your code here
}

/*********************************************************************
 * @fn                - I2C_SlaveReceive
 *
 * @brief             - Receives 1 byte of data from Master in Slave mode
 *
 * @param[in]         - pI2Cx: Base address of the I2C peripheral
 *
 * @return            - Received data byte
 *
 * @Note              - None
 *********************************************************************/
uint8_t I2C_SlaveReceive(I2C_RegDef_t *pI2Cx)
{
    // Write your code here
    return 0;
}

/*********************************************************************
 * @fn                - I2C_PeripheralControl
 *
 * @brief             - Enables or disables the I2C peripheral (PE bit in CR1)
 *
 * @param[in]         - pI2Cx: Base address of the I2C peripheral
 * @param[in]         - EnorDi: ENABLE or DISABLE
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        pI2Cx->CR1 |= (1 << 0);
    }
    else
    {
        pI2Cx->CR1 &= ~(1 << 0);
    }
}

/*********************************************************************
 * @fn                - I2C_GetFlagStatus
 *
 * @brief             - Checks whether a specific I2C status flag is set or reset
 *
 * @param[in]         - pI2Cx: Base address of the I2C peripheral
 * @param[in]         - FlagName: Flag macro from @I2C_FLAGS
 *
 * @return            - FLAG_SET or FLAG_RESET
 *
 * @Note              - None
 *********************************************************************/
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
    // Write your code here
    if(pI2Cx->SR1 & FlagName)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*********************************************************************
 * @fn                - I2C_ManageAcking
 *
 * @brief             - Enables or disables automatic ACK response in CR1
 *
 * @param[in]         - pI2Cx: Base address of the I2C peripheral
 * @param[in]         - EnorDi: ENABLE or DISABLE
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
    pI2Cx->CR1 |= (EnorDi << 10); // Set or clear ACK bit
}

/*********************************************************************
 * @fn                - I2C_IRQInterruptConfig
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
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    // Write your code here
}

/*********************************************************************
 * @fn                - I2C_IRQPriorityConfig
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
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    // Write your code here
}
