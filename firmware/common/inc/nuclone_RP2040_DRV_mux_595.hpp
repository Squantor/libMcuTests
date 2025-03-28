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

#include <raspberrypi/libmcu_RP2040_hal.hpp>
#include <drivers/mux3to8.hpp>
#include <drivers/spi74595.hpp>

#define TICKS_PER_S (8u)

// LED connected pin 25 via a resistor to ground
using ledPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN25, libmcuhw::IOfuncts::SIO>;
// multiplexer pins
using muxNotEnablePinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN00, libmcuhw::IOfuncts::SIO>;
using muxA0PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN01, libmcuhw::IOfuncts::SIO>;
using muxA1PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN02, libmcuhw::IOfuncts::SIO>;
using muxA2PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN03, libmcuhw::IOfuncts::SIO>;
using muxY0PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN04, libmcuhw::IOfuncts::SIO>;
using muxY1PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN05, libmcuhw::IOfuncts::SIO>;
using muxY2PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN06, libmcuhw::IOfuncts::SIO>;
using muxY7PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN07, libmcuhw::IOfuncts::SIO>;

constexpr ledPinType ledPin;
constexpr muxNotEnablePinType muxNotEnablePin;
constexpr muxA0PinType muxA0Pin;
constexpr muxA1PinType muxA1Pin;
constexpr muxA2PinType muxA2Pin;
constexpr muxY0PinType muxY0Pin;
constexpr muxY1PinType muxY1Pin;
constexpr muxY2PinType muxY2Pin;
constexpr muxY7PinType muxY7Pin;

extern libmcull::systick::systick<libmcuhw::systickAddress> systickPeripheral;
extern libmcull::nvic::nvic<libmcuhw::nvicAddress, libmcuhw::scbAddress> nvicPeripheral;
extern libmcull::resets::resets<libmcuhw::resetsAddress> resetsPeripheral;

extern libmcuhal::pins::pins<libmcuhw::padsBank0Address, libmcuhw::ioBank0Address> pinsHal;
extern libmcuhal::gpio::gpio<libmcuhw::padsBank0Address, libmcuhw::ioBank0Address, libmcuhw::sioAddress> gpioHal;
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