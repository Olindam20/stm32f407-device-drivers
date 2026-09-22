/*
 * 009_i2c_tx.c
 *
 *  Created on: Sep 22, 2026
 *      Author: OLINDAM SARMA
 */

#include "stm32f407xx_i2c_driver.h"
#include "stm32f407xx_gpio_driver.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SLAVE_ADDR   0x68

/*
PB6 --> I2C1_SCL
PB9 --> I2C1_SDA
*/

void delay(void)
{
    for(volatile uint32_t i = 0; i < 500000 / 2; i++);
}

void I2C1_GPIOInits(void)
{
    GPIO_Handle_t I2CPins = {0};

    I2CPins.pGPIOx = GPIOB;
    I2CPins.GPIO_PinConfig.mode       = GPIO_MODE_AF;
    I2CPins.GPIO_PinConfig.altFunMode = 4;
    I2CPins.GPIO_PinConfig.opType     = GPIO_OP_TYPE_OD;
    I2CPins.GPIO_PinConfig.pupd       = GPIO_PULL_UP;
    I2CPins.GPIO_PinConfig.speed      = GPIO_SPEED_FAST;

    // SCL - PB6
    I2CPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_6;
    GPIO_Init(&I2CPins);

    // SDA - PB9
    I2CPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_9;
    GPIO_Init(&I2CPins);
}

void I2C1_Inits(I2C_Handle_t *pI2C1Handle)
{
    pI2C1Handle->pI2Cx = I2C1;                                  // Assign base address!
    pI2C1Handle->I2C_Config.I2C_ACKControl   = I2C_ACKControl_EN;
    pI2C1Handle->I2C_Config.I2C_DeviceAddress = 0x61;           // Master's own address
    pI2C1Handle->I2C_Config.I2C_FMDutyCycle  = I2C_FMDutyCycle_2;
    pI2C1Handle->I2C_Config.I2C_SCLSpeed     = I2C_SCLSpeed_100KHz;

    I2C_Init(pI2C1Handle);                                      // Pass pointer directly
}

int main(void)
{
    I2C_Handle_t I2C1Handle = {0};
    char data[] = "Hello World";

    // 1. Initialize GPIO pins
    I2C1_GPIOInits();

    // 2. Initialize I2C1 peripheral
    I2C1_Inits(&I2C1Handle);

    // 3. Enable the I2C1 peripheral (PE = 1)
    I2C_PeripheralControl(I2C1, ENABLE);

    // 4. Send data to slave
    I2C_MasterTransmit(&I2C1Handle, (uint8_t*)data, strlen(data), SLAVE_ADDR);

    while(1);
}