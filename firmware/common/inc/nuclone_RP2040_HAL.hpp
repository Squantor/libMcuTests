/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef NUCLONE_RP2040_HAL_HPP
#define NUCLONE_RP2040_HAL_HPP

#define FREQ_XTAL (12000000u)      /**< Crystal oscillator frequncy */
#define FREQ_CPU (125000000u)      /**< CPU frequency */
#define FREQ_USB (48000000u)       /**< USB frequency */
#define FREQ_ADC (48000000u)       /**< ADC frequency */
#define FREQ_RTC (FREQ_XTAL / 256) /**< RTC frequency */
#define FREQ_PERI (FREQ_CPU)       /**< Peripherals frequency */

#include <raspberrypi/libmcu_RP2040_hal.hpp>

#define TICKS_PER_S (8u)

// Summarized HSI
// connected pin 0 and 1 via 3.3K resistors, links UART RX/TX, PIN1 on board
using Gpio0PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin00, libmcuhw::IoFunctions::Sio>;
using Gpio1PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin01, libmcuhw::IoFunctions::Sio>;
using UartOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin00, libmcuhw::IoFunctions::Uart0Tx>;
using UartInPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin01, libmcuhw::IoFunctions::UArt0Rx>;
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO, PIN3 on board
using Gpio2PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin03, libmcuhw::IoFunctions::Sio>;
using Gpio3PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin04, libmcuhw::IoFunctions::Sio>;
using SpiOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin03, libmcuhw::IoFunctions::Spi0Tx>;
using SpiInPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin04, libmcuhw::IoFunctions::Spi0Rx>;
// connected pin 2 and 5 via 3.3K resistors, links SPI CS/SCK, PIN2 on board
using Gpio4PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin02, libmcuhw::IoFunctions::Sio>;
using Gpio5PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin05, libmcuhw::IoFunctions::Sio>;
using SpiSckPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin02, libmcuhw::IoFunctions::Spi0Sck>;
using SpiCsPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin05, libmcuhw::IoFunctions::Spi0Cs>;
// connected pin 16 to 2.2K pull up resistor, SDA
using I2cSdaPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin16, libmcuhw::IoFunctions::I2c0Sda>;
// connected pin 17 to 2.2K pull up resistor, SCL
using I2cSclPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin17, libmcuhw::IoFunctions::I2c0Scl>;

// LED connected pin 25 via a resistor to ground
using LedPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin25, libmcuhw::IoFunctions::Sio>;
// clock sense connected to pin 21
using ClockOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin21, libmcuhw::IoFunctions::ClkGpout0>;

constexpr LedPinType led_pin;
constexpr ClockOutPinType clock_out_pin;
constexpr Gpio0PinType gpio0_pin;
constexpr Gpio1PinType gpio1_pin;
constexpr Gpio2PinType gpio2_pin;
constexpr Gpio3PinType gpio3_pin;
constexpr Gpio4PinType gpio4_pin;
constexpr Gpio5PinType gpio5_pin;
constexpr SpiSckPinType spi_sck_pin;

extern libmcull::systick::Systick<libmcuhw::SystickAddress> systick_peripheral;
extern libmcull::nvic::Nvic<libmcuhw::NvicAddress, libmcuhw::ScbAddress> nvic_peripheral;
extern libmcull::resets::Resets<libmcuhw::ResetsAddress> resets_peripheral;
extern libmcull::pads::PadsBank0<libmcuhw::PadsBank0Address> pads_bank0_peripheral;
extern libmcull::gpioBank0::GpioBank0<libmcuhw::IoBank0Address> gpio_bank0_peripheral;
extern libmcull::sio_gpio::SioGpio<libmcuhw::SioAddress> sio_gpio_peripheral;

extern libmcuhal::gpio::Gpio<pads_bank0_peripheral, sio_gpio_peripheral> gpio_bank0_hal;

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