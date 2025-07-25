/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef NUCLONE_RP2040_LL_HPP
#define NUCLONE_RP2040_LL_HPP

#define FREQ_XTAL (12'000'000u)    /**< Crystal oscillator frequncy */
#define FREQ_CPU (120'000'000u)    /**< CPU frequency */
#define FREQ_USB (48'000'000u)     /**< USB frequency */
#define FREQ_ADC (48'000'000u)     /**< ADC frequency */
#define FREQ_RTC (FREQ_XTAL / 256) /**< RTC frequency */
#define FREQ_PERI (FREQ_CPU)       /**< Peripherals frequency */

#include <raspberrypi/libmcu_RP2040_ll.hpp>

#define TICKS_PER_S (8u)

// Summarized HSI
// connected pin 0 and 1 via 3.3K resistors, links UART RX/TX, PIN1 on board
using Gpio0PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin00, libmcuhw::IoFunctions::Sio>;
using Gpio1PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin01, libmcuhw::IoFunctions::Sio>;
using UartOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin00, libmcuhw::IoFunctions::Uart0Tx>;
using UartInPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin01, libmcuhw::IoFunctions::UArt0Rx>;
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO, PIN3 on board
using Gpio2PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin03, libmcuhw::IoFunctions::Sio>;
using gpio3PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin04, libmcuhw::IoFunctions::Sio>;
using SpiOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin03, libmcuhw::IoFunctions::Spi0Tx>;
using SpiInPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin04, libmcuhw::IoFunctions::Spi0Rx>;
// connected pin 2 and 5 via 3.3K resistors, links SPI CS/SCK, PIN2 on board
using Gpio4PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin02, libmcuhw::IoFunctions::Sio>;
using Gpio5PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin05, libmcuhw::IoFunctions::Sio>;
using SpiScPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin02, libmcuhw::IoFunctions::Spi0Sck>;
using SpiCsPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin05, libmcuhw::IoFunctions::Spi0Cs>;
// connected pin 16 to 2.2K pull up resistor, SDA
using I2cSdaPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin16, libmcuhw::IoFunctions::I2c0Sda>;
// connected pin 17 to 2.2K pull up resistor, SCL
using I2cSclPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin17, libmcuhw::IoFunctions::I2c0Scl>;

// LED connected pin 25 via a resistor to ground
using LedPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin25, libmcuhw::IoFunctions::Sio>;
// clock sense connected to pin 21
using ClockOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin21, libmcuhw::IoFunctions::ClkGpout0>;

constexpr LedPinType ledPin;
constexpr ClockOutPinType clockOutPin;
constexpr Gpio0PinType gpio0Pin;
constexpr Gpio1PinType gpio1Pin;
constexpr Gpio2PinType gpio2Pin;
constexpr gpio3PinType gpio3Pin;
constexpr Gpio4PinType gpio4Pin;
constexpr Gpio5PinType gpio5Pin;
constexpr SpiOutPinType spiOutPin;
constexpr SpiInPinType spiInPin;
constexpr SpiScPinType spiSckPin;
constexpr SpiCsPinType spiCsPin;
constexpr I2cSclPinType i2c_scl_pin;
constexpr I2cSdaPinType i2cSdaPin;
constexpr UartInPinType uartRxPin;
constexpr UartOutPinType uartTxPin;

extern libmcull::systick::Systick<libmcuhw::SystickAddress> systick_peripheral;
extern libmcull::nvic::Nvic<libmcuhw::NvicAddress, libmcuhw::ScbAddress> nvic_peripheral;
extern libmcull::pads::PadsBank0<libmcuhw::PadsBank0Address> pads_bank0_peripheral;
extern libmcull::gpioBank0::GpioBank0<libmcuhw::IoBank0Address> gpio_bank0_peripheral;
extern libmcull::resets::Resets<libmcuhw::ResetsAddress> resets_peripheral;
extern libmcull::sio_gpio::SioGpio<libmcuhw::SioAddress> sio_gpio_peripheral;
extern libmcull::clocks::Clocks<libmcuhw::ClocksAddress> clocks_peripheral;
extern libmcull::xosc::Xosc<libmcuhw::XoscAddress> xosc_peripheral;
extern libmcull::pll::Pll<libmcuhw::PllSysAddress> pll_sys_peripheral;
extern libmcull::pll::Pll<libmcuhw::PllUsbAddress> pll_usb_peripheral;
extern libmcull::spi::SpiPolled<libmcuhw::Spi0Address> spi_polled_peripheral;
extern libmcull::i2c::I2cPolled<libmcuhw::I2c0Address> i2c_polled_peripheral;
extern libmcull::uart::UartPolled<libmcuhw::Uart0Address> uart_polled_peripheral;

/**
 * @brief Initialize the development board
 *
 */
void board_init(void);

#endif