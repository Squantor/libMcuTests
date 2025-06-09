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
using ledPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin25, libmcuhw::IoFunctions::kSio>;
// multiplexer pins
using muxNotEnablePinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin00, libmcuhw::IoFunctions::kSio>;
using muxA0PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin01, libmcuhw::IoFunctions::kSio>;
using muxA1PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin02, libmcuhw::IoFunctions::kSio>;
using muxA2PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin03, libmcuhw::IoFunctions::kSio>;
using muxY0PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin04, libmcuhw::IoFunctions::kSio>;
using muxY1PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin05, libmcuhw::IoFunctions::kSio>;
using muxY2PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin06, libmcuhw::IoFunctions::kSio>;
using muxY7PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin07, libmcuhw::IoFunctions::kSio>;

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
extern libmcull::resets::Resets<libmcuhw::kResetsAddress> resetsPeripheral;

extern libmcuhal::pins::Pins<libmcuhw::kPadsBank0Address, libmcuhw::kIoBank0Address> pinsHal;
extern libmcuhal::gpio::Gpio<libmcuhw::kPadsBank0Address, libmcuhw::kIoBank0Address, libmcuhw::kSioAddress> gpioHal;
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