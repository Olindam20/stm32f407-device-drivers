# STM32F407 Custom Device Drivers

Custom bare-metal device drivers for the **STM32F407VET6** MCU (ARM Cortex-M4) written in C from scratch without using vendor HAL or standard peripheral libraries.

---

## 📁 Repository Structure
```
├── drivers/
│   ├── Inc/
│   │   ├── stm32f407xx.h             # MCU-specific base addresses, register structs & macros
│   │   └── stm32f407xx_gpio_driver.h # GPIO driver handle, structures & API declarations
│   └── Src/
│       └── stm32f407xx_gpio_driver.c # GPIO driver API implementation
├── Src/
│   ├── 001led_toggle.c               # Application 1: Software delay polling LED toggle
│   └── 002led_button.c               # Application 2: Interrupt-driven button LED toggle (EXTI)
├── Startup/
│   └── startup_stm32f407vetx.s       # Vector table and Reset_Handler
├── STM32F407VETX_FLASH.ld            # Linker script
└── GPIO_Driver_Documentation.md      # Complete technical documentation & study guide
```

---

## 🚀 Supported GPIO Features & APIs

### 1. Clock Management
- `GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)`: Enables/disables AHB1 clock gating for GPIO ports A through I.

### 2. Initialization & Reset
- `GPIO_Init(GPIO_Handle_t *pGPIOHandle)`: Configures Pin Mode (Input, Output, Alternate Function, Analog, or EXTI Interrupts), Output Speed, Pull-up/Pull-down, and Output Type (Push-Pull/Open-Drain).
- `GPIO_DeInit(GPIO_RegDef_t *pGPIOx)`: Resets entire GPIO port using `RCC_AHB1RSTR`.

### 3. Read & Write APIs
- `GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)`: Reads single-pin binary logic state.
- `GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)`: Reads full 16-bit port value from `IDR`.
- `GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value)`: Writes HIGH/LOW to single pin.
- `GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value)`: Writes 16-bit word to `ODR`.
- `GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)`: Fast atomic XOR toggle on `ODR`.

### 4. Interrupt & NVIC APIs (EXTI + SYSCFG + NVIC)
- `GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi)`: Enables/disables interrupt vector in ARM Cortex-M4 `NVIC_ISER` / `NVIC_ICER`.
- `GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)`: Sets interrupt priority level (0–15) using upper 4 bits in `NVIC_IPR`.
- `GPIO_IRQHandling(uint8_t pinNumber)`: Clears pending interrupt flag in `EXTI_PR` (`rc_w1`).

---

## 📖 Complete Documentation
For full register bit maps, formulas, and architecture diagrams, refer to **[`GPIO_Driver_Documentation.md`](GPIO_Driver_Documentation.md)**.

