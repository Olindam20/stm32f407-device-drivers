/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: Sep 12, 2026
 *      Author: OLINDAM SARMA
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include <stm32f407xx.h>

#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */

/* config structure for a gpio pin */
typedef struct
{
    uint8_t pinNumber;      /* Pin number */
    uint8_t mode;           /* Pin mode */
    uint8_t speed;          /* Pin speed */
    uint8_t opType;         /* Pin output type */
    uint8_t pupd;           /* Pin pull-up/pull-down */
    uint8_t altFunMode;     /* Alternate function mode */
}GPIO_PinConfig_t;

/* handle structure for GPIO pins */
typedef struct
{
    GPIO_RegDef_t *pGPIOx; /* Base address of the GPIO port */
    GPIO_PinConfig_t GPIO_PinConfig; /* GPIO pin configuration settings */
    
}GPIO_Handle_t; 

/* Pin numbers */
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
#define GPIO_MODE_IT_FT     4
#define GPIO_MODE_IT_RT     5
#define GPIO_MODE_IT_RFT    6

/*output types*/
#define GPIO_OP_TYPE_PP     0
#define GPIO_OP_TYPE_OD     1   

/* GPIO Output speed*/
#define GPIO_SPEED_LOW      0
#define GPIO_SPEED_MEDIUM   1   
#define GPIO_SPEED_FAST     2
#define GPIO_SPEED_HIGH     3

/* GPIO Pull-up/Pull-down */
#define GPIO_NO_PUPD        0
#define GPIO_PULL_UP        1
#define GPIO_PULL_DOWN      2


/*
 * ====================================================================
 *                 APIs supported by this GPIO driver
 * ====================================================================
 */

/* Peripheral Clock Setup */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/* Init and De-Init */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/* Data Read and Write */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);

/* IRQ Configuration and ISR Handling */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t pinNumber);