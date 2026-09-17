/*
 * 006spi_tx_testing.c
 *
 *  Created on: Sep 16, 2026
 *      Author: OLINDAM SARMA
 */
#include "stm32f407xx_spi_driver.h"
#include "stm32f407xx_gpio_driver.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

//PB15 --> SPI2_MOSI
//PB14 --> SPI2_MISO
//PB13 --> SPI2_SCLK
//PB12 --> SPI2_NSS
//Alternate function mode: 5

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins = {0};

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_15; //MOSI
    SPIPins.GPIO_PinConfig.mode       = GPIO_MODE_AF;
    SPIPins.GPIO_PinConfig.altFunMode = 5;
    SPIPins.GPIO_PinConfig.opType     = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.pupd       = GPIO_NO_PUPD;
    SPIPins.GPIO_PinConfig.speed      = GPIO_SPEED_FAST;
    GPIO_Init(&SPIPins);

	SPIPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_13; //SCLK
	GPIO_Init(&SPIPins);
	
    /*
	SPIPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_14; //MISO
	GPIO_Init(&SPIPins);

	

	SPIPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_12; //NSS
	GPIO_Init(&SPIPins);
	*/
}
void SPI2_Inits(void)
{
	SPI_Handle_t SPI2Handle = {0};

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FULL_DUPLEX;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2; //generates sclk of 8MHz
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8_BIT;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;	
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_FIRST_EDGE;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_ENABLE; //software slave management enabled for NSS pin

	SPI_Init(&SPI2Handle);
}

int main(void)
{
    // 1. Initialize GPIO pins
    SPI2_GPIOInits();

    // 2. Initialize SPI2
    SPI2_Inits();

    // 3. FIRST enable SSI (Make NSS HIGH BEFORE turning on SPI)
    SPI_SSIConfig(SPI2, ENABLE);

    // 4. NOW enable the peripheral (SPE = 1)
    SPI_PeripheralControl(SPI2, ENABLE);

    char user_data[] = "Hello World";
    SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

    while(SPI2->SR & (1 << 7)); // Wait for BSY to clear
    SPI_PeripheralControl(SPI2, DISABLE);

    while(1);
    return 0;
}