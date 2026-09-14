# STM32F407 Custom GPIO Driver Documentation & Complete Reference

This document covers **100% of the custom GPIO driver codebase**, including every API, structure, macro, register manipulation, and debugging insight learned during implementation for the **STM32F407 (ARM Cortex-M4)** MCU.

---

## Table of Contents
1. [Architecture & Bus Map](#1-architecture--bus-map)
2. [Header Definitions & Macros (`stm32f407xx.h`)](#2-header-definitions--macros-stm32f407xxh)
3. [GPIO Driver Header (`stm32f407xx_gpio_driver.h`)](#3-gpio-driver-header-stm32f407xx_gpio_driverh)
4. [Complete Driver APIs & Implementation (`stm32f407xx_gpio_driver.c`)](#4-complete-driver-apis--implementation-stm32f407xx_gpio_driverc)
   - [4.1 Clock Control (`GPIO_PeriClockControl`)](#41-clock-control-gpio_periclockcontrol)
   - [4.2 Initialization (`GPIO_Init`)](#42-initialization-gpio_init)
   - [4.3 De-initialization (`GPIO_DeInit`)](#43-de-initialization-gpio_deinit)
   - [4.4 Pin & Port Data Read (`GPIO_ReadFromInputPin` & `GPIO_ReadFromInputPort`)](#44-pin--port-data-read)
   - [4.5 Pin & Port Data Write (`GPIO_WriteToOutputPin` & `GPIO_WriteToOutputPort`)](#45-pin--port-data-write)
   - [4.6 Output Pin Toggle (`GPIO_ToggleOutputPin`)](#46-output-pin-toggle-gpio_toggleoutputpin)
   - [4.7 Interrupt Configuration (`GPIO_IRQConfig`)](#47-interrupt-configuration-gpio_irqconfig)
   - [4.8 Interrupt Priority Configuration (`GPIO_IRQPriorityConfig`)](#48-interrupt-priority-configuration-gpio_irqpriorityconfig)
   - [4.9 Interrupt Handling & Clearing Flag (`GPIO_IRQHandling`)](#49-interrupt-handling--clearing-flag-gpio_irqhandling)
5. [Interrupt Subsystem Deep Dive (EXTI + SYSCFG + NVIC)](#5-interrupt-subsystem-deep-dive-exti--syscfg--nvic)
6. [Critical Debugging Lessons & Embedded C Gotchas](#6-critical-debugging-lessons--embedded-c-gotchas)
7. [Full Application Examples](#7-full-application-examples)

---

## 1. Architecture & Bus Map

The STM32F407 connects its peripherals across multiple internal buses:

```
                            ┌────────────────────────────────────────┐
                            │        ARM Cortex-M4 CPU Core          │
                            │         • NVIC (0xE000E100)            │
                            └───────────────────┬────────────────────┘
                                                │
                 ┌──────────────────────────────┴──────────────────────────────┐
                 │                                                             │
                 ▼ AHB1 Bus (Max 168 MHz)                                      ▼ APB2 Bus (Max 84 MHz)
  ┌───────────────────────────────┐                             ┌───────────────────────────────┐
  │         GPIO Ports            │                             │       SYSCFG & EXTI           │
  │ • GPIOA: 0x40020000           │                             │ • SYSCFG: 0x40013800          │
  │ • GPIOB: 0x40020400           │                             │ • EXTI:   0x40013C00          │
  │ • GPIOC: 0x40020800           │                             │                               │
  │ • GPIOD: 0x40020C00           │                             │ Routes pins (PAx..PIx) to     │
  │ • GPIOE: 0x40021000           │                             │ EXTI Lines 0 to 15, detects   │
  │ • GPIOF: 0x40021400           │                             │ rising/falling edges.         │
  │ • GPIOG: 0x40021800           │                             └───────────────────────────────┘
  │ • GPIOH: 0x40021C00           │
  │ • GPIOI: 0x40022000           │
  │ • RCC:   0x40023800           │
  └───────────────────────────────┘
```

---

## 2. Header Definitions & Macros (`stm32f407xx.h`)

### 2.1 Peripheral Base Addresses (Parentheses Protected)
```c
#define FLASH_BASEADDR      0x08000000U
#define SRAM1_BASEADDR      0x20000000U
#define SRAM2_BASEADDR      (SRAM1_BASEADDR + (112 * 1024))
#define ROM_BASEADDR        0x1FFF0000U
#define SRAM_BASEADDR       SRAM1_BASEADDR

#define PERIPH_BASEADDR     0x40000000U
#define APB1_BASEADDR       PERIPH_BASEADDR
#define APB2_BASEADDR       0x40010000U
#define AHB1_BASEADDR       0x40020000U 
#define AHB2_BASEADDR       0x50000000U

/* AHB1 Peripherals */
#define GPIOA_BASEADDR      (AHB1_BASEADDR + 0x0000U)
#define GPIOB_BASEADDR      (AHB1_BASEADDR + 0x0400U)   
#define GPIOC_BASEADDR      (AHB1_BASEADDR + 0x0800U)
#define GPIOD_BASEADDR      (AHB1_BASEADDR + 0x0C00U)
#define GPIOE_BASEADDR      (AHB1_BASEADDR + 0x1000U)
#define GPIOF_BASEADDR      (AHB1_BASEADDR + 0x1400U) 
#define GPIOG_BASEADDR      (AHB1_BASEADDR + 0x1800U)
#define GPIOH_BASEADDR      (AHB1_BASEADDR + 0x1C00U)
#define GPIOI_BASEADDR      (AHB1_BASEADDR + 0x2000U)
#define RCC_BASEADDR        (AHB1_BASEADDR + 0x3800U)

/* APB2 Peripherals */
#define SYSCFG_BASEADDR     (APB2_BASEADDR + 0x3800U)
#define EXTI_BASEADDR       (APB2_BASEADDR + 0x3C00U)
```

### 2.2 Peripheral Register Structures
```c
#define __vo volatile

/* GPIO Register Definition Structure */
typedef struct
{
    __vo uint32_t MODER;        /*!< GPIO port mode register,               Offset: 0x00 */
    __vo uint32_t OTYPER;       /*!< GPIO port output type register,        Offset: 0x04 */
    __vo uint32_t OSPEEDR;      /*!< GPIO port output speed register,       Offset: 0x08 */
    __vo uint32_t PUPDR;        /*!< GPIO port pull-up/pull-down register,  Offset: 0x0C */
    __vo uint32_t IDR;          /*!< GPIO port input data register,         Offset: 0x10 */
    __vo uint32_t ODR;          /*!< GPIO port output data register,        Offset: 0x14 */
    __vo uint32_t BSRR;         /*!< GPIO port bit set/reset register,      Offset: 0x18 */
    __vo uint32_t LCKR;         /*!< GPIO port configuration lock register, Offset: 0x1C */
    __vo uint32_t AFR[2];       /*!< AFR[0]: AFRL (pins 0-7), AFR[1]: AFRH (pins 8-15)  Offset: 0x20-0x24 */ 
} GPIO_RegDef_t;

/* EXTI Register Definition Structure */
typedef struct
{
    __vo uint32_t IMR;   /*!< Interrupt mask register,            Offset: 0x00 */
    __vo uint32_t EMR;   /*!< Event mask register,                Offset: 0x04 */
    __vo uint32_t RTSR;  /*!< Rising trigger selection register,  Offset: 0x08 */
    __vo uint32_t FTSR;  /*!< Falling trigger selection register, Offset: 0x0C */
    __vo uint32_t SWIER; /*!< Software interrupt enable register, Offset: 0x10 */
    __vo uint32_t PR;    /*!< Pending register (rc_w1),           Offset: 0x14 */
} EXTI_RegDef_t;

/* SYSCFG Register Definition Structure */
typedef struct
{
    __vo uint32_t MEMRMP;       /*!< Memory remap register,                      Offset: 0x00 */
    __vo uint32_t PMC;          /*!< Peripheral mode configuration register,     Offset: 0x04 */
    __vo uint32_t EXTICR[4];    /*!< External interrupt configuration registers, Offset: 0x08-0x14 */
         uint32_t RESERVED1[2]; /*!< Reserved,                                   Offset: 0x18-0x1C */
    __vo uint32_t CMPCR;        /*!< Compensation cell control register,         Offset: 0x20 */
} SYSCFG_RegDef_t;
```

### 2.3 Clock Enable & Disable Macros
```c
/* GPIO Clock Enable */
#define RCC_GPIOA_CLK_EN()  (RCC->AHB1ENR |= (1 << 0))
#define RCC_GPIOB_CLK_EN()  (RCC->AHB1ENR |= (1 << 1))
#define RCC_GPIOC_CLK_EN()  (RCC->AHB1ENR |= (1 << 2))
#define RCC_GPIOD_CLK_EN()  (RCC->AHB1ENR |= (1 << 3))
#define RCC_GPIOE_CLK_EN()  (RCC->AHB1ENR |= (1 << 4))
#define RCC_GPIOF_CLK_EN()  (RCC->AHB1ENR |= (1 << 5))
#define RCC_GPIOG_CLK_EN()  (RCC->AHB1ENR |= (1 << 6))
#define RCC_GPIOH_CLK_EN()  (RCC->AHB1ENR |= (1 << 7))
#define RCC_GPIOI_CLK_EN()  (RCC->AHB1ENR |= (1 << 8))

/* SYSCFG Clock Enable (APB2 Bit 14) */
#define RCC_SYSCFG_CLK_EN() (RCC->APB2ENR |= (1 << 14))
#define RCC_SYSCFG_CLK_DI() (RCC->APB2ENR &= ~(1 << 14))

/* Macro to convert GPIO base address to 4-bit port code for SYSCFG_EXTICR */
#define GPIO_BASEADDR_TO_CODE(pGPIOx) (((pGPIOx) == GPIOA) ? 0 : \
                                        ((pGPIOx) == GPIOB) ? 1 : \
                                        ((pGPIOx) == GPIOC) ? 2 : \
                                        ((pGPIOx) == GPIOD) ? 3 : \
                                        ((pGPIOx) == GPIOE) ? 4 : \
                                        ((pGPIOx) == GPIOF) ? 5 : \
                                        ((pGPIOx) == GPIOG) ? 6 : \
                                        ((pGPIOx) == GPIOH) ? 7 : \
                                        ((pGPIOx) == GPIOI) ? 8 : 0)
```

### 2.4 Cortex-M4 NVIC Register Macros & IRQ Numbers
```c
/* NVIC ISER / ICER / IPR Registers */
#define NVIC_ISER0          ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1          ((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2          ((volatile uint32_t*)0xE000E108)
#define NVIC_ICER0          ((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1          ((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2          ((volatile uint32_t*)0xE000E188)
#define NVIC_IPR_BASEADDR   ((volatile uint32_t*)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED  4

/* EXTI IRQ Numbers in Cortex-M4 Vector Table */
#define IRQ_NO_EXTI0        6
#define IRQ_NO_EXTI1        7      
#define IRQ_NO_EXTI2        8
#define IRQ_NO_EXTI3        9
#define IRQ_NO_EXTI4        10
#define IRQ_NO_EXTI9_5      23
#define IRQ_NO_EXTI15_10    40
```

---

## 3. GPIO Driver Header (`stm32f407xx_gpio_driver.h`)

```c
/* Pin Configuration Structure */
typedef struct
{
    uint8_t pinNumber;      /* GPIO_PIN_0 to GPIO_PIN_15 */
    uint8_t mode;           /* GPIO Modes */
    uint8_t speed;          /* GPIO Output Speed */
    uint8_t opType;         /* Output Type: Push-Pull / Open-Drain */
    uint8_t pupd;           /* Pull-up / Pull-down */
    uint8_t altFunMode;     /* Alternate Function Mode (AF0 to AF15) */
} GPIO_PinConfig_t;

/* GPIO Handle Structure */
typedef struct
{
    GPIO_RegDef_t *pGPIOx;           /* Base address of port (GPIOA..GPIOI) */
    GPIO_PinConfig_t GPIO_PinConfig; /* User configuration */
} GPIO_Handle_t;

/* GPIO Pin Numbers */
#define GPIO_PIN_0          0
#define GPIO_PIN_1          1
#define GPIO_PIN_2          2
#define GPIO_PIN_3          3
#define GPIO_PIN_4          4
#define GPIO_PIN_5          5
#define GPIO_PIN_6          6
#define GPIO_PIN_7          7
#define GPIO_PIN_8          8
#define GPIO_PIN_9          9
#define GPIO_PIN_10         10
#define GPIO_PIN_11         11
#define GPIO_PIN_12         12
#define GPIO_PIN_13         13
#define GPIO_PIN_14         14
#define GPIO_PIN_15         15

/* GPIO Modes */
#define GPIO_MODE_INPUT     0
#define GPIO_MODE_OUTPUT    1
#define GPIO_MODE_AF        2
#define GPIO_MODE_ANALOG    3
#define GPIO_MODE_IT_FT     4   /* Interrupt Falling Edge */
#define GPIO_MODE_IT_RT     5   /* Interrupt Rising Edge */
#define GPIO_MODE_IT_RFT    6   /* Interrupt Rising & Falling Edge */

/* Output Types */
#define GPIO_OP_TYPE_PP     0   /* Push-Pull */
#define GPIO_OP_TYPE_OD     1   /* Open-Drain */

/* Output Speeds */
#define GPIO_SPEED_LOW      0
#define GPIO_SPEED_MEDIUM   1   
#define GPIO_SPEED_FAST     2
#define GPIO_SPEED_HIGH     3

/* Pull-up / Pull-down */
#define GPIO_NO_PUPD        0
#define GPIO_PULL_UP        1
#define GPIO_PULL_DOWN      2
```

---

## 4. Complete Driver APIs & Implementation (`stm32f407xx_gpio_driver.c`)

### 4.1 Clock Control (`GPIO_PeriClockControl`)
Enables or disables peripheral clock gating in `RCC->AHB1ENR`:

```c
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(pGPIOx == GPIOA)      RCC_GPIOA_CLK_EN();
        else if(pGPIOx == GPIOB) RCC_GPIOB_CLK_EN();
        else if(pGPIOx == GPIOC) RCC_GPIOC_CLK_EN();
        else if(pGPIOx == GPIOD) RCC_GPIOD_CLK_EN();
        else if(pGPIOx == GPIOE) RCC_GPIOE_CLK_EN();
        else if(pGPIOx == GPIOF) RCC_GPIOF_CLK_EN();
        else if(pGPIOx == GPIOG) RCC_GPIOG_CLK_EN();
        else if(pGPIOx == GPIOH) RCC_GPIOH_CLK_EN();
        else if(pGPIOx == GPIOI) RCC_GPIOI_CLK_EN();
    }
    else
    {
        if(pGPIOx == GPIOA)      RCC_GPIOA_CLK_DI();
        else if(pGPIOx == GPIOB) RCC_GPIOB_CLK_DI();
        else if(pGPIOx == GPIOC) RCC_GPIOC_CLK_DI();
        else if(pGPIOx == GPIOD) RCC_GPIOD_CLK_DI();
        else if(pGPIOx == GPIOE) RCC_GPIOE_CLK_DI();
        else if(pGPIOx == GPIOF) RCC_GPIOF_CLK_DI();
        else if(pGPIOx == GPIOG) RCC_GPIOG_CLK_DI();
        else if(pGPIOx == GPIOH) RCC_GPIOH_CLK_DI();
        else if(pGPIOx == GPIOI) RCC_GPIOI_CLK_DI();
    }
}
```

---

### 4.2 Initialization (`GPIO_Init`)
Configures all GPIO registers according to `pGPIOHandle`:

```c
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
    uint32_t temp = 0;

    // 1. Mode Configuration
    if(pGPIOHandle->GPIO_PinConfig.mode <= GPIO_MODE_ANALOG)
    {
        // Non-interrupt mode
        temp = (pGPIOHandle->GPIO_PinConfig.mode << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber));
        pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber)); // Clear
        pGPIOHandle->pGPIOx->MODER |= temp; // Set
    }
    else
    {
        // Interrupt mode: Ensure pin is configured as input (MODER = 00)
        pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber));

        // Configure Edge Trigger in EXTI
        if(pGPIOHandle->GPIO_PinConfig.mode == GPIO_MODE_IT_FT)
        {
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.pinNumber);
            EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.pinNumber);
        }
        else if(pGPIOHandle->GPIO_PinConfig.mode == GPIO_MODE_IT_RT)
        {
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.pinNumber);
            EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.pinNumber);
        }
        else if(pGPIOHandle->GPIO_PinConfig.mode == GPIO_MODE_IT_RFT)
        {
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.pinNumber);
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.pinNumber);
        }

        // Unmask Interrupt Delivery in EXTI_IMR
        EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.pinNumber);

        // Configure SYSCFG Port Routing (EXTICR1..4)
        RCC_SYSCFG_CLK_EN();
        uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.pinNumber / 4;
        uint8_t temp2 = (pGPIOHandle->GPIO_PinConfig.pinNumber % 4) * 4;
        uint8_t portCode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

        SYSCFG->EXTICR[temp1] &= ~(0xF << temp2);
        SYSCFG->EXTICR[temp1] |= (portCode << temp2);
    }

    // 2. Speed Configuration
    temp = (pGPIOHandle->GPIO_PinConfig.speed << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber));
    pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber));
    pGPIOHandle->pGPIOx->OSPEEDR |= temp;

    // 3. Pull-Up / Pull-Down Configuration
    temp = (pGPIOHandle->GPIO_PinConfig.pupd << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber));
    pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.pinNumber));
    pGPIOHandle->pGPIOx->PUPDR |= temp;

    // 4. Output Type Configuration
    temp = (pGPIOHandle->GPIO_PinConfig.opType << (pGPIOHandle->GPIO_PinConfig.pinNumber));
    pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << (pGPIOHandle->GPIO_PinConfig.pinNumber));
    pGPIOHandle->pGPIOx->OTYPER |= temp;

    // 5. Alternate Function Configuration (if AF mode is selected)
    if(pGPIOHandle->GPIO_PinConfig.mode == GPIO_MODE_AF)    
    {
        uint8_t afrIndex    = pGPIOHandle->GPIO_PinConfig.pinNumber / 8; // 0 for AFR[0], 1 for AFR[1]
        uint8_t afrPosition = pGPIOHandle->GPIO_PinConfig.pinNumber % 8; // 0 to 7
        temp = (pGPIOHandle->GPIO_PinConfig.altFunMode << (4 * afrPosition));

        pGPIOHandle->pGPIOx->AFR[afrIndex] &= ~(0xF << (4 * afrPosition));
        pGPIOHandle->pGPIOx->AFR[afrIndex] |= temp;
    }
}
```

---

### 4.3 De-initialization (`GPIO_DeInit`)
Resets all registers of a GPIO port back to default state using **`RCC_AHB1RSTR` (Peripheral Reset Register)**:

```c
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
    if(pGPIOx == GPIOA)      { RCC->AHB1RSTR |= (1 << 0); RCC->AHB1RSTR &= ~(1 << 0); }
    else if(pGPIOx == GPIOB) { RCC->AHB1RSTR |= (1 << 1); RCC->AHB1RSTR &= ~(1 << 1); }
    else if(pGPIOx == GPIOC) { RCC->AHB1RSTR |= (1 << 2); RCC->AHB1RSTR &= ~(1 << 2); }
    else if(pGPIOx == GPIOD) { RCC->AHB1RSTR |= (1 << 3); RCC->AHB1RSTR &= ~(1 << 3); }
    else if(pGPIOx == GPIOE) { RCC->AHB1RSTR |= (1 << 4); RCC->AHB1RSTR &= ~(1 << 4); }
    else if(pGPIOx == GPIOF) { RCC->AHB1RSTR |= (1 << 5); RCC->AHB1RSTR &= ~(1 << 5); }
    else if(pGPIOx == GPIOG) { RCC->AHB1RSTR |= (1 << 6); RCC->AHB1RSTR &= ~(1 << 6); }
    else if(pGPIOx == GPIOH) { RCC->AHB1RSTR |= (1 << 7); RCC->AHB1RSTR &= ~(1 << 7); }
    else if(pGPIOx == GPIOI) { RCC->AHB1RSTR |= (1 << 8); RCC->AHB1RSTR &= ~(1 << 8); }
}
```

---

### 4.4 Pin & Port Data Read
```c
/* Read state of single input pin (0 or 1) */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
    return (uint8_t)((pGPIOx->IDR >> pinNumber) & 0x1);
}

/* Read state of entire 16-bit port */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
    return (uint16_t)(pGPIOx->IDR & 0xFFFF);
}
```

---

### 4.5 Pin & Port Data Write
```c
/* Write logic state to single output pin */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value)
{
    if(value == GPIO_PIN_SET)
        pGPIOx->ODR |= (1 << pinNumber);
    else
        pGPIOx->ODR &= ~(1 << pinNumber);
}

/* Write 16-bit word to entire port */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value)
{
    pGPIOx->ODR = value;
}
```

---

### 4.6 Output Pin Toggle (`GPIO_ToggleOutputPin`)
```c
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
    pGPIOx->ODR ^= (1 << pinNumber); // Toggle bit via XOR
}
```

---

### 4.7 Interrupt Configuration (`GPIO_IRQConfig`)
Enables or disables the IRQ in Cortex-M4 NVIC ISER / ICER registers:

```c
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    if(EnorDi == ENABLE) 
    {
        if(IRQNumber <= 31)
            *NVIC_ISER0 |= (1 << IRQNumber);
        else if(IRQNumber < 64)
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        else if(IRQNumber < 96)
            *NVIC_ISER2 |= (1 << (IRQNumber % 64));
    }
    else
    {
        if(IRQNumber <= 31)
            *NVIC_ICER0 |= (1 << IRQNumber);
        else if(IRQNumber < 64)
            *NVIC_ICER1 |= (1 << (IRQNumber % 32));
        else if(IRQNumber < 96)
            *NVIC_ICER2 |= (1 << (IRQNumber % 64));
    }
}
```

---

### 4.8 Interrupt Priority Configuration (`GPIO_IRQPriorityConfig`)
Calculates the register index and upper 4-bit shift offset for the NVIC Priority Register:

$$\text{shift\_amount} = (8 \times \text{iprx\_section}) + (8 - \text{NO\_PR\_BITS\_IMPLEMENTED})$$

```c
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    uint8_t iprx         = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;
    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

    *(NVIC_IPR_BASEADDR + iprx) &= ~(0xFF << (8 * iprx_section)); // Clear byte
    *(NVIC_IPR_BASEADDR + iprx) |= (IRQPriority << shift_amount);  // Set priority
}
```

---

### 4.9 Interrupt Handling & Clearing Flag (`GPIO_IRQHandling`)
Clears the pending flag in `EXTI_PR` by writing `1`:

```c
void GPIO_IRQHandling(uint8_t pinNumber)
{
    if(EXTI->PR & (1 << pinNumber))
    {
        EXTI->PR |= (1 << pinNumber); // Write 1 to clear pending flag
    }
}
```

---

## 5. Interrupt Subsystem Deep Dive (EXTI + SYSCFG + NVIC)

### 5.1 How SYSCFG Multiplexes Pins to EXTI Lines

In STM32, pins with the same pin number across all ports share one single EXTI line:
- `PA0, PB0, PC0, PD0...` $\rightarrow$ Share **EXTI Line 0**
- `PA1, PB1, PC1, PD1...` $\rightarrow$ Share **EXTI Line 1**
- ...
- `PA15, PB15, PC15...` $\rightarrow$ Share **EXTI Line 15**

The **SYSCFG_EXTICR** registers select which port is active for each line:
- `EXTICR[0]`: Pins 0, 1, 2, 3
- `EXTICR[1]`: Pins 4, 5, 6, 7
- `EXTICR[2]`: Pins 8, 9, 10, 11
- `EXTICR[3]`: Pins 12, 13, 14, 15

Each line takes a 4-bit port code: `0000 = Port A`, `0001 = Port B`, `0010 = Port C`, etc.

---

### 5.2 EXTI Line to Cortex-M4 ISR Vector Mapping

| Pins | EXTI Line | IRQ Number | Startup Vector Table ISR Function Name |
| :--- | :--- | :--- | :--- |
| `P_0` (PA0, PB0, ...) | Line 0 | 6 | `EXTI0_IRQHandler(void)` |
| `P_1` (PA1, PB1, ...) | Line 1 | 7 | `EXTI1_IRQHandler(void)` |
| `P_2` (PA2, PB2, ...) | Line 2 | 8 | `EXTI2_IRQHandler(void)` |
| `P_3` (PA3, PB3, ...) | Line 3 | 9 | `EXTI3_IRQHandler(void)` |
| `P_4` (PA4, PB4, ...) | Line 4 | 10 | `EXTI4_IRQHandler(void)` |
| `P_5` to `P_9` | Lines 5–9 | 23 | `EXTI9_5_IRQHandler(void)` *(Shared)* |
| `P_10` to `P_15` | Lines 10–15 | 40 | `EXTI15_10_IRQHandler(void)` *(Shared)* |

---

## 6. Critical Debugging Lessons & Embedded C Gotchas

### 1. Pointer Arithmetic & Macro Parentheses
- **Bug:** `#define RCC_BASEADDR AHB1_BASEADDR + 0x3800`
- **Root cause:** `(RCC_RegDef_t*)AHB1_BASEADDR + 0x3800` evaluates typecast before addition, performing pointer arithmetic ($14,336 \times 136\text{ bytes} \approx 1.9\text{ MB}$ offset).
- **Solution:** Always wrap arithmetic additions in parentheses: `(AHB1_BASEADDR + 0x3800U)`.

### 2. Peripheral Clock Bus Locations
- **Bug:** Enabling SYSCFG on `RCC->AHB1ENR`.
- **Root cause:** SYSCFG is on the **APB2 bus** at bit 14 of `RCC->APB2ENR`.
- **Solution:** `#define RCC_SYSCFG_CLK_EN() (RCC->APB2ENR |= (1 << 14))`.

### 3. Missing `volatile` in Delay Loops
- **Bug:** `for(int i = 0; i < 500000; i++);`
- **Root cause:** Compiler optimization detects no side-effects and strips the loop away, causing the LED to toggle at CPU clock speed ($168\text{ MHz}$).
- **Solution:** Use `volatile uint32_t i`.

### 4. Dereferencing Pointer Constants
- **Bug:** `NVIC_ISER0 |= (1 << IRQNumber);` when `NVIC_ISER0` is defined as `(volatile uint32_t*)0xE000E100`.
- **Root cause:** Attempting to assign to a pointer value itself rather than the memory address it points to.
- **Solution:** Dereference with `*NVIC_ISER0 |= ...` or define macro with `*` (`(*((volatile uint32_t*)0xE000E100))`).

### 5. `EXTI_PR` Flag Clearing (`rc_w1`)
- **Bug:** Clearing flag with `EXTI->PR &= ~(1 << pinNumber);`
- **Root cause:** STM32 EXTI Pending Register bits are **cleared by writing `1`**, not `0`.
- **Solution:** `EXTI->PR |= (1 << pinNumber);`.

---

## 7. Full Application Examples

### Example: External Interrupt Button Toggle (`002led_button.c`)

```c
#include <stdint.h>
#include "stm32f407xx_gpio_driver.h"

int main(void)
{
    // 1. Configure LED (PA6, Output, Push-Pull)
    GPIO_Handle_t gpioLed = {0};
    gpioLed.pGPIOx = GPIOA;
    gpioLed.GPIO_PinConfig.pinNumber = GPIO_PIN_6;
    gpioLed.GPIO_PinConfig.mode      = GPIO_MODE_OUTPUT;
    gpioLed.GPIO_PinConfig.speed     = GPIO_SPEED_FAST;
    gpioLed.GPIO_PinConfig.opType    = GPIO_OP_TYPE_PP;
    gpioLed.GPIO_PinConfig.pupd      = GPIO_NO_PUPD;

    // 2. Configure Button on PA0 (Input, Falling Edge Interrupt)
    GPIO_Handle_t gpioButton = {0};
    gpioButton.pGPIOx = GPIOA;
    gpioButton.GPIO_PinConfig.pinNumber = GPIO_PIN_0;
    gpioButton.GPIO_PinConfig.mode      = GPIO_MODE_IT_FT; // Falling edge
    gpioButton.GPIO_PinConfig.speed     = GPIO_SPEED_FAST;
    gpioButton.GPIO_PinConfig.pupd      = GPIO_NO_PUPD;

    // 3. Initialize Peripherals
    GPIO_PeriClockControl(GPIOA, ENABLE);
    GPIO_Init(&gpioLed);
    GPIO_Init(&gpioButton);

    // 4. Configure NVIC IRQ Priority & Enable Interrupt
    GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, 15);
    GPIO_IRQConfig(IRQ_NO_EXTI0, ENABLE);

    while(1)
    {
        // CPU remains idle; toggling happens inside the ISR
    }

    return 0;
}

// 5. ISR executed when button is clicked
void EXTI0_IRQHandler(void)
{
    for(volatile uint32_t i = 0; i < 50000; i++); // Debounce delay
    GPIO_IRQHandling(GPIO_PIN_0);                 // Clear pending flag in EXTI_PR
    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_6);      // Toggle LED
}
```
