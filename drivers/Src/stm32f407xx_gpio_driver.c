#include "stm32f407xx_gpio_driver.h"

/*********************************************************************
 * @fn                - GPIO_PeriClockControl
 *
 * @brief             - Enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral (GPIOA, GPIOB, etc.)
 * @param[in]         - EnorDi: ENABLE or DISABLE macros
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
    // Write your code here
    if(EnorDi==ENABLE)
    {
        if(pGPIOx == GPIOA)
        {
            RCC_GPIOA_CLK_EN();
        }
        else if(pGPIOx == GPIOB)
        {
            RCC_GPIOB_CLK_EN();
        }
        else if(pGPIOx == GPIOC)
        {
            RCC_GPIOC_CLK_EN();
        }
        else if(pGPIOx == GPIOD)
        {
            RCC_GPIOD_CLK_EN();
        }
        else if(pGPIOx == GPIOE)
        {
            RCC_GPIOE_CLK_EN();
        }
        else if(pGPIOx == GPIOF)
        {
            RCC_GPIOF_CLK_EN();
        }
        else if(pGPIOx == GPIOG)
        {
            RCC_GPIOG_CLK_EN();
        }
        else if(pGPIOx == GPIOH)
        {
            RCC_GPIOH_CLK_EN();
        }
        else if(pGPIOx == GPIOI)
        {
            RCC_GPIOI_CLK_EN();
        }
    }
    else
    {
        if(pGPIOx == GPIOA)
        {
            RCC_GPIOA_CLK_DI();
        }
        else if(pGPIOx == GPIOB)
        {
            RCC_GPIOB_CLK_DI();
        }
        else if(pGPIOx == GPIOC)
        {
            RCC_GPIOC_CLK_DI();
        }
        else if(pGPIOx == GPIOD)
        {
            RCC_GPIOD_CLK_DI();
        }
        else if(pGPIOx == GPIOE)
        {
            RCC_GPIOE_CLK_DI();
        }
        else if(pGPIOx == GPIOF)
        {
            RCC_GPIOF_CLK_DI();
        }
        else if(pGPIOx == GPIOG)
        {
            RCC_GPIOG_CLK_DI();
        }
        else if(pGPIOx == GPIOH)
        {
            RCC_GPIOH_CLK_DI();
        }
        else if(pGPIOx == GPIOI)
        {
            RCC_GPIOI_CLK_DI();
        }
    }
}


/*********************************************************************
 * @fn                - GPIO_Init
 *
 * @brief             - Initializes the given GPIO pin according to the settings in pGPIOHandle
 *
 * @param[in]         - pGPIOHandle: Pointer to GPIO handle structure containing port base address and pin configuration
 *
 * @return            - None
 *
 * @Note              - None
 *********************************************************************/
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
    // Write your code here
    if(pGPIOHandle->GPIO_PinConfig.mode <= GPIO_MODE_ANALOG)
    {
        // Non-interrupt mode
        uint32_t temp = 0;
        // 1. Configure the mode of GPIO pin
        temp = (pGPIOHandle->GPIO_PinConfig.mode << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber));
        pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber)); // Clear the bits
        pGPIOHandle->pGPIOx->MODER |= temp; // Set the bits
    }
    else
    {
        // Interrupt mode configuration will be handled here
        if(pGPIOHandle->GPIO_PinConfig.mode == GPIO_MODE_IT_FT)
        {
            // Configure falling edge trigger
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.pinNumber);
            EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.pinNumber); // Disable rising edge
        }
        else if(pGPIOHandle->GPIO_PinConfig.mode == GPIO_MODE_IT_RT)
        {
            // Configure rising edge trigger
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.pinNumber);
            EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.pinNumber); // Disable falling edge
        }
        else if(pGPIOHandle->GPIO_PinConfig.mode == GPIO_MODE_IT_RFT)
        {
            // Configure both edges trigger
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.pinNumber);
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.pinNumber);
        }


        //enable external interrupt delivery using IMR
        EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.pinNumber);

        RCC_SYSCFG_CLK_EN(); // Enable clock for SYSCFG peripheral

        //Configure the gpio port selection in SYSCFG_EXTICR
        uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.pinNumber / 4; // Determine which EXTICR register to use
        uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.pinNumber % 4; // Determine the position within the EXTICR register
        uint8_t portCode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx); // Get the port code (0 for GPIOA, 1 for GPIOB, etc.)
        SYSCFG->EXTICR[temp1] |= (portCode << (temp2 * 4)); // Set the port code
    }

    //speed
    uint32_t temp = 0;
    temp = (pGPIOHandle->GPIO_PinConfig.speed << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber));
    pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber)); // Clear the bits
    pGPIOHandle->pGPIOx->OSPEEDR |= temp; // Set the bits

    //pull-up/pull-down
    temp = (pGPIOHandle->GPIO_PinConfig.pupd << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber));
    pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber)); // Clear the bits
    pGPIOHandle->pGPIOx->PUPDR |= temp; // Set the bits

    // Output type
    temp = (pGPIOHandle->GPIO_PinConfig.opType << (pGPIOHandle->GPIO_PinConfig.pinNumber));
    pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << (pGPIOHandle->GPIO_PinConfig.pinNumber)); // Clear the bit
    pGPIOHandle->pGPIOx->OTYPER |= temp; // Set the bit

    //alternate function
    if(pGPIOHandle->GPIO_PinConfig.mode == GPIO_MODE_AF)    
    {
        uint8_t afrIndex = pGPIOHandle->GPIO_PinConfig.pinNumber / 8; // 0 for AFR[0], 1 for AFR[1]
        uint8_t afrPosition = pGPIOHandle->GPIO_PinConfig.pinNumber % 8; // Position within AFR register
        temp = (pGPIOHandle->GPIO_PinConfig.altFunMode << (4 * afrPosition));
        pGPIOHandle->pGPIOx->AFR[afrIndex] &= ~(0xF << (4 * afrPosition)); // Clear the bits
        pGPIOHandle->pGPIOx->AFR[afrIndex] |= temp; // Set the bits
    }
}


/*********************************************************************
 * @fn                - GPIO_DeInit
 *
 * @brief             - Resets all registers of the given GPIO port back to default state
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 *
 * @return            - None
 *
 * @Note              - Done using RCC AHB1 peripheral reset register (RCC_AHB1RSTR)
 *********************************************************************/
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
    // Write your code here
    // Reset the GPIO port
    if(pGPIOx == GPIOA)
    {
        RCC->AHB1RSTR |= (1 << 0); // Set the reset bit for GPIOA
        RCC->AHB1RSTR &= ~(1 << 0); // Clear the reset bit for GPIOA
    }
    else if(pGPIOx == GPIOB)
    {
        RCC->AHB1RSTR |= (1 << 1); // Set the reset bit for GPIOB
        RCC->AHB1RSTR &= ~(1 << 1); // Clear the reset bit for GPIOB
    }
    else if(pGPIOx == GPIOC)
    {
        RCC->AHB1RSTR |= (1 << 2); // Set the reset bit for GPIOC
        RCC->AHB1RSTR &= ~(1 << 2); // Clear the reset bit for GPIOC
    }
    else if(pGPIOx == GPIOD)
    {
        RCC->AHB1RSTR |= (1 << 3); // Set the reset bit for GPIOD
        RCC->AHB1RSTR &= ~(1 << 3); // Clear the reset bit for GPIOD
    }
    else if(pGPIOx == GPIOE)
    {
        RCC->AHB1RSTR |= (1 << 4); // Set the reset bit for GPIOE
        RCC->AHB1RSTR &= ~(1 << 4); // Clear the reset bit for GPIOE
    }
    else if(pGPIOx == GPIOF)
    {
        RCC->AHB1RSTR |= (1 << 5); // Set the reset bit for GPIOF
        RCC->AHB1RSTR &= ~(1 << 5); // Clear the reset bit for GPIOF
    }
    else if(pGPIOx == GPIOG)
    {
        RCC->AHB1RSTR |= (1 << 6); // Set the reset bit for GPIOG
        RCC->AHB1RSTR &= ~(1 << 6); // Clear the reset bit for GPIOG
    }
    else if(pGPIOx == GPIOH)
    {
        RCC->AHB1RSTR |= (1 << 7); // Set the reset bit for GPIOH
        RCC->AHB1RSTR &= ~(1 << 7); // Clear the reset bit for GPIOH
    }
    else if(pGPIOx == GPIOI)
    {
        RCC->AHB1RSTR |= (1 << 8); // Set the reset bit for GPIOI
        RCC->AHB1RSTR &= ~(1 << 8); // Clear the reset bit for GPIOI
    }
    
}


/*********************************************************************
 * @fn                - GPIO_ReadFromInputPin
 *
 * @brief             - Reads logic state (0 or 1) from the specified GPIO input pin
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 * @param[in]         - pinNumber: GPIO pin number (0 to 15)
 * @param[out]        - pData: Optional pointer to store the read value (or return directly)
 *
 * @return            - 0 or 1 (value of the pin)
 *
 * @Note              - Reads from the Input Data Register (IDR)
 *********************************************************************/
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
    // Write your code here
    uint8_t value = (uint8_t)((pGPIOx->IDR >> pinNumber) & 0x1);
    return value;
}


/*********************************************************************
 * @fn                - GPIO_ReadFromInputPort
 *
 * @brief             - Reads 16-bit logic values from the entire GPIO input port
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 * @param[out]        - pData: Optional pointer to store the port value (or return directly)
 *
 * @return            - 16-bit value representing state of all 16 pins of the port
 *
 * @Note              - Reads entire Input Data Register (IDR)
 *********************************************************************/
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
    // Write your code here
    uint16_t value = (uint16_t)(pGPIOx->IDR & 0xFFFF);
    return value;
}


/*********************************************************************
 * @fn                - GPIO_WriteToOutputPin
 *
 * @brief             - Writes logic state (GPIO_PIN_SET / GPIO_PIN_RESET) to the specified output pin
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 * @param[in]         - pinNumber: GPIO pin number (0 to 15)
 * @param[in]         - value: GPIO_PIN_SET (1) or GPIO_PIN_RESET (0)
 *
 * @return            - None
 *
 * @Note              - Writes to Output Data Register (ODR) or Bit Set/Reset Register (BSRR)
 *********************************************************************/
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value)
{
    // Write your code here
    pGPIOx->ODR &=~(1<<pinNumber); // Clear the bit
    pGPIOx->ODR |= (value << pinNumber); // Set the bit according to value
}


/*********************************************************************
 * @fn                - GPIO_WriteToOutputPort
 *
 * @brief             - Writes 16-bit value to the entire GPIO output port
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 * @param[in]         - value: 16-bit value to write to the entire port
 *
 * @return            - None
 *
 * @Note              - Writes to Output Data Register (ODR)
 *********************************************************************/
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value)
{
    // Write your code here
    pGPIOx->ODR &= ~(0xFFFF); // Clear the entire port
    pGPIOx->ODR |= value; // Set the port to the given value
}


/*********************************************************************
 * @fn                - GPIO_ToggleOutputPin
 *
 * @brief             - Toggles the current state of the specified GPIO output pin (0 -> 1 or 1 -> 0)
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 * @param[in]         - pinNumber: GPIO pin number (0 to 15)
 *
 * @return            - None
 *
 * @Note              - Uses XOR (^) on the Output Data Register (ODR)
 *********************************************************************/
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
    // Write your code here
    pGPIOx->ODR ^= (1 << pinNumber); // Toggle the bit using XOR
}


/*********************************************************************
 * @fn                - GPIO_IRQConfig
 *
 * @brief             - Configures IRQ number, priority, and enables/disables the interrupt in NVIC
 *
 * @param[in]         - IRQNumber: Cortex-M4 NVIC IRQ position number
 * @param[in]         - IRQPriority: Interrupt priority level (0 to 15)
 * @param[in]         - EnorDi: ENABLE or DISABLE
 *
 * @return            - None
 *
 * @Note              - Configures NVIC ISER, ICER, and IPR registers
 *********************************************************************/
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    // Write your code here
    if(EnorDi == ENABLE) 
    
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

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    // Write your code here
    uint8_t iprx = IRQNumber / 4; // Determine which IPR register to use
    uint8_t iprx_section = IRQNumber % 4; // Determine the section within the IPR register
    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED); // Calculate the shift amount based on implemented priority bits
    uint32_t priority_mask = (0xFF << shift_amount); // Create a mask for the priority bits
    *(NVIC_IPR_BASEADDR + iprx) |= (IRQPriority << shift_amount); // Set the priority in the appropriate IPR register
}


/*********************************************************************
 * @fn                - GPIO_IRQHandling
 *
 * @brief             - Clears the pending interrupt flag in EXTI_PR register when an interrupt occurs
 *
 * @param[in]         - pinNumber: Pin number (EXTI line number) that triggered the interrupt
 *
 * @return            - None
 *
 * @Note              - Cleared by writing '1' to corresponding bit in EXTI_PR
 *********************************************************************/
void GPIO_IRQHandling(uint8_t pinNumber)
{
    // Write your code here
    //clear the EXTI PR register corresponding to the pin number
    if(EXTI->PR & (1 << pinNumber))
    {
        EXTI->PR |= (1 << pinNumber); // Clear the pending bit by writing '1'
    }
}
