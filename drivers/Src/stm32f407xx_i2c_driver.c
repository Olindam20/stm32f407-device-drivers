/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Sep 21, 2026
 *      Author: OLINDAM SARMA
 */

#include "stm32f407xx_i2c_driver.h"

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
    // Write your code here
    if(EnorDi == ENABLE)
    {
        if(pI2Cx == I2C1)
        {
            // Enable I2C1 clock
        }
        else if(pI2Cx == I2C2)
        {
            // Enable I2C2 clock
        }
        else if(pI2Cx == I2C3)
        {
            // Enable I2C3 clock
        }
    }
    else
    {
        if(pI2Cx == I2C1)
        {
            // Disable I2C1 clock
        }
        else if(pI2Cx == I2C2)
        {
            // Disable I2C2 clock
        }
        else if(pI2Cx == I2C3)
        {
            // Disable I2C3 clock
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
    // Write your code here
    pI2Cx->CR1 |= (EnorDi << 0); // Set or clear PE bit
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
    return 0;
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
