/*
 * stm32f407xx.h
 *
 *  Created on: Sep 11, 2026
 *      Author: OLINDAM SARMA
 */
#include<stdint.h>
#include<stdio.h>

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_
#define __vo volatile

/* processor specific details */
#define NVIC_ISER0          ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1          ((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2          ((volatile uint32_t*)0xE000E108)
#define NVIC_ICER0          ((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1          ((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2          ((volatile uint32_t*)0xE000E188)
#define NVIC_IPR_BASEADDR   ((volatile uint32_t*)0xE000E400)
#define NO_PR_BITS_IMPLEMENTED 4
/*
 base address of flash and sram
*/
#define FLASH_BASEADDR     0x08000000U
#define SRAM1_BASEADDR	   0x20000000U
#define SRAM2_BASEADDR	   (SRAM1_BASEADDR + (112 * 1024))
#define ROM_BASEADDR       0x1FFF0000U
#define SRAM_BASEADDR      SRAM1_BASEADDR



/*
 base address of AHBx and APBx
*/
#define PERIPH_BASEADDR    0x40000000U
#define APB1_BASEADDR      PERIPH_BASEADDR
#define APB2_BASEADDR      0x40010000U
#define AHB1_BASEADDR      0x40020000U 
#define AHB2_BASEADDR      0x50000000U


/*
 base address of peripherels on AHB1bus
*/
#define GPIOA_BASEADDR     (AHB1_BASEADDR + 0x0000U)
#define GPIOB_BASEADDR     (AHB1_BASEADDR + 0x0400U)   
#define GPIOC_BASEADDR     (AHB1_BASEADDR + 0x0800U)
#define GPIOD_BASEADDR     (AHB1_BASEADDR + 0x0C00U)
#define GPIOE_BASEADDR     (AHB1_BASEADDR + 0x1000U)
#define GPIOF_BASEADDR     (AHB1_BASEADDR + 0x1400U) 
#define GPIOG_BASEADDR     (AHB1_BASEADDR + 0x1800U)
#define GPIOH_BASEADDR     (AHB1_BASEADDR + 0x1C00U)
#define GPIOI_BASEADDR     (AHB1_BASEADDR + 0x2000U)
#define RCC_BASEADDR       (AHB1_BASEADDR + 0x3800U)

/*
 base address of peripherels on APB1bus
*/
#define I2C1_BASEADDR      (APB1_BASEADDR + 0x5400U)
#define I2C2_BASEADDR      (APB1_BASEADDR + 0x5800U)
#define I2C3_BASEADDR      (APB1_BASEADDR + 0x5C00U)

#define SPI2_BASEADDR      (APB1_BASEADDR + 0x3800U)
#define SPI3_BASEADDR      (APB1_BASEADDR + 0x3C00U)

#define USART2_BASEADDR    (APB1_BASEADDR + 0x4400U)
#define USART3_BASEADDR    (APB1_BASEADDR + 0x4800U)
#define USART5_BASEADDR     (APB1_BASEADDR + 0x5000U)

/*
 base address of peripherels on APB2bus
*/
#define EXTI_BASEADDR      (APB2_BASEADDR + 0x3C00U)

#define SPI1_BASEADDR      (APB2_BASEADDR + 0x3000U)
#define SPI4_BASEADDR      (APB2_BASEADDR + 0x3400U)

#define USART1_BASEADDR    (APB2_BASEADDR + 0x1000U)
#define USART6_BASEADDR    (APB2_BASEADDR + 0x1400U)

#define SYSCFG_BASEADDR    (APB2_BASEADDR + 0x3800U)




/* Peripherel register defination structure */
typedef struct
{
    __vo uint32_t MODER;        /*!< GPIO port mode register,               Address offset: 0x00      */
    __vo uint32_t OTYPER;       /*!< GPIO port output type register,        Address offset: 0x04      */
    __vo uint32_t OSPEEDR;      /*!< GPIO port output speed register,       Address offset: 0x08      */
    __vo uint32_t PUPDR;        /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    __vo uint32_t IDR;          /*!< GPIO port input data register,         Address offset: 0x10      */
    __vo uint32_t ODR;          /*!< GPIO port output data register,        Address offset: 0x14      */
    __vo uint32_t BSRR;         /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
    __vo uint32_t LCKR;         /*!< GPIO port configuration lock register, Address offset: 0x1C      */
    __vo uint32_t AFR[2];       /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */ 
}GPIO_RegDef_t;

typedef struct
{
    __vo uint32_t CR;            /*!< RCC clock control register,                                  Address offset: 0x00 */
    __vo uint32_t PLLCFGR;       /*!< RCC PLL configuration register,                              Address offset: 0x04 */
    __vo uint32_t CFGR;          /*!< RCC clock configuration register,                            Address offset: 0x08 */
    __vo uint32_t CIR;           /*!< RCC clock interrupt register,                                Address offset: 0x0C */
    __vo uint32_t AHB1RSTR;      /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
    __vo uint32_t AHB2RSTR;      /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
    __vo uint32_t AHB3RSTR;      /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
         uint32_t RESERVED0;     /*!< Reserved,                                                    Address offset: 0x1C */       
    __vo uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
    __vo uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
         uint32_t RESERVED1[2];  /*!< Reserved,                                                    Address offset: 0x28-0x2C */ 
    __vo uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock enable register,                   Address offset: 0x30 */
    __vo uint32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock enable register,                   Address offset: 0x34 */
    __vo uint32_t AHB3ENR;       /*!< RCC AHB3 peripheral clock enable register,                   Address offset: 0x38 */
         uint32_t RESERVED2;     /*!< Reserved,                                                    Address offset: 0x3C */  
    __vo uint32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
    __vo uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
         uint32_t RESERVED3[2];  /*!< Reserved,                                                    Address offset: 0x48-0x4C */
    __vo uint32_t AHB1LPENR;     /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
    __vo uint32_t AHB2LPENR;     /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
    __vo uint32_t AHB3LPENR;     /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
         uint32_t RESERVED4;     /*!< Reserved,                                                    Address offset: 0x5C */
    __vo uint32_t APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
    __vo uint32_t APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
         uint32_t RESERVED5[2];  /*!< Reserved,                                                    Address offset: 0x68-0x6C */     
    __vo uint32_t BDCR;          /*!< RCC Backup domain control register,                          Address offset: 0x70 */
    __vo uint32_t CSR;           /*!< RCC clock control & status register,                         Address offset: 0x74 */
         uint32_t RESERVED6[2];  /*!< Reserved,                                                    Address offset: 0x78-0x7C */
    __vo uint32_t SSCGR;         /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
    __vo uint32_t PLLI2SCFGR;    /*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */
} RCC_RegDef_t;

typedef struct
{
    __vo uint32_t IMR;   /*!< Interrupt mask register,                     Address offset: 0x00 */
    __vo uint32_t EMR;   /*!< Event mask register,                         Address offset: 0x04 */
    __vo uint32_t RTSR;  /*!< Rising trigger selection register,           Address offset: 0x08 */
    __vo uint32_t FTSR;  /*!< Falling trigger selection register,          Address offset: 0x0C */
    __vo uint32_t SWIER; /*!< Software interrupt enable register,          Address offset: 0x10 */
    __vo uint32_t PR;    /*!< Pending register,                            Address offset: 0x14 */
}EXTI_RegDef_t;

typedef struct{
     
    __vo uint32_t MEMRMP;       /*!< SYSCFG memory remap register,                      Address offset: 0x00 */
    __vo uint32_t PMC;          /*!< SYSCFG peripheral mode configuration register,     Address offset: 0x04 */
    __vo uint32_t EXTICR[4];    /*!< SYSCFG external interrupt configuration registers, Address offset: 0x08-0x14 */
         uint32_t RESERVED1[2]; /*!< Reserved,                                           Address offset: 0x18-0x1C */
    __vo uint32_t CMPCR;        /*!< Compensation cell control register,                Address offset: 0x20 */
}
SYSCFG_RegDef_t;

typedef struct
{
    __vo uint32_t CR1;         /*!< SPI control register 1,            Address offset: 0x00 */
    __vo uint32_t CR2;         /*!< SPI control register 2,            Address offset: 0x04 */
    __vo uint32_t SR;          /*!< SPI status register,                Address offset: 0x08 */
    __vo uint32_t DR;          /*!< SPI data register,                  Address offset: 0x0C */
    __vo uint32_t CRCPR;       /*!< SPI CRC polynomial register,        Address offset: 0x10 */
    __vo uint32_t RXCRCR;      /*!< SPI RX CRC register,                Address offset: 0x14 */
    __vo uint32_t TXCRCR;      /*!< SPI TX CRC register,                Address offset: 0x18 */
    __vo uint32_t I2SCFGR;     /*!< SPI_I2S configuration register,     Address offset: 0x1C */
    __vo uint32_t I2SPR;       /*!< SPI_I2S prescaler register,         Address offset: 0x20 */
    __vo uint32_t RESERVED[3];  /*!< Reserved,                           Address offset: 0x24-0x2C */
}SPI_RegDef_t;

typedef struct
{
     __vo uint32_t CR1;       /*!< I2C Control register 1,         Address offset: 0x00 */
     __vo uint32_t CR2;       /*!< I2C Control register 2,         Address offset: 0x04 */
     __vo uint32_t OAR1;      /*!< I2C Own address register 1,     Address offset: 0x08 */
     __vo uint32_t OAR2;      /*!< I2C Own address register 2,     Address offset: 0x0C */
     __vo uint32_t DR;        /*!< I2C Data register,               Address offset: 0x10 */
     __vo uint32_t SR1;       /*!< I2C Status register 1,           Address offset: 0x14 */
     __vo uint32_t SR2;       /*!< I2C Status register 2,           Address offset: 0x18 */
     __vo uint32_t CCR;       /*!< I2C Clock control register,      Address offset: 0x1C */
     __vo uint32_t TRISE;     /*!< I2C TRISE register,               Address offset: 0x20 */
     __vo uint32_t FLTR;      /*!< I2C FLTR register,                Address offset: 0x24 */
}I2C_RegDef_t;


typedef struct
{
     __vo uint32_t SR;         /*!< USART Status register,            Address offset: 0x00 */
     __vo uint32_t DR;         /*!< USART Data register,              Address offset: 0x04 */
     __vo uint32_t BRR;        /*!< USART Baud rate register,         Address offset: 0x08 */
     __vo uint32_t CR1;        /*!< USART Control register 1,         Address offset: 0x0C */
     __vo uint32_t CR2;        /*!< USART Control register 2,         Address offset: 0x10 */
     __vo uint32_t CR3;        /*!< USART Control register 3,         Address offset: 0x14 */
     __vo uint32_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
}USART_RegDef_t;

/* peripheral definitions (GPIO) */
#define GPIOA               ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB               ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC               ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD               ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE               ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF               ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG               ((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH               ((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI               ((GPIO_RegDef_t*)GPIOI_BASEADDR)

/* peripheral definitions (RCC) */
#define RCC                 ((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI                ((EXTI_RegDef_t*)EXTI_BASEADDR)

#define SYSCFG              ((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)


/* peripheral definitions (SPI) */
#define SPI1                ((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2                ((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3                ((SPI_RegDef_t*)SPI3_BASEADDR)
#define SPI4                ((SPI_RegDef_t*)SPI4_BASEADDR)



/*peripheral definitions (I2C) */
#define I2C1                ((I2C_RegDef_t*)I2C1_BASEADDR)
#define I2C2                ((I2C_RegDef_t*)I2C2_BASEADDR)
#define I2C3                ((I2C_RegDef_t*)I2C3_BASEADDR)
#define I2C4                ((I2C_RegDef_t*)I2C4_BASEADDR)


/*peripheral definitions (USART) */
#define USART1              ((USART_RegDef_t*)USART1_BASEADDR)
#define USART2              ((USART_RegDef_t*)USART2_BASEADDR)
#define USART3              ((USART_RegDef_t*)USART3_BASEADDR)
#define USART4              ((USART_RegDef_t*)USART4_BASEADDR)
#define USART5              ((USART_RegDef_t*)USART5_BASEADDR)
#define USART6              ((USART_RegDef_t*)USART6_BASEADDR)


/*Clock enable macros for GPIOx peripherals */
#define RCC_GPIOA_CLK_EN()          (RCC->AHB1ENR |= (1 << 0))
#define RCC_GPIOB_CLK_EN()          (RCC->AHB1ENR |= (1 << 1))
#define RCC_GPIOC_CLK_EN()          (RCC->AHB1ENR |= (1 << 2))
#define RCC_GPIOD_CLK_EN()          (RCC->AHB1ENR |= (1 << 3))
#define RCC_GPIOE_CLK_EN()          (RCC->AHB1ENR |= (1 << 4))
#define RCC_GPIOF_CLK_EN()          (RCC->AHB1ENR |= (1 << 5))
#define RCC_GPIOG_CLK_EN()          (RCC->AHB1ENR |= (1 << 6))
#define RCC_GPIOH_CLK_EN()          (RCC->AHB1ENR |= (1 << 7))
#define RCC_GPIOI_CLK_EN()          (RCC->AHB1ENR |= (1 << 8))

/*clock disable macros for gpio peripherals */
#define RCC_GPIOA_CLK_DI()          (RCC->AHB1ENR &= ~(1 << 0))
#define RCC_GPIOB_CLK_DI()          (RCC->AHB1ENR &= ~(1 << 1))
#define RCC_GPIOC_CLK_DI()          (RCC->AHB1ENR &= ~(1 << 2))
#define RCC_GPIOD_CLK_DI()          (RCC->AHB1ENR &= ~(1 << 3))
#define RCC_GPIOE_CLK_DI()          (RCC->AHB1ENR &= ~(1 << 4))
#define RCC_GPIOF_CLK_DI()          (RCC->AHB1ENR &= ~(1 << 5))
#define RCC_GPIOG_CLK_DI()          (RCC->AHB1ENR &= ~(1 << 6))
#define RCC_GPIOH_CLK_DI()          (RCC->AHB1ENR &= ~(1 << 7))
#define RCC_GPIOI_CLK_DI()          (RCC->AHB1ENR &= ~(1 << 8))


/*clock enable macros for i2c peripherals */
#define RCC_I2C1_CLK_EN()           (RCC->APB1ENR |= (1 << 21))
#define RCC_I2C2_CLK_EN()           (RCC->APB1ENR |= (1 << 22))
#define RCC_I2C3_CLK_EN()           (RCC->APB1ENR |= (1 << 23))

/*clock disable macros for i2c peripherals */
#define RCC_I2C1_CLK_DI()           (RCC->APB1ENR &= ~(1 << 21))
#define RCC_I2C2_CLK_DI()           (RCC->APB1ENR &= ~(1 << 22))
#define RCC_I2C3_CLK_DI()           (RCC->APB1ENR &= ~(1 << 23)) 

/*clock enable macros for spi peripherals */
#define RCC_SPI1_CLK_EN()           (RCC->APB2ENR |= (1 << 12))
#define RCC_SPI2_CLK_EN()           (RCC->APB1ENR |= (1 << 14))
#define RCC_SPI3_CLK_EN()           (RCC->APB1ENR |= (1 << 15))

/*clock disable macros for spi peripherals */
#define RCC_SPI1_CLK_DI()           (RCC->APB2ENR &= ~(1 << 12))
#define RCC_SPI2_CLK_DI()           (RCC->APB1ENR &= ~(1 << 14))
#define RCC_SPI3_CLK_DI()           (RCC->APB1ENR &= ~(1 << 15))

/*clock enable macros for uart peripherals */
#define RCC_USART1_CLK_EN()          (RCC->APB2ENR |= (1 << 4))
#define RCC_USART2_CLK_EN()          (RCC->APB1ENR |= (1 << 17))
#define RCC_USART3_CLK_EN()          (RCC->APB1ENR |= (1 << 18))
#define RCC_USART4_CLK_EN()          (RCC->APB1ENR |= (1 << 19))
#define RCC_USART5_CLK_EN()          (RCC->APB1ENR |= (1 << 20))
#define RCC_USART6_CLK_EN()          (RCC->APB2ENR |= (1 << 5))

/*clock disable macros for uart peripherals */
#define RCC_USART1_CLK_DI()          (RCC->APB2ENR &= ~(1 << 4))
#define RCC_USART2_CLK_DI()          (RCC->APB1ENR &= ~(1 << 17))
#define RCC_USART3_CLK_DI()          (RCC->APB1ENR &= ~(1 << 18))
#define RCC_USART4_CLK_DI()          (RCC->APB1ENR &= ~(1 << 19))
#define RCC_USART5_CLK_DI()          (RCC->APB1ENR &= ~(1 << 20))
#define RCC_USART6_CLK_DI()          (RCC->APB2ENR &= ~(1 << 5))






/*clock enable for SYSCFG*/
#define RCC_SYSCFG_CLK_EN()         (RCC->APB2ENR |= (1 << 14))   
/*clock disable for SYSCFG*/
#define RCC_SYSCFG_CLK_DI()         (RCC->APB2ENR &= ~(1 << 14))

#define ENABLE                      1
#define DISABLE                     0
#define SET                         ENABLE
#define RESET                       DISABLE
#define FLAG_RESET                  RESET
#define FLAG_SET                    SET

/* Macro to convert GPIO base address to port code */
#define GPIO_BASEADDR_TO_CODE(pGPIOx) (((pGPIOx) == GPIOA) ? 0 : \
                                        ((pGPIOx) == GPIOB) ? 1 : \
                                        ((pGPIOx) == GPIOC) ? 2 : \
                                        ((pGPIOx) == GPIOD) ? 3 : \
                                        ((pGPIOx) == GPIOE) ? 4 : \
                                        ((pGPIOx) == GPIOF) ? 5 : \
                                        ((pGPIOx) == GPIOG) ? 6 : \
                                        ((pGPIOx) == GPIOH) ? 7 : \
                                        ((pGPIOx) == GPIOI) ? 8 : 9)

/*IRQ Numbers for MCU STM32F407x*/
#define IRQ_NO_EXTI0        6
#define IRQ_NO_EXTI1        7      
#define IRQ_NO_EXTI2        8
#define IRQ_NO_EXTI3        9
#define IRQ_NO_EXTI4        10
#define IRQ_NO_EXTI9_5      23
#define IRQ_NO_EXTI15_10    40



/*IRQ for SPI*/
#define IRQ_NO_SPI1         35
#define IRQ_NO_SPI2         36
#define IRQ_NO_SPI3         51


/*
 * ====================================================================
 *         Bit position definitions of I2C peripheral registers
 * ====================================================================
 */

/* Bit positions for I2C_CR1 register */
#define I2C_CR1_PE            0
#define I2C_CR1_SMBUS         1
#define I2C_CR1_SMBTYPE       3
#define I2C_CR1_ENARP         4
#define I2C_CR1_ENPEC         5
#define I2C_CR1_ENGC          6
#define I2C_CR1_NOSTRETCH     7
#define I2C_CR1_START         8
#define I2C_CR1_STOP          9
#define I2C_CR1_ACK           10
#define I2C_CR1_POS           11
#define I2C_CR1_PEC           12
#define I2C_CR1_ALERT         13
#define I2C_CR1_SWRST         15

/* Bit positions for I2C_CR2 register */
#define I2C_CR2_FREQ          0
#define I2C_CR2_ITERREN       8
#define I2C_CR2_ITEVTEN       9
#define I2C_CR2_ITBUFEN       10
#define I2C_CR2_DMAEN         11
#define I2C_CR2_LAST          12

/* Bit positions for I2C_OAR1 register */
#define I2C_OAR1_ADD0         0
#define I2C_OAR1_ADD7_1       1
#define I2C_OAR1_ADD9_8       8
#define I2C_OAR1_ADDMODE      15

/* Bit positions for I2C_OAR2 register */
#define I2C_OAR2_ENDUAL       0
#define I2C_OAR2_ADD2         1

/* Bit positions for I2C_SR1 register */
#define I2C_SR1_SB            0
#define I2C_SR1_ADDR          1
#define I2C_SR1_BTF           2
#define I2C_SR1_ADD10         3
#define I2C_SR1_STOPF         4
#define I2C_SR1_RXNE          6
#define I2C_SR1_TXE           7
#define I2C_SR1_BERR          8
#define I2C_SR1_ARLO          9
#define I2C_SR1_AF            10
#define I2C_SR1_OVR           11
#define I2C_SR1_PECERR        12
#define I2C_SR1_TIMEOUT       14
#define I2C_SR1_SMBALERT      15

/* Bit positions for I2C_SR2 register */
#define I2C_SR2_MSL           0
#define I2C_SR2_BUSY          1
#define I2C_SR2_TRA           2
#define I2C_SR2_GENCALL       4
#define I2C_SR2_SMBDEFAULT    5
#define I2C_SR2_SMBHOST       6
#define I2C_SR2_DUALF         7
#define I2C_SR2_PEC           8

/* Bit positions for I2C_CCR register */
#define I2C_CCR_CCR           0
#define I2C_CCR_DUTY          14
#define I2C_CCR_FS            15

/* Bit positions for I2C_TRISE register */
#define I2C_TRISE_TRISE       0

/* Bit positions for I2C_FLTR register */
#define I2C_FLTR_DNF          0
#define I2C_FLTR_ANOFF        4

/* IRQ Numbers for I2C */
#define IRQ_NO_I2C1_EV        31
#define IRQ_NO_I2C1_ER        32
#define IRQ_NO_I2C2_EV        33
#define IRQ_NO_I2C2_ER        34
#define IRQ_NO_I2C3_EV        72
#define IRQ_NO_I2C3_ER        73



/*I2C application states*/
#define I2C_READY             0
#define I2C_BUSY_RX          1
#define I2C_BUSY_TX           2




/*
 * ====================================================================
 *         Bit position definitions of USART peripheral registers
 * ====================================================================
 */

/* Bit positions for USART_SR register */
#define USART_SR_PE            0
#define USART_SR_FE            1
#define USART_SR_NF            2
#define USART_SR_ORE           3
#define USART_SR_IDLE          4
#define USART_SR_RXNE          5
#define USART_SR_TC            6
#define USART_SR_TXE           7
#define USART_SR_LBD           8
#define USART_SR_CTS           9

/* Bit positions for USART_BRR register */
#define USART_BRR_DIV_Fraction 0
#define USART_BRR_DIV_Mantissa 4

/* Bit positions for USART_CR1 register */
#define USART_CR1_SBK          0
#define USART_CR1_RWU          1
#define USART_CR1_RE           2
#define USART_CR1_TE           3
#define USART_CR1_IDLEIE       4
#define USART_CR1_RXNEIE       5
#define USART_CR1_TCIE         6
#define USART_CR1_TXEIE        7
#define USART_CR1_PEIE         8
#define USART_CR1_PS           9
#define USART_CR1_PCE          10
#define USART_CR1_WAKE         11
#define USART_CR1_M            12
#define USART_CR1_UE           13
#define USART_CR1_OVER8        15

/* Bit positions for USART_CR2 register */
#define USART_CR2_ADD          0
#define USART_CR2_LBDL         5
#define USART_CR2_LBDIE        6
#define USART_CR2_LBCL         8
#define USART_CR2_CPHA         9
#define USART_CR2_CPOL         10
#define USART_CR2_CLKEN        11
#define USART_CR2_STOP         12
#define USART_CR2_LINEN        14

/* Bit positions for USART_CR3 register */
#define USART_CR3_EIE          0
#define USART_CR3_IREN         1
#define USART_CR3_IRLP         2
#define USART_CR3_HDSEL        3
#define USART_CR3_NACK         4
#define USART_CR3_SCEN         5
#define USART_CR3_DMAR         6
#define USART_CR3_DMAT         7
#define USART_CR3_RTSE         8
#define USART_CR3_CTSE         9
#define USART_CR3_CTSIE        10
#define USART_CR3_ONEBIT       11

/* IRQ Numbers for USART/UART */
#define IRQ_NO_USART1          37
#define IRQ_NO_USART2          38
#define IRQ_NO_USART3          39
#define IRQ_NO_UART4           52
#define IRQ_NO_UART5           53
#define IRQ_NO_USART6          71

/* USART Application States */
#define USART_BUSY_IN_RX       1
#define USART_BUSY_IN_TX       2
#define USART_READY            0

/* USART Application Events */
#define USART_EVENT_TX_CMPLT   0
#define USART_EVENT_RX_CMPLT   1
#define USART_EVENT_IDLE       2
#define USART_EVENT_CTS        3
#define USART_EVENT_PE         4
#define USART_ERR_FE           5
#define USART_ERR_NE           6
#define USART_ERR_ORE          7

#endif /* INC_STM32F407XX_H_ */