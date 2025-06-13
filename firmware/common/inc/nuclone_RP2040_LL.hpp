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
using Gpio0PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin00, libmcuhw::IoFunctions::kSio>;
using Gpio1PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin01, libmcuhw::IoFunctions::kSio>;
using UartOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin00, libmcuhw::IoFunctions::kUart0Tx>;
using UartInPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin01, libmcuhw::IoFunctions::kUArt0Rx>;
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO, PIN3 on board
using Gpio2PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin03, libmcuhw::IoFunctions::kSio>;
using gpio3PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin04, libmcuhw::IoFunctions::kSio>;
using SpiOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin03, libmcuhw::IoFunctions::kSpi0Tx>;
using SpiInPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin04, libmcuhw::IoFunctions::kSpi0Rx>;
// connected pin 2 and 5 via 3.3K resistors, links SPI CS/SCK, PIN2 on board
using Gpio4PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin02, libmcuhw::IoFunctions::kSio>;
using Gpio5PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin05, libmcuhw::IoFunctions::kSio>;
using SpiSckPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin02, libmcuhw::IoFunctions::kSpi0Sck>;
using SpiCsPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin05, libmcuhw::IoFunctions::kSpi0Cs>;
// connected pin 16 to 2.2K pull up resistor, SDA
using I2cSdaPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin16, libmcuhw::IoFunctions::kI2c0Sda>;
// connected pin 17 to 2.2K pull up resistor, SCL
using I2cSclPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin17, libmcuhw::IoFunctions::kI2c0Scl>;

// LED connected pin 25 via a resistor to ground
using LedPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin25, libmcuhw::IoFunctions::kSio>;
// clock sense connected to pin 21
using ClockOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin21, libmcuhw::IoFunctions::kClkGpout0>;

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
constexpr SpiSckPinType spiSckPin;
constexpr SpiCsPinType spiCsPin;
constexpr I2cSclPinType i2c_scl_pin;
constexpr I2cSdaPinType i2cSdaPin;
constexpr UartInPinType uartRxPin;
constexpr UartOutPinType uartTxPin;

extern libmcull::systick::Systick<libmcuhw::kSystickAddress> systick_peripheral;
extern libmcull::nvic::Nvic<libmcuhw::kNvicAddress, libmcuhw::kScbAddress> nvic_peripheral;
extern libmcull::pads::PadsBank0<libmcuhw::kPadsBank0Address> pads_bank0_peripheral;
extern libmcull::gpioBank0::GpioBank0<libmcuhw::kIoBank0Address> gpio_bank0_peripheral;
extern libmcull::resets::Resets<libmcuhw::kResetsAddress> resets_peripheral;
extern libmcull::sio_gpio::SioGpio<libmcuhw::kSioAddress> sio_gpio_peripheral;
extern libmcull::clocks::Clocks<libmcuhw::kClocksAddress> clocks_peripheral;
extern libmcull::xosc::Xosc<libmcuhw::kXoscAddress> xosc_peripheral;
extern libmcull::pll::Pll<libmcuhw::kPllSysAddress> pll_sys_peripheral;
extern libmcull::pll::Pll<libmcuhw::kPllUsbAddress> pll_usb_peripheral;
extern libmcull::spi::SpiPolled<libmcuhw::kSpi0Address> spi_polled_peripheral;
extern libmcull::i2c::I2cPolled<libmcuhw::kI2c0Address> i2c_polled_peripheral;
extern libmcull::uart::UartPolled<libmcuhw::kUart0Address> uart_polled_peripheral;

/**
 * @brief Initialize the development board
 *
 */
void board_init(void);

#endif