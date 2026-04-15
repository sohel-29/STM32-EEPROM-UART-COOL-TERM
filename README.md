# STM32-EEPROM-UART-COOL-TERM
STM32 HAL project to receive text over UART from Tera Term, store it in AT24C256 EEPROM over I2C, read it back, and print the result on UART.



# STM32 EEPROM UART Tera Term

STM32 HAL project to receive text from **Tera Term** over **USART2**, write it to an **AT24C256 I2C EEPROM**, read it back, and display both the received and stored data on the serial terminal. [1][2][3]

## Features

- UART receive from Tera Term using STM32 HAL polling mode. [2]
- EEPROM read/write using `HAL_I2C_Mem_Write()` and `HAL_I2C_Mem_Read()`. [1]
- Page-safe EEPROM buffer writing for AT24C256-style devices. [1]
- Serial output verification on Tera Term at 115200 baud. [2]
- Simple CubeMX / CubeIDE friendly structure. [2]

## Hardware Used

- STM32 board with **I2C1** and **USART2**
- AT24C256 EEPROM
- Tera Term on PC
- Pull-up resistors for I2C if not already present on module

## Pin Example

| STM32 Pin | Function |
|---|---|
| PB8 | I2C1 SCL |
| PB9 | I2C1 SDA |
| PA2 | USART2 TX |
| PA3 | USART2 RX |

This PB8/PB9 and PA2/PA3 mapping is a common STM32F446RE example arrangement for I2C1 and USART2. [web:173]

## How It Works

1. STM32 prints a prompt on Tera Term. [web:159]
2. User types a word or sentence in Tera Term and presses Enter.
3. STM32 receives the text using UART blocking receive. [web:168]
4. The text is written into EEPROM over I2C using HAL memory write functions. [web:1]
5. STM32 reads the same data back from EEPROM using HAL memory read functions. [web:1]
6. Both UART input and EEPROM output are printed on Tera Term for verification. [web:159]

## Example Output

```text
SYSTEM START
EEPROM READY

Enter text: Sohel
WRITE OK
READ OK
UART DATA   : Sohel
EEPROM DATA : Sohel
--------------------------
```

## UART Settings

Use these Tera Term serial settings:

- Baud rate: **115200**
- Data bits: **8**
- Parity: **None**
- Stop bits: **1**
- Flow control: **None**

These settings match common STM32 USART2 terminal examples for Tera Term. [web:159]

## EEPROM Notes

This project is designed for **AT24C256**-style EEPROM devices using a 16-bit memory address and page write handling. AT24C256 is a 256 Kbit serial EEPROM commonly interfaced to STM32 over I2C using HAL APIs. [web:1][web:174]

The EEPROM device address is typically used in STM32 HAL as a left-shifted 7-bit address, for example `0x50 << 1`. HAL handles the read/write direction bit internally in its I2C APIs. [web:24][web:174]

## Software

- STM32CubeIDE / STM32CubeMX
- STM32 HAL drivers
- Tera Term

## Project Structure

```text
Core/
├── Inc/
│   └── main.h
├── Src/
│   ├── main.c
│   └── stm32f4xx_hal_msp.c
```

## Future Improvements

- Interrupt-based UART receive for non-blocking input. [web:168]
- EEPROM data logging with timestamps or packet counters.
- String length header storage for variable-length data.
- Circular buffer logging inside EEPROM.
- DMA-based UART communication for larger data transfers.

