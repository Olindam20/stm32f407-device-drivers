# Bare-Metal STM32F407 Device Driver Development Master Guide
### *Comprehensive Architecture, Implementation Notes, Hardware Pitfalls & Embedded Interview Reference*

---

## 📑 Table of Contents
1. [Core MCU & Driver Architecture Overview](#1-core-mcu--driver-architecture-overview)
2. [GPIO Driver (General Purpose Input/Output)](#2-gpio-driver)
3. [SPI Driver (Serial Peripheral Interface)](#3-spi-driver)
4. [I2C Driver (Inter-Integrated Circuit)](#4-i2c-driver)
5. [USART Driver (Universal Synchronous/Asynchronous Receiver Transmitter)](#5-usart-driver)
6. [ARM Cortex-M4 NVIC & Interrupt Subsystem](#6-arm-cortex-m4-nvic--interrupt-subsystem)
7. [Comprehensive Embedded Interview Question Bank](#7-comprehensive-embedded-interview-question-bank)

---

## 1. Core MCU & Driver Architecture Overview

### 1.1 Memory Map & Bus Topology (STM32F407VET6)
* **Core**: ARM Cortex-M4 with FPU @ up to 168 MHz.
* **Bus Architecture**:
  * **AHB1 (Advanced High-performance Bus 1)**: GPIOA - GPIOI, RCC, Flash interface, DMA1, DMA2. Max clock: **168 MHz**.
  * **APB1 (Advanced Peripheral Bus 1 / Low Speed)**: TIM2-TIM7, TIM12-TIM14, SPI2, SPI3, USART2, USART3, UART4, UART5, I2C1, I2C2, I2C3, CAN1, CAN2, DAC, PWR. Max clock: **42 MHz**.
  * **APB2 (Advanced Peripheral Bus 2 / High Speed)**: TIM1, TIM8, USART1, USART6, ADC1-ADC3, SDIO, SPI1, SPI4, SYSCFG, EXTI, TIM9-TIM11. Max clock: **84 MHz**.

```mermaid
graph TD
    Core["ARM Cortex-M4 Core (up to 168 MHz)"] --> AHB["AHB Bus Matrix"]
    AHB --> AHB1["AHB1 (168 MHz)"]
    AHB1 --> GPIO["GPIO Ports A to I"]
    AHB1 --> RCC["RCC Clock Controller"]
    AHB --> APB1_Bridge["APB1 Bridge (Prescaler /1, /2, /4, /8, /16)"]
    AHB --> APB2_Bridge["APB2 Bridge (Prescaler /1, /2, /4, /8, /16)"]
    APB1_Bridge --> APB1["APB1 Bus (Max 42 MHz)"]
    APB1 --> I2C["I2C1, I2C2, I2C3"]
    APB1 --> SPI_APB1["SPI2, SPI3"]
    APB1 --> USART_APB1["USART2, USART3, UART4, UART5"]
    APB2_Bridge --> APB2["APB2 Bus (Max 84 MHz)"]
    APB2 --> SPI_APB2["SPI1, SPI4"]
    APB2 --> USART_APB2["USART1, USART6"]
    APB2 --> SYSCFG["SYSCFG / EXTI"]
```

### 1.2 Driver Design Philosophy
All drivers are structured using clean **Object-Oriented C (CMSIS-style)**:
1. **Peripheral Register Definition Structures (`xxx_RegDef_t`)**: Maps peripheral hardware memory directly via struct offsets and `__vo` (volatile) qualifiers.
2. **Configuration Structures (`xxx_Config_t`)**: Holds user-selected modes, speeds, word lengths, parity, baud rates, and pin options.
3. **Handle Structures (`xxx_Handle_t`)**: Bundles the base register pointer, the config struct, plus non-blocking state variables (`pTxBuffer`, `pRxBuffer`, `TxLen`, `RxLen`, `TxRxState`).

---

## 2. GPIO Driver

### 2.1 Hardware Registers
Each GPIO port has 10 registers (32-bit each):
* `MODER`: Mode (Input `00`, Output `01`, Alternate Function `10`, Analog `11`).
* `OTYPER`: Output Type (Push-Pull `0`, Open-Drain `1`).
* `OSPEEDR`: Output Speed (Low `00`, Medium `01`, Fast `10`, High `11`).
* `PUPDR`: Pull-up/Pull-down (No PUPD `00`, Pull-up `01`, Pull-down `10`).
* `IDR`: Input Data Register (Read-only, bit $x$ represents Pin $x$).
* `ODR`: Output Data Register (Read/Write output state).
* `BSRR`: Bit Set/Reset Register (Atomic pin set `bits [15:0]` and reset `bits [31:16]`).
* `LCKR`: Port Configuration Lock Register (Freezes pin configuration until next reset).
* `AFR[0] / AFR[1]`: Alternate Function Low (`AFRL` for Pins 0-7) and High (`AFRH` for Pins 8-15) (4 bits per pin).

### 2.2 Key Operational Concepts
* **Push-Pull vs Open-Drain**:
  * **Push-Pull**: Uses both P-MOS and N-MOS. Actively drives $V_{DD}$ (HIGH) and $V_{SS}$ (LOW). Used for high-speed signals, SPI, UART TX.
  * **Open-Drain**: P-MOS is disabled. Only N-MOS is active. Drives LOW actively, but floats (Hi-Z) when HIGH. Requires an external pull-up resistor. Used for I2C (wired-AND) and shared buses.
* **Alternate Function Selection**:
  * $AFR\_index = pinNumber / 8$ (0 for AFRL, 1 for AFRH).
  * $AFR\_shift = (pinNumber \% 8) \times 4$.

### 2.3 EXTI & GPIO Interrupt Architecture
1. Microcontroller pin cannot connect directly to NVIC for interrupts; it routes through the **EXTI (Extended Interrupt and Event Controller)**.
2. **SYSCFG Peripheral**: Selects which GPIO port (A, B, C...) maps to a specific EXTI line (EXTI0 to EXTI15).
   * **Rule**: Only ONE Port Pin per line number can be active for interrupts simultaneously (e.g. `PA0`, `PB0`, `PC0` share `EXTI0`).
   * `SYSCFG_EXTICR1` to `SYSCFG_EXTICR4`: 4 bits per line.
3. **Trigger Selection**: Configured in `EXTI_RTSR` (Rising Trigger) and `EXTI_FTSR` (Falling Trigger).
4. **Interrupt Masking**: Unmask in `EXTI_IMR`.
5. **Clearing Pending Bit**: Write `1` to `EXTI_PR` to clear the pending flag.

---

## 3. SPI Driver

### 3.1 Hardware Architecture & Frame Mechanics
* **Protocol Type**: Synchronous, Full-Duplex or Half-Duplex/Simplex, Master-Slave, 4-Wire (MOSI, MISO, SCK, NSS).
* **Clock Polarity (`CPOL`) & Clock Phase (`CPHA`)**:
  * **Mode 0 (`CPOL=0, CPHA=0`)**: SCLK idles LOW. Data sampled on leading (rising) edge, shifted on falling edge.
  * **Mode 1 (`CPOL=0, CPHA=1`)**: SCLK idles LOW. Data sampled on trailing (falling) edge, shifted on rising edge.
  * **Mode 2 (`CPOL=1, CPHA=0`)**: SCLK idles HIGH. Data sampled on leading (falling) edge, shifted on rising edge.
  * **Mode 3 (`CPOL=1, CPHA=1`)**: SCLK idles HIGH. Data sampled on trailing (rising) edge, shifted on falling edge.

```
Mode 0: SCK Idle LOW  ─┐   ┌─┐   ┌─┐   ┌─┐
                       └───┘ └───┘ └───┘ └───
        Sampling:        ▲     ▲     ▲
```

### 3.2 Slave Select Management (`SSM` & `SSI`)
* **Hardware Slave Management (`SSM = 0`)**: Hardware manages NSS pin. If multi-master mode is disabled, pulling NSS LOW can trigger a Master Mode Fault (`MODF`).
* **Software Slave Management (`SSM = 1`)**: The external NSS pin is freed for normal GPIO use. The internal NSS signal is driven by the value of the **`SSI` (Internal Slave Select)** bit in `SPI_CR1`.
  * For Master Mode with `SSM=1`: **`SSI` MUST be set to `1`** to avoid MODF errors and forced reversion to slave mode.

### 3.3 Data Transmission & Flag Synchronization
* **`TXE` (Transmit Buffer Empty)**: Indicates `SPI_DR` is empty and ready for the next data byte/word.
* **`RXNE` (Receive Buffer Not Empty)**: Indicates incoming data in `SPI_DR` is ready to be read.
* **`BSY` (Busy Flag)**: Set when SPI is actively shifting data.
  * **Critical Closing Sequence**:
    1. Wait until `TXE = 1`.
    2. Wait until `BSY = 0` (guarantees the last bit physically left the shift register).
    3. Clear overrun/dummy data if necessary.
    4. Disable SPI (`SPE = 0`).

---

## 4. I2C Driver

### 4.1 Bus Architecture & Signal Dynamics
* **Protocol Type**: Synchronous, Half-Duplex, 2-Wire (SCL, SDA), Multi-Master/Multi-Slave.
* **Physical Layer**: Open-Drain on both SCL and SDA with external pull-up resistors ($R_P$).
* **Speed Standards**:
  * Standard Mode (Sm): Up to **100 kHz** ($T_{\text{high}} = T_{\text{low}} = 5\mu\text{s}$).
  * Fast Mode (Fm): Up to **400 kHz** (Duty cycle 2:1 or 16:9).

### 4.2 Pull-Up Resistor Calculation & Physical Bus Dynamics
$$R_{p(\text{min})} = \frac{V_{DD} - V_{OL(\text{max})}}{I_{OL}} = \frac{3.3\text{V} - 0.4\text{V}}{3\text{ mA}} \approx 966\,\Omega$$
$$R_{p(\text{max})} = \frac{t_r}{0.8473 \times C_b}$$
*(Where $t_r = 1000\text{ ns}$ for Sm, $300\text{ ns}$ for Fm; $C_b$ is total bus capacitance).*
* Typical practical value for breadboards / short buses: **$2.2\text{ k}\Omega$ to $4.7\text{ k}\Omega$**.
* **Why Internal Pull-ups (~40kΩ) Fail for RX**:
  * Internal pull-ups are too weak ($40\text{ k}\Omega$), producing very slow rise times ($RC > 1\mu\text{s}$).
  * TX works because STM32 drives LOW actively; RX fails because floating slave release and STOP conditions cannot snap to $V_{DD}$ fast enough, leading to false bus-busy detection (`BUSY=1` in `SR2`) and timeout lockups.

### 4.3 Clock Control Register (`CCR`) and `TRISE` Calculations
1. **Peripheral Frequency (`FREQ` in `CR2`)**:
   $$\text{FREQ} = \frac{f_{\text{PCLK1}}}{1\text{ MHz}}$$ (Must be at least 2 MHz for Sm, 14 MHz for Fm).
2. **Standard Mode CCR Calculation**:
   $$T_{\text{SCL}} = 2 \times \text{CCR} \times T_{\text{PCLK1}} \implies \mathbf{\text{CCR} = \frac{f_{\text{PCLK1}}}{2 \times f_{\text{SCL}}}}$$
3. **TRISE Register Calculation**:
   $$\text{TRISE} = \left(\frac{t_{r(\text{max})}}{T_{\text{PCLK1}}}\right) + 1 = \left(t_{r(\text{max})} \times f_{\text{PCLK1}}\right) + 1$$
   * For Standard Mode ($t_{r(\text{max})} = 1000\text{ ns} = 1\mu\text{s}$): $\text{TRISE} = \left(\frac{f_{\text{PCLK1}}}{1\text{ MHz}}\right) + 1 = \text{FREQ} + 1$.

### 4.4 I2C Read Sequence (The 1-Byte vs Multi-Byte Rule)

```mermaid
sequenceDiagram
    autonumber
    participant M as STM32 Master
    participant S as I2C Slave
    Note over M,S: Single Byte Reception Sequence (Len = 1)
    M->>S: Generate START (CR1: START=1)
    M->>S: Send Slave Address + READ (0x68 | 0x01)
    S-->>M: ACK
    Note over M: ADDR flag set in SR1
    critical Hardware Requirement for 1-Byte Read
        M->>M: 1. Disable ACKing (CR1: ACK=0)
        M->>M: 2. Clear ADDR (Read SR1 followed by SR2)
        M->>M: 3. Generate STOP (CR1: STOP=1)
    end
    M->>S: Master NACKs incoming byte
    M->>M: Wait for RXNE=1, Read DR into buffer
```

* **Multi-Byte Read Sequence (`Len > 1`)**:
  1. Clear ADDR flag (Read `SR1` then `SR2`).
  2. For each byte until `Len - 2`: wait for `RXNE=1`, read `DR`.
  3. When `remaining == 2`:
     * Disable ACK (`ACK = 0`).
     * Generate STOP condition (`STOP = 1`).
     * Wait for `RXNE=1`, read byte $N-1$.
     * Wait for `RXNE=1`, read last byte $N$.

---

## 5. USART Driver

### 5.1 Asynchronous Serial Frame Format
* **Idle Line**: High ($V_{DD}$).
* **Start Bit**: 1 bit LOW ($0$).
* **Data Bits**: 8 or 9 bits (LSB first).
* **Parity Bit (Optional)**: Even or Odd (occupies the MSB bit of the data word).
* **Stop Bits**: 0.5, 1, 1.5, or 2 bits HIGH ($1$).

```
Idle (1) ──┐      ┌───┬───┬───┬───┬───┬───┬───┬───┐     ┌───┐ Idle (1)
           └──────┴───┴───┴───┴───┴───┴───┴───┴───┴─────┘   └───
           START   D0  D1  D2  D3  D4  D5  D6  D7  PARITY STOP
```

### 5.2 Oversampling Mechanics (`OVER8`)
* **Oversampling by 16 (`OVER8 = 0`)**: Bit duration split into 16 clock ticks. Middle samples at **8, 9, 10** with 2/3 majority voting. Maximum noise tolerance.
* **Oversampling by 8 (`OVER8 = 1`)**: Bit duration split into 8 clock ticks. Middle samples at **4, 5, 6**. Doubles maximum achievable baud rate ($\frac{f_{\text{PCLK}}}{8}$).

### 5.3 Fixed-Point Baud Rate Calculation (`BRR`)
The exact formula:
$$\text{Baud Rate} = \frac{f_{\text{PCLK}}}{8 \times (2 - \text{OVER8}) \times \text{USARTDIV}}$$

To calculate accurately in pure integer C without floating-point math:
```c
// Scaled by 100 to preserve 2 decimal places:
if(OVER8 == 1) {
    usartdiv = (25 * PCLK) / (2 * BaudRate);
} else {
    usartdiv = (25 * PCLK) / (4 * BaudRate);
}

M_part = usartdiv / 100;
F_part = usartdiv - (M_part * 100);

if(OVER8 == 1) {
    F_part = (((F_part * 8) + 50) / 100) & 0x07; // +50 implements round-to-nearest
} else {
    F_part = (((F_part * 16) + 50) / 100) & 0x0F;
}

pUSARTx->BRR = (M_part << 4) | F_part;
```

---

## 6. ARM Cortex-M4 NVIC & Interrupt Subsystem

### 6.1 NVIC Registers
* **`NVIC_ISER0` - `NVIC_ISER7`**: Interrupt Set-Enable Registers. Writing `1` enables the IRQ. Writing `0` has NO effect.
* **`NVIC_ICER0` - `NVIC_ICER7`**: Interrupt Clear-Enable Registers. Writing `1` disables the IRQ. Writing `0` has NO effect.
* **`NVIC_IPR0` - `NVIC_IPR59`**: Interrupt Priority Registers (8 bits per IRQ).
  * **STM32 Implementation Detail**: Only the top **4 bits** (`[7:4]`) of each 8-bit priority field are implemented (`NO_PR_BITS_IMPLEMENTED = 4`). Priority values range from `0` (highest) to `15` (lowest).
  * Shift formula: `shift_amount = (8 * (IRQNumber % 4)) + (8 - NO_PR_BITS_IMPLEMENTED)`.

---

## 7. Comprehensive Embedded Interview Question Bank

### 🔹 General & Architecture
1. **Q: Why are peripheral registers defined with `volatile` (`__vo`)?**
   * **A**: Hardware registers change asynchronously outside the compiler's control (e.g. status flags set by incoming hardware signals). Without `volatile`, compiler optimization (O2/O3) will cache register reads into CPU general-purpose registers (R0-R12) and generate dead loops (e.g. `while(!FLAG)`), never re-reading the memory-mapped register address.

2. **Q: What is the difference between atomic bit access via `BSRR` versus read-modify-write on `ODR`?**
   * **A**: Read-modify-write (`ODR |= (1 << pin)`) compiles into three assembly instructions (`LDR`, `ORR`, `STR`). If an interrupt fires between `LDR` and `STR` and modifies another pin on the same port, the subsequent `STR` will overwrite and corrupt the interrupt's change. `BSRR` is a single-cycle atomic hardware operation that sets/resets specific pins without affecting other pins on the port.

### 🔹 SPI Peripheral
3. **Q: Why must you wait for `BSY == 0` instead of just `TXE == 1` before disabling SPI?**
   * **A**: `TXE = 1` only means the software data register (`SPI_DR`) is empty because the byte has moved into the internal Shift Register. Shifting the 8/16 bits out onto the wire takes multiple clock cycles. Disabling SPI immediately upon `TXE=1` abruptly cuts off the clock and truncates the transmission mid-byte. `BSY = 0` guarantees the shift register has physically completed transmission.

4. **Q: What is MODF (Master Mode Fault) error in SPI and how do you prevent it?**
   * **A**: In hardware NSS mode, if the MCU is configured as Master and an external signal pulls the `NSS` pin LOW, the SPI hardware detects a bus collision (another master asserting control), clears `SPE` (disables SPI), sets `MODF`, and forces the MCU into Slave mode. To prevent this in single-master systems, enable Software Slave Management (`SSM = 1`) and force internal NSS high by setting `SSI = 1`.

### 🔹 I2C Peripheral
5. **Q: Why does I2C single-byte read require clearing ACK *before* clearing the ADDR flag?**
   * **A**: When `ADDR` is cleared, the I2C shift register immediately begins clocking in data bits from the slave. If `ACK=1` when data reception starts, the hardware will automatically generate an ACK pulse on the 9th SCL clock cycle. By disabling ACK (`ACK=0`) before clearing `ADDR`, hardware is primed to output a NACK on the 9th clock, signaling to the slave that this is the final byte.

6. **Q: What causes I2C bus lockup (SDA held LOW by slave), and how do you recover?**
   * **A**: If the Master resets or encounters an error in the middle of an active byte transfer while the Slave is driving SDA LOW (e.g., transmitting a `0` data bit or ACK), the Slave waits indefinitely for clock pulses to finish its byte. 
   * **Recovery Procedure**: Configure SCL pin as GPIO output, manually toggle SCL for 9 clock cycles to clock out the stuck byte, send a manual STOP condition, and then re-initialize the I2C peripheral.

### 🔹 USART Peripheral
7. **Q: What is the difference between `TXE` and `TC` status flags in USART?**
   * **A**: 
     * `TXE` (Transmit Data Register Empty): Set when data moves from `DR` into the Transmit Shift Register. Signals that software can safely write the next byte into `DR`.
     * `TC` (Transmission Complete): Set only when the entire frame (including the last stop bit) has physically left the Transmit Shift Register and the transmitter is completely idle. Used before putting the MCU to sleep, turning off the transceiver, or switching RS-485 direction lines.

8. **Q: Why does 9-bit word length require 2 bytes of buffer advance only when Parity is disabled?**
   * **A**: In STM32 USART, the `M` bit sets the total frame size. If 9-bit is selected and parity is enabled, the 9th bit is generated automatically by the hardware parity engine from the 8 data bits provided in a 1-byte user buffer. If parity is disabled, all 9 bits are data provided by the user, which cannot fit in an 8-bit `uint8_t` and must be stored as a 16-bit (`uint16_t`) integer (advancing 2 bytes per transfer).

