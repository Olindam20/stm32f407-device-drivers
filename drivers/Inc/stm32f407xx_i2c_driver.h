/*
 * stm32f407xx_i2c_driver.h
 *
 *  Created on: Sep 21, 2026
 *      Author: OLINDAM SARMA
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_
#include "stm32f407xx.h"

typedef struct{
    uint32_t I2C_SCLSpeed;       /* possible values from @I2C_SCLSpeed */
    uint8_t  I2C_DeviceAddress;  /* possible values from @I2C_DeviceAddress */
    uint8_t  I2C_ACKControl;     /* possible values from @I2C_ACKControl */
    uint16_t I2C_FMDutyCycle;    /* possible values from @I2C_FMDutyCycle */
}I2C_Config_t;

typedef struct
{
    I2C_RegDef_t *pI2Cx; /* This holds the base address of I2Cx(x:0,1,2) peripheral */
    I2C_Config_t I2C_Config; /* This holds I2C configuration settings */
}I2C_Handle_t;


/* @I2C_SCLSpeed
 * possible values for I2C_SCLSpeed
 */
#define I2C_SCLSpeed_100KHz     100000
#define I2C_SCLSpeed_400KHz     400000 

/* @I2C_ACKControl
 * possible values for I2C_ACKControl
 */
#define I2C_ACKControl_EN       1
#define I2C_ACKControl_DI       0

/* @I2C_FMDutyCycle
 * possible values for I2C_FMDutyCycle
 */
#define I2C_FMDutyCycle_2       0
#define I2C_FMDutyCycle_16_9    1

/*
 * @I2C_FLAGS
 * Status flags in I2C_SR1 and I2C_SR2
 */
#define I2C_FLAG_SB             (1 << I2C_SR1_SB)
#define I2C_FLAG_ADDR           (1 << I2C_SR1_ADDR)
#define I2C_FLAG_BTF            (1 << I2C_SR1_BTF)
#define I2C_FLAG_ADD10          (1 << I2C_SR1_ADD10)
#define I2C_FLAG_STOPF          (1 << I2C_SR1_STOPF)
#define I2C_FLAG_RXNE           (1 << I2C_SR1_RXNE)
#define I2C_FLAG_TXE            (1 << I2C_SR1_TXE)
#define I2C_FLAG_BERR           (1 << I2C_SR1_BERR)
#define I2C_FLAG_ARLO           (1 << I2C_SR1_ARLO)
#define I2C_FLAG_AF             (1 << I2C_SR1_AF)
#define I2C_FLAG_OVR            (1 << I2C_SR1_OVR)
#define I2C_FLAG_PECERR         (1 << I2C_SR1_PECERR)
#define I2C_FLAG_TIMEOUT        (1 << I2C_SR1_TIMEOUT)
#define I2C_FLAG_SMBALERT       (1 << I2C_SR1_SMBALERT)

#define I2C_FLAG_MSL            (1 << I2C_SR2_MSL)
#define I2C_FLAG_BUSY           (1 << I2C_SR2_BUSY)
#define I2C_FLAG_TRA            (1 << I2C_SR2_TRA)
#define I2C_FLAG_GENCALL        (1 << I2C_SR2_GENCALL)
#define I2C_FLAG_DUALF          (1 << I2C_SR2_DUALF)

#define I2C_DISABLE_SR          0
#define I2C_ENABLE_SR           1

/*
 * ====================================================================
 *                 APIs supported by this I2C driver
 * ====================================================================
 */

/* Peripheral Clock Setup */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/* Init and De-Init */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/* Data Send and Receive (Master Mode) */
void I2C_MasterTransmit(I2C_Handle_t *pI2CHandle, uint8_t *pTxData, uint32_t len, uint8_t slaveAddr);
void I2C_MasterReceive(I2C_Handle_t *pI2CHandle, uint8_t *pRxData, uint32_t len, uint8_t slaveAddr);

/* Data Send and Receive (Slave Mode) */
void I2C_SlaveTransmit(I2C_RegDef_t *pI2Cx, uint8_t data);
uint8_t I2C_SlaveReceive(I2C_RegDef_t *pI2Cx);

/* IRQ Configuration and ISR Handling */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

/* Other Peripheral Control APIs */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */
