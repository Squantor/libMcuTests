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
using LedPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin25, libmcuhw::IoFunctions::kSio>;
// multiplexer pins
using MuxNotEnablePinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin00, libmcuhw::IoFunctions::kSio>;
using MuxA0PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin01, libmcuhw::IoFunctions::kSio>;
using MuxA1PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin02, libmcuhw::IoFunctions::kSio>;
using MuxA2PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin03, libmcuhw::IoFunctions::kSio>;
using MuxY0PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin04, libmcuhw::IoFunctions::kSio>;
using MuxY1PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin05, libmcuhw::IoFunctions::kSio>;
using MuxY2PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin06, libmcuhw::IoFunctions::kSio>;
using MuxY7PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin07, libmcuhw::IoFunctions::kSio>;

constexpr LedPinType ledPin;
constexpr MuxNotEnablePinType muxNotEnablePin;
constexpr MuxA0PinType muxA0Pin;
constexpr MuxA1PinType muxA1Pin;
constexpr MuxA2PinType muxA2Pin;
constexpr MuxY0PinType muxY0Pin;
constexpr MuxY1PinType muxY1Pin;
constexpr MuxY2PinType muxY2Pin;
constexpr MuxY7PinType muxY7Pin;

extern libmcull::systick::Systick<libmcuhw::kSystickAddress> systick_peripheral;
extern libmcull::nvic::Nvic<libmcuhw::kNvicAddress, libmcuhw::kScbAddress> nvic_peripheral;
extern libmcull::resets::Resets<libmcuhw::kResetsAddress> resets_peripheral;

extern libmcuhal::pins::Pins<libmcuhw::kPadsBank0Address, libmcuhw::kIoBank0Address> pins_hal;
extern libmcuhal::gpio::Gpio<libmcuhw::kPadsBank0Address, libmcuhw::kIoBank0Address, libmcuhw::kSioAddress> gpio_hal;
extern libMcuDriver::mux::mux3to8<gpio_hal, MuxNotEnablePinType, MuxA0PinType, MuxA1PinType, MuxA2PinType> test_mux;

/**
 * @brief Initialize the development board
 */
void board_init(void);
/**
 * @brief Simple delay function that burns time with 10 NOP instructions
 * @param iterations amount of iterations to execute
 */
void crudeDelay(uint32_t iterations);

#endif