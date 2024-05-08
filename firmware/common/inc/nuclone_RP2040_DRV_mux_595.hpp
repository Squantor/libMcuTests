/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef NUCLONE_RP2040_DRV_595_HPP
#define NUCLONE_RP2040_DRV_595_HPP

#define FREQ_XTAL (12000000u)      /**< Crystal oscillator frequncy */
#define FREQ_CPU (125000000u)      /**< CPU frequency */
#define FREQ_USB (48000000u)       /**< USB frequency */
#define FREQ_ADC (48000000u)       /**< ADC frequency */
#define FREQ_RTC (FREQ_XTAL / 256) /**< RTC frequency */
#define FREQ_PERI (FREQ_CPU)       /**< Peripherals frequency */

#include <libmcuRP2040hal.hpp>

#define TICKS_PER_S (8u)

// LED connected pin 25 via a resistor to ground
using ledPinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN25, libMcuLL::hw::IOfuncts::SIO>;

constexpr ledPinType ledPin;

extern libMcuLL::sw::systick::systick<libMcuLL::hw::systickAddress> systickPeripheral;
extern libMcuLL::sw::nvic::nvic<libMcuLL::hw::nvicAddress, libMcuLL::hw::scbAddress> nvicPeripheral;
extern libMcuLL::sw::resets::resets<libMcuLL::hw::resetsAddress> resetsPeripheral;

extern libMcuHal::pinsHalType pinsHal;
extern libMcuHal::gpioHalType gpioHal;

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