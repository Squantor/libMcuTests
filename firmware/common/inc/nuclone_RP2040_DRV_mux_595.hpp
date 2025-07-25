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
using LedPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin25, libmcuhw::IoFunctions::Sio>;
// multiplexer pins
using MuxNotEnablePinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin00, libmcuhw::IoFunctions::Sio>;
using MuxA0PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin01, libmcuhw::IoFunctions::Sio>;
using MuxA1PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin02, libmcuhw::IoFunctions::Sio>;
using MuxA2PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin03, libmcuhw::IoFunctions::Sio>;
using MuxY0PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin04, libmcuhw::IoFunctions::Sio>;
using MuxY1PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin05, libmcuhw::IoFunctions::Sio>;
using MuxY2PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin06, libmcuhw::IoFunctions::Sio>;
using MuxY7PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin07, libmcuhw::IoFunctions::Sio>;

constexpr LedPinType led_pin;
constexpr MuxNotEnablePinType mux_not_enable_pin;
constexpr MuxA0PinType mux_A0_pin;
constexpr MuxA1PinType mux_A1_pin;
constexpr MuxA2PinType mux_A2_pin;
constexpr MuxY0PinType mux_Y0_pin;
constexpr MuxY1PinType mux_Y1_pin;
constexpr MuxY2PinType mux_Y2_pin;
constexpr MuxY7PinType mux_Y7_pin;

extern libmcull::systick::Systick<libmcuhw::SystickAddress> systick_peripheral;
extern libmcull::nvic::Nvic<libmcuhw::NvicAddress, libmcuhw::ScbAddress> nvic_peripheral;
extern libmcull::resets::Resets<libmcuhw::ResetsAddress> resets_peripheral;
extern libmcull::pads::PadsBank0<libmcuhw::PadsBank0Address> pads_bank0_peripheral;
extern libmcull::gpioBank0::GpioBank0<libmcuhw::IoBank0Address> gpio_bank0_peripheral;
extern libmcull::sio_gpio::SioGpio<libmcuhw::SioAddress> sio_gpio_peripheral;

extern libmcuhal::gpio::Gpio<pads_bank0_peripheral, sio_gpio_peripheral> gpio_bank0_hal;
extern libMcuDriver::mux::mux3to8<gpio_bank0_hal, MuxNotEnablePinType, MuxA0PinType, MuxA1PinType, MuxA2PinType> test_mux;

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