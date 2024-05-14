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
#include <drivers/mux3to8.hpp>
#include <drivers/spi74595.hpp>

#define TICKS_PER_S (8u)

// LED connected pin 25 via a resistor to ground
using ledPinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN25, libMcuLL::hw::IOfuncts::SIO>;
// multiplexer pins
using muxNotEnablePinType =
  libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN00, libMcuLL::hw::IOfuncts::SIO>;
using muxA0PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN01, libMcuLL::hw::IOfuncts::SIO>;
using muxA1PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN02, libMcuLL::hw::IOfuncts::SIO>;
using muxA2PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN03, libMcuLL::hw::IOfuncts::SIO>;
using muxY0PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN04, libMcuLL::hw::IOfuncts::SIO>;
using muxY1PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN05, libMcuLL::hw::IOfuncts::SIO>;
using muxY2PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN06, libMcuLL::hw::IOfuncts::SIO>;
using muxY7PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN07, libMcuLL::hw::IOfuncts::SIO>;

constexpr ledPinType ledPin;
constexpr muxNotEnablePinType muxNotEnablePin;
constexpr muxA0PinType muxA0Pin;
constexpr muxA1PinType muxA1Pin;
constexpr muxA2PinType muxA2Pin;
constexpr muxY0PinType muxY0Pin;
constexpr muxY1PinType muxY1Pin;
constexpr muxY2PinType muxY2Pin;
constexpr muxY7PinType muxY7Pin;

extern libMcuLL::sw::systick::systick<libMcuLL::hw::systickAddress> systickPeripheral;
extern libMcuLL::sw::nvic::nvic<libMcuLL::hw::nvicAddress, libMcuLL::hw::scbAddress> nvicPeripheral;
extern libMcuLL::sw::resets::resets<libMcuLL::hw::resetsAddress> resetsPeripheral;

extern libMcuHal::pinsHalType pinsHal;
extern libMcuHal::gpioHalType gpioHal;
extern libMcuDriver::mux::mux3to8<gpioHal, muxNotEnablePinType, muxA0PinType, muxA1PinType, muxA2PinType> testMux;

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