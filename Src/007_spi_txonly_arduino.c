#include "stm32f407xx_spi_driver.h"
#include "stm32f407xx_gpio_driver.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Simple software delay for button debouncing
void delay(void)
{
    for(volatile uint32_t i = 0; i < 500000 / 2; i++);
}

void SPI2_GPIOInits(void)
{
    GPIO_Handle_t SPIPins = {0};

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.mode       = GPIO_MODE_AF;
    SPIPins.GPIO_PinConfig.altFunMode = 5;
    SPIPins.GPIO_PinConfig.opType     = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.pupd       = GPIO_NO_PUPD;
    SPIPins.GPIO_PinConfig.speed      = GPIO_SPEED_FAST;

    // MOSI - PB15
    SPIPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_15;
    GPIO_Init(&SPIPins);

    // SCLK - PB13
    SPIPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_13;
    GPIO_Init(&SPIPins);

    // NSS - PB12
    SPIPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_12;
    GPIO_Init(&SPIPins);
}

void GPIO_ButtonInit(void)
{
    GPIO_Handle_t GPIOBtn = {0};
    GPIOBtn.pGPIOx = GPIOA;
    GPIOBtn.GPIO_PinConfig.pinNumber = GPIO_PIN_0;
    GPIOBtn.GPIO_PinConfig.mode      = GPIO_MODE_INPUT;
    GPIOBtn.GPIO_PinConfig.speed     = GPIO_SPEED_FAST;
    GPIOBtn.GPIO_PinConfig.pupd      = GPIO_NO_PUPD; // PA0 is pulled-down externally on Discovery

    GPIO_Init(&GPIOBtn);
}

void SPI2_Inits(void)
{
    SPI_Handle_t SPI2Handle = {0};

    SPI2Handle.pSPIx = SPI2;
    SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI2Handle.SPIConfig.SPI_BusConfig  = SPI_BUS_CONFIG_FULL_DUPLEX;
    SPI2Handle.SPIConfig.SPI_SclkSpeed  = SPI_SCLK_SPEED_DIV64; // ~250 KHz SCLK
    SPI2Handle.SPIConfig.SPI_DFF        = SPI_DFF_8_BIT;
    SPI2Handle.SPIConfig.SPI_CPOL       = SPI_CPOL_LOW;	
    SPI2Handle.SPIConfig.SPI_CPHA       = SPI_CPHA_FIRST_EDGE;
    SPI2Handle.SPIConfig.SPI_SSM        = SPI_SSM_DISABLE; // Hardware NSS

    SPI_Init(&SPI2Handle);
}

int main(void)
{
    char user_data[] = "Hello World";
    uint8_t dataLen = strlen(user_data);

    // 1. Initialize GPIO pins for SPI and Button
    SPI2_GPIOInits();
    GPIO_ButtonInit();

    // 2. Initialize SPI2
    SPI2_Inits();

    // 3. Enable SSOE for Hardware NSS pin management
    SPI_SSOEConfig(SPI2, ENABLE);

    while(1)
    {
        // 4. Wait until the user button (PA0) is pressed
        while(! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0) );

        // 5. Button debounce delay
        delay();

        // 6. Enable the peripheral (SPE = 1, pulls NSS LOW)
        SPI_PeripheralControl(SPI2, ENABLE);

        // 7. Send length of the data first (1 byte)
        SPI_SendData(SPI2, &dataLen, 1);

        // 8. Send the actual data string
        SPI_SendData(SPI2, (uint8_t*)user_data, dataLen);

        // 9. Wait until SPI is not busy before disabling
        while(SPI2->SR & (1 << 7)); // Wait for BSY to clear

        // 10. Disable the peripheral (SPE = 0, pulls NSS HIGH)
        SPI_PeripheralControl(SPI2, DISABLE);
    }

    return 0;
}