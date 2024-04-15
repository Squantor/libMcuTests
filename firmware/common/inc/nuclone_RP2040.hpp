/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef NUCLONE_RP2040_HPP
#define NUCLONE_RP2040_HPP

#define FREQ_XTAL (12000000u)      /**< Crystal oscillator frequncy */
#define FREQ_CPU (125000000u)      /**< CPU frequency */
#define FREQ_USB (48000000u)       /**< USB frequency */
#define FREQ_ADC (48000000u)       /**< ADC frequency */
#define FREQ_RTC (FREQ_XTAL / 256) /**< RTC frequency */
#define FREQ_PERI (FREQ_CPU)       /**< Peripherals frequency */

#include <mcu_ll.hpp>

#define TICKS_PER_S (8u)

// Summarized HSI
// connected pin 0 and 1 via 3.3K resistors, links UART RX/TX
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO
// connected pin 2 and 5 via 3.3K resistors, links SPI CS/SCK
// connected pin 16 to 2.2K pull up resistor, SDA
// connected pin 17 to 2.2K pull up resistor, SCL
// LED connected pin 25 via a resistor to ground
// clock sense connected to pin 21

// test blink pin
using ledPinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN25, libMcuLL::hw::IOfuncts::SIO>;
// clock output test pin
using clockOutPinType =
  libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN21, libMcuLL::hw::IOfuncts::CLK_GPOUT0>;

constexpr ledPinType ledPin;
constexpr clockOutPinType clockOutPin;

/**
 * @brief Initialize the development board
 *
 */
void boardInit(void);
/**
 * @brief Simple delay function that burns time with 10 NOP instructions
 *
 * @param iterations amount of iterations to execute
 */
void crudeDelay(uint32_t iterations);

#endif