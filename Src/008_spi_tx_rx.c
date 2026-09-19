/**
 ******************************************************************************
 * @file           : 008_spi_tx_rx.c
 * @author         : OLINDAM SARMA
 * @brief          : SPI Master Command & Response Testing with Arduino Slave
 ******************************************************************************
 */

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Command codes supported by Arduino Slave
#define COMMAND_LED_CTRL          0x50
#define COMMAND_SENSOR_READ       0x51
#define COMMAND_LED_READ          0x52
#define COMMAND_PRINT             0x53
#define COMMAND_ID_READ           0x54

#define LED_ON                    1
#define LED_OFF                   0

// Arduino analog pin definitions
#define ANALOG_PIN0               0
#define ANALOG_PIN1               1
#define ANALOG_PIN2               2
#define ANALOG_PIN3               3
#define ANALOG_PIN4               4

// Arduino LED Pin (Arduino Uno onboard LED is Pin 13 or external Pin 9)
#define ARDUINO_LED_PIN           9

// ACK / NACK definitions matching Arduino sketch
#define ACK                       0xF5
#define NACK                      0xA5

// Simple delay function for debouncing & response wait
void delay(void)
{
    for(volatile uint32_t i = 0; i < 500000 / 2; i++);
}

/*
 * PB12 --> SPI2_NSS
 * PB13 --> SPI2_SCLK
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * Alternate Function: AF5
 */
void SPI2_GPIOInits(void)
{
    GPIO_Handle_t SPIPins = {0};

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.mode       = GPIO_MODE_AF;
    SPIPins.GPIO_PinConfig.altFunMode = 5;
    SPIPins.GPIO_PinConfig.opType     = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.pupd       = GPIO_NO_PUPD;
    SPIPins.GPIO_PinConfig.speed      = GPIO_SPEED_FAST;

    // NSS - PB12
    SPIPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_12;
    GPIO_Init(&SPIPins);

    // SCLK - PB13
    SPIPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_13;
    GPIO_Init(&SPIPins);

    // MISO - PB14 (Input from Arduino)
    SPIPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_14;
    GPIO_Init(&SPIPins);

    // MOSI - PB15 (Output to Arduino)
    SPIPins.GPIO_PinConfig.pinNumber  = GPIO_PIN_15;
    GPIO_Init(&SPIPins);
}

void GPIO_ButtonInit(void)
{
    GPIO_Handle_t GPIOBtn = {0};
    GPIOBtn.pGPIOx = GPIOA;
    GPIOBtn.GPIO_PinConfig.pinNumber = GPIO_PIN_0;
    GPIOBtn.GPIO_PinConfig.mode      = GPIO_MODE_INPUT;
    GPIOBtn.GPIO_PinConfig.speed     = GPIO_SPEED_FAST;
    GPIOBtn.GPIO_PinConfig.pupd      = GPIO_NO_PUPD;

    GPIO_Init(&GPIOBtn);
}

void SPI2_Inits(void)
{
    SPI_Handle_t SPI2Handle = {0};

    SPI2Handle.pSPIx = SPI2;
    SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI2Handle.SPIConfig.SPI_BusConfig  = SPI_BUS_CONFIG_FULL_DUPLEX;
    SPI2Handle.SPIConfig.SPI_SclkSpeed  = SPI_SCLK_SPEED_DIV64; // ~250 KHz safe speed for Arduino
    SPI2Handle.SPIConfig.SPI_DFF        = SPI_DFF_8_BIT;
    SPI2Handle.SPIConfig.SPI_CPOL       = SPI_CPOL_LOW;
    SPI2Handle.SPIConfig.SPI_CPHA       = SPI_CPHA_FIRST_EDGE;
    SPI2Handle.SPIConfig.SPI_SSM        = SPI_SSM_DISABLE; // Hardware NSS management

    SPI_Init(&SPI2Handle);
}

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
    if(ackbyte == (uint8_t)ACK)
    {
        return 1; // Valid ACK
    }
    return 0; // NACK or invalid
}

// Global test message in Flash memory (.rodata)
const char user_message[] = "Hello from STM32 Master!";

// Wait for user button press AND release (prevents multiple commands firing in one press)
void wait_for_button_press(void)
{
    // 1. Wait until button is pressed
    while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0));
    delay();

    // 2. Wait until button is released
    while(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0));
    delay();
}

int main(void)
{
    uint8_t dummy_write = 0xFF;
    uint8_t dummy_read  = 0;

    // 1. Initialize GPIOs for Button and SPI
    GPIO_ButtonInit();
    SPI2_GPIOInits();

    // 2. Initialize SPI2 peripheral
    SPI2_Inits();

    // 3. Enable SSOE (SS Output Enable) for Hardware NSS pin management
    SPI_SSOEConfig(SPI2, ENABLE);

    while(1)
    {
        // ====================================================================
        // 1. Send COMMAND_LED_CTRL <pin> <value>
        // ====================================================================
        wait_for_button_press();

        // Enable SPI2 (NSS goes LOW)
        SPI_PeripheralControl(SPI2, ENABLE);

        uint8_t command_code = COMMAND_LED_CTRL;
        uint8_t ackbyte = 0;
        uint8_t args[2];

        // Step 1: Send Command Code
        SPI_SendData(SPI2, &command_code, 1);
        SPI_ReceiveData(SPI2, &dummy_read, 1);

        // Step 2: Send dummy byte to fetch ACK/NACK
        SPI_SendData(SPI2, &dummy_write, 1);
        SPI_ReceiveData(SPI2, &ackbyte, 1);

        // Step 3: Check ACK and send arguments
        if(SPI_VerifyResponse(ackbyte))
        {
            args[0] = ARDUINO_LED_PIN;
            args[1] = LED_ON;
            SPI_SendData(SPI2, args, 2);
            SPI_ReceiveData(SPI2, args, 2);
        }

        // Wait for BSY to clear and disable SPI (NSS goes HIGH)
        while(SPI2->SR & (1 << 7));
        SPI_PeripheralControl(SPI2, DISABLE);


        // ====================================================================
        // 2. Send COMMAND_SENSOR_READ <analog pin>
        // ====================================================================
        wait_for_button_press();

        // Enable SPI2 (NSS goes LOW)
        SPI_PeripheralControl(SPI2, ENABLE);

        command_code = COMMAND_SENSOR_READ;
        SPI_SendData(SPI2, &command_code, 1);
        SPI_ReceiveData(SPI2, &dummy_read, 1);

        // Fetch ACK
        SPI_SendData(SPI2, &dummy_write, 1);
        SPI_ReceiveData(SPI2, &ackbyte, 1);

        if(SPI_VerifyResponse(ackbyte))
        {
            uint8_t analog_pin = ANALOG_PIN0;
            SPI_SendData(SPI2, &analog_pin, 1);
            SPI_ReceiveData(SPI2, &dummy_read, 1);

            // Wait small duration for Arduino ADC
            delay();

            uint8_t sensor_read = 0;
            SPI_SendData(SPI2, &dummy_write, 1);
            SPI_ReceiveData(SPI2, &sensor_read, 1);
        }

        // Wait for BSY to clear and disable SPI (NSS goes HIGH)
        while(SPI2->SR & (1 << 7));
        SPI_PeripheralControl(SPI2, DISABLE);


        // ====================================================================
        // 3. Send COMMAND_PRINT <len> <message>
        // ====================================================================
        wait_for_button_press();

        // Enable SPI2 (NSS goes LOW)
        SPI_PeripheralControl(SPI2, ENABLE);

        command_code = COMMAND_PRINT;
        SPI_SendData(SPI2, &command_code, 1);
        SPI_ReceiveData(SPI2, &dummy_read, 1);

        // Fetch ACK
        SPI_SendData(SPI2, &dummy_write, 1);
        SPI_ReceiveData(SPI2, &ackbyte, 1);

        if(SPI_VerifyResponse(ackbyte))
        {
            uint8_t msg_len = strlen(user_message);

            // Send length first
            SPI_SendData(SPI2, &msg_len, 1);
            SPI_ReceiveData(SPI2, &dummy_read, 1);

            // Send actual message characters
            for(uint8_t i = 0; i < msg_len; i++)
            {
                SPI_SendData(SPI2, (uint8_t*)&user_message[i], 1);
                SPI_ReceiveData(SPI2, &dummy_read, 1);
            }
        }

        // Wait for BSY to clear and disable SPI (NSS goes HIGH)
        while(SPI2->SR & (1 << 7));
        SPI_PeripheralControl(SPI2, DISABLE);


        // ====================================================================
        // 4. Send COMMAND_ID_READ
        // ====================================================================
        wait_for_button_press();

        // Enable SPI2 (NSS goes LOW)
        SPI_PeripheralControl(SPI2, ENABLE);

        command_code = COMMAND_ID_READ;
        SPI_SendData(SPI2, &command_code, 1);
        SPI_ReceiveData(SPI2, &dummy_read, 1);

        // Fetch ACK
        SPI_SendData(SPI2, &dummy_write, 1);
        SPI_ReceiveData(SPI2, &ackbyte, 1);

        uint8_t id_buf[11] = {0};
        if(SPI_VerifyResponse(ackbyte))
        {
            for(uint8_t i = 0; i < 10; i++)
            {
                SPI_SendData(SPI2, &dummy_write, 1);
                SPI_ReceiveData(SPI2, &id_buf[i], 1);
            }
            id_buf[10] = '\0';
        }

        // Wait for BSY to clear and disable SPI (NSS goes HIGH)
        while(SPI2->SR & (1 << 7));
        SPI_PeripheralControl(SPI2, DISABLE);
    }
    SPI_PeripheralControl(SPI2, DISABLE);
    return 0;
}

