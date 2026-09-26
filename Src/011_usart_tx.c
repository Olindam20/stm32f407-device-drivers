/*
 * 011_usart_tx.c
 *
 *  Created on: Sep 26, 2026
 *      Author: OLINDAM SARMA
 *
 *  Exercise:
 *  Send a message over UART from STM32 to Arduino board at 115200 bps.
 *  Frame format: 1 stop bit, 8 bits word length, no parity.
 *
 *  Pin mapping for USART2:
 *  PA2 -> USART2_TX (AF7)
 *  PA3 -> USART2_RX (AF7)
 *  PA0 -> User Button
 */

#include <stdio.h>
#include <string.h>
#include "stm32f407xx.h"
#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_gpio_driver.h"

// Message to be sent
char msg[1024] = "UART testing from STM32 to Arduino at 115200 bps...\n\r";

USART_Handle_t usart2_handle;

void delay(void)
{
    for(volatile uint32_t i = 0; i < 500000 / 2; i++);
}

void USART2_GPIOInits(void)
{
    GPIO_Handle_t usart_pins = {0};

    usart_pins.pGPIOx = GPIOA;
    usart_pins.GPIO_PinConfig.mode = GPIO_MODE_AF;
    usart_pins.GPIO_PinConfig.opType = GPIO_OP_TYPE_PP;
    usart_pins.GPIO_PinConfig.pupd = GPIO_PULL_UP;
    usart_pins.GPIO_PinConfig.speed = GPIO_SPEED_FAST;
    usart_pins.GPIO_PinConfig.altFunMode = 7; // AF7 for USART2

    // USART2_TX -> PA2
    usart_pins.GPIO_PinConfig.pinNumber = GPIO_PIN_2;
    GPIO_Init(&usart_pins);

    // USART2_RX -> PA3
    usart_pins.GPIO_PinConfig.pinNumber = GPIO_PIN_3;
    GPIO_Init(&usart_pins);
}

void USART2_Inits(void)
{
    usart2_handle.pUSARTx = USART2;
    usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
    usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    usart2_handle.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
    usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
    usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;

    USART_Init(&usart2_handle);
}

void GPIO_ButtonInit(void)
{
    GPIO_Handle_t GPIOBtn = {0};

    GPIOBtn.pGPIOx = GPIOA;
    GPIOBtn.GPIO_PinConfig.pinNumber = GPIO_PIN_0;
    GPIOBtn.GPIO_PinConfig.mode = GPIO_MODE_INPUT;
    GPIOBtn.GPIO_PinConfig.speed = GPIO_SPEED_FAST;
    GPIOBtn.GPIO_PinConfig.pupd = GPIO_NO_PUPD;

    GPIO_Init(&GPIOBtn);
}

int main(void)
{
    // Initialize button
    GPIO_ButtonInit();

    // Initialize GPIO pins for USART2
    USART2_GPIOInits();

    // Initialize USART2 peripheral parameters
    USART2_Inits();

    // Enable USART2 Peripheral
    USART_PeripheralControl(USART2, ENABLE);

    while(1)
    {
        // Wait for button press (PA0 is HIGH when pressed on STM32 Discovery)
        while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0));
        // Send the message over USART2
        USART_SendData(&usart2_handle, (uint8_t*)msg, strlen(msg));

        // Button debounce delay
        // Delay 1 second between transmissions
        delay();

        // Send the message over USART2
        USART_SendData(&usart2_handle, (uint8_t*)msg, strlen(msg));
        delay();
    }

    return 0;
}
