# STM32F407 Bare-Metal Peripheral Device Drivers

[![Target](https://img.shields.io/badge/Target-STM32F407VET6-blue.svg)](https://www.st.com/en/microcontrollers-microprocessors/stm32f407ve.html)
[![Architecture](https://img.shields.io/badge/Architecture-ARM%20Cortex--M4%20(168%20MHz)-green.svg)]()
[![Standard](https://img.shields.io/badge/Standard-CMSIS%20C11%20Bare--Metal-orange.svg)]()

Complete, production-quality, bare-metal peripheral device drivers developed from scratch for the **STM32F407VET6** MCU (ARM Cortex-M4) in pure C without using STM32Cube HAL or standard peripheral libraries.

---

## 📁 Repository Structure

```
stm32f4xx_drivers/
├── drivers/
│   ├── Inc/
│   │   ├── stm32f407xx.h               # Base addresses, peripheral register defs & bit macros
│   │   ├── stm32f407xx_gpio_driver.h   # GPIO driver API declarations & configuration structs
│   │   ├── stm32f407xx_spi_driver.h    # SPI driver API declarations & configuration structs
│   │   ├── stm32f407xx_i2c_driver.h    # I2C driver API declarations & configuration structs
│   │   └── stm32f407xx_usart_driver.h  # USART driver API declarations & configuration structs
│   └── Src/
│       ├── stm32f407xx_gpio_driver.c   # GPIO driver implementation
│       ├── stm32f407xx_spi_driver.c    # SPI driver implementation
│       ├── stm32f407xx_i2c_driver.c    # I2C driver implementation (Polling + Interrupts)
│       └── stm32f407xx_usart_driver.c  # USART driver implementation (Polling + Interrupts)
├── Src/
│   ├── 001led_toggle.c                 # GPIO: Software delay polling LED toggle
│   ├── 002led_button.c                 # GPIO: Interrupt-driven button toggle (EXTI + NVIC)
│   ├── 005_spi_tx_testing.c            # SPI: Master TX verification
│   ├── 006_spi_txonly_arduino.c        # SPI: Master TX to Arduino Uno slave
│   ├── 007_spi_cmd_handling.c          # SPI: Command-response communication with Arduino
│   ├── 008_spi_cmd_handling_it.c       # SPI: Non-blocking interrupt-based communication
│   ├── 009_i2c_tx.c                    # I2C: Master TX testing to Arduino slave (0x68)
│   ├── 010_i2c_rx.c                    # I2C: Master RX length & string reception
│   └── 011_usart_tx.c                  # USART: 115200 bps Serial transmission to PC / Arduino
├── Startup/
│   └── startup_stm32f407vetx.s         # Vector table & Reset Handler
├── STM32F407VETX_FLASH.ld              # Linker script
├── stm32f407_driver_development_master_guide.md # Comprehensive theory, architecture & interview guide
└── README.md
```

---

## 🛠️ Implemented Driver Modules

### 1. GPIO Driver (`stm32f407xx_gpio_driver.c/h`)
* **Modes**: Input, Output, Alternate Function (`AF0`-`AF15`), Analog, and EXTI Interrupt modes (Rising, Falling, Rising-Falling).
* **Speed & Output Types**: Low, Medium, Fast, High speed; Push-Pull and Open-Drain output stages.
* **Pull-Up / Pull-Down**: Internal pull-up, pull-down, or floating configuration.
* **Interrupt Engine**: Integrated with `SYSCFG_EXTICR` routing, `EXTI` edge triggers/masking, and Cortex-M4 `NVIC` prioritization.

### 2. SPI Driver (`stm32f407xx_spi_driver.c/h`)
* **Modes & Duplex**: Master & Slave modes; Full-Duplex, Half-Duplex (1-line bidirectional), and Simplex (TX/RX-only).
* **Clock Dynamics**: All 4 SPI Modes supported via Clock Polarity (`CPOL`) and Clock Phase (`CPHA`).
* **Slave Select**: Software Slave Management (`SSM = 1`, `SSI = 1`) to eliminate MODF bus faults, and Hardware Slave Management (`SSM = 0`).
* **Data Transmission**: 8-bit and 16-bit Data Frame Formats (`DFF`), blocking polling (`TXE`, `RXNE`, `BSY` flag synchronization) and non-blocking interrupt mode.

### 3. I2C Driver (`stm32f407xx_i2c_driver.c/h`)
* **Bus Standards**: Standard Mode (up to 100 kHz) and Fast Mode (up to 400 kHz with 2:1 & 16:9 duty cycles).
* **Timing & Clock Generation**: Dynamic PCLK1 prescaler lookup, `CCR` clock control calculation, and `TRISE` maximum rise-time programming.
* **Protocol Handling**: START, Repeated START, STOP generation, 7-bit addressing, and dynamic ACKing management.
* **Critical RX Sequences**:
  * **1-Byte Read**: Disables ACK *before* clearing `ADDR`, followed by immediate `STOP` generation to satisfy hardware state machine rules.
  * **Multi-Byte Read**: Automatic second-to-last byte NACK and STOP sequence.
* **Interrupt Mode**: Non-blocking `I2C_MasterTransmitIT` and `I2C_MasterReceiveIT` with event handler (`I2C_EV_IRQHandling`) for `SB`, `ADDR`, `BTF`, `TXE`, `RXNE`, and error handler (`I2C_ER_IRQHandling`) for bus errors.

### 4. USART Driver (`stm32f407xx_usart_driver.c/h`)
* **Baud Rates**: Standard rates up to 3 Mbps (9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 2M, 3M bps).
* **Baud Rate Generator (`BRR`)**: Accurate fixed-point scaled arithmetic computing 12-bit `DIV_Mantissa` and 4-bit `DIV_Fraction` without floating-point overhead.
* **Oversampling**: Oversampling by 16 (`OVER8 = 0`) for maximum noise immunity and Oversampling by 8 (`OVER8 = 1`) for high-speed transmission.
* **Frame Configuration**: 8-bit & 9-bit word lengths (`M` bit); Even, Odd, and None Parity; 0.5, 1, 1.5, and 2 Stop bits.
* **Flow Control**: Hardware RTS/CTS flow control configuration (`CR3`).
* **Transmission & Reception**: Blocking (`USART_SendData`, `USART_ReceiveData`) and non-blocking interrupt APIs (`USART_SendDataIT`, `USART_ReceiveDataIT`).

---

## ⚡ Bus & Clock Topology Reference

```
                             [ 16 MHz HSI / HSE / PLL ]
                                         │
                                   [ AHB Matrix ] (Max 168 MHz)
                                  ┌──────┴──────┐
                       (Max 42 MHz)             (Max 84 MHz)
                       [ APB1 Bus ]             [ APB2 Bus ]
                       ────────────             ────────────
                       • I2C1, I2C2, I2C3       • USART1, USART6
                       • SPI2, SPI3             • SPI1, SPI4
                       • USART2, USART3         • SYSCFG / EXTI
                       • UART4, UART5           • ADC1 - ADC3
```

---

## 🔌 Hardware Setup & Interfacing Guides

### 1. USART2 to PC / Arduino (115200 Baud, 8N1)
* **STM32 Pin `PA2` (USART2_TX, AF7)** $\rightarrow$ **Arduino `Pin 0 (RX)`** *(or Level Shifter `LV1` $\rightarrow$ `HV1` $\rightarrow$ Arduino)*
* **STM32 `GND`** $\rightarrow$ **Arduino `GND`**
* Serial Terminal: **`115200 baud, 8 Data bits, 1 Stop bit, No parity`**.

### 2. I2C1 to Arduino Slave (Address: `0x68`, 100 kHz)
* **STM32 `PB6` (I2C1_SCL, AF4)** $\rightarrow$ **Arduino `A5 (SCL)`** *(with 2.2kΩ–4.7kΩ pull-up to 3.3V)*
* **STM32 `PB9` (I2C1_SDA, AF4)** $\rightarrow$ **Arduino `A4 (SDA)`** *(with 2.2kΩ–4.7kΩ pull-up to 3.3V)*
* **STM32 `GND`** $\rightarrow$ **Arduino `GND`**

### 3. SPI2 to Arduino Slave (Full-Duplex, Mode 0)
* **STM32 `PB15` (MOSI)** $\rightarrow$ **Arduino `Pin 11 (MOSI)`**
* **STM32 `PB14` (MISO)** $\leftarrow$ **Arduino `Pin 12 (MISO)`**
* **STM32 `PB13` (SCK)**  $\rightarrow$ **Arduino `Pin 13 (SCK)`**
* **STM32 `PB12` (NSS)**  $\rightarrow$ **Arduino `Pin 10 (SS)`**
* **STM32 `GND`**         $\rightarrow$ **Arduino `GND`**

---

## 📚 Technical Reference & Interview Guide
For in-depth explanations, timing diagrams, mathematical derivations, hardware edge cases, and 25+ embedded systems interview questions, read the **[`stm32f407_driver_development_master_guide.md`](stm32f407_driver_development_master_guide.md)**.
