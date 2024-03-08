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

void boardInit(void);

#define LED_PIN BANK0_GPIO25
#define LED_MASK BANK0_GPIO25_MASK
#define CLOCK_PIN BANK0_GPIO21
#define CLOCK_MASK BANK0_GPIO21_MASK

// connected pin 0 and 1 via 3.3K resistors, links UART RX/TX
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO
// connected pin 2 and 5 via 3.3K resistors, links SPI CS/SCK
// connected pin 16 to 2.2K pull up resistor, SDA
// connected pin 17 to 2.2K pull up resistor, SCL

#endif