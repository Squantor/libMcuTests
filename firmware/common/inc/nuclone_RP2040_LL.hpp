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
using gpio0PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN00, libmcuhw::IOfuncts::SIO>;
using gpio1PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN01, libmcuhw::IOfuncts::SIO>;
using uartOutPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN00, libmcuhw::IOfuncts::UART0_TX>;
using uartInPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN01, libmcuhw::IOfuncts::UART0_RX>;
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO, PIN3 on board
using gpio2PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN03, libmcuhw::IOfuncts::SIO>;
using gpio3PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN04, libmcuhw::IOfuncts::SIO>;
using spiOutPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN03, libmcuhw::IOfuncts::SPI0_TX>;
using spiInPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN04, libmcuhw::IOfuncts::SPI0_RX>;
// connected pin 2 and 5 via 3.3K resistors, links SPI CS/SCK, PIN2 on board
using gpio4PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN02, libmcuhw::IOfuncts::SIO>;
using gpio5PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN05, libmcuhw::IOfuncts::SIO>;
using spiSckPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN02, libmcuhw::IOfuncts::SPI0_SCK>;
using spiCsPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN05, libmcuhw::IOfuncts::SPI0_CS>;
// connected pin 16 to 2.2K pull up resistor, SDA
using i2cSdaPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN16, libmcuhw::IOfuncts::I2C0_SDA>;
// connected pin 17 to 2.2K pull up resistor, SCL
using i2cSclPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN17, libmcuhw::IOfuncts::I2C0_SCL>;

// LED connected pin 25 via a resistor to ground
using ledPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN25, libmcuhw::IOfuncts::SIO>;
// clock sense connected to pin 21
using clockOutPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN21, libmcuhw::IOfuncts::CLK_GPOUT0>;

constexpr ledPinType ledPin;
constexpr clockOutPinType clockOutPin;
constexpr gpio0PinType gpio0Pin;
constexpr gpio1PinType gpio1Pin;
constexpr gpio2PinType gpio2Pin;
constexpr gpio3PinType gpio3Pin;
constexpr gpio4PinType gpio4Pin;
constexpr gpio5PinType gpio5Pin;
constexpr spiOutPinType spiOutPin;
constexpr spiInPinType spiInPin;
constexpr spiSckPinType spiSckPin;
constexpr spiCsPinType spiCsPin;
constexpr i2cSclPinType i2cSclPin;
constexpr i2cSdaPinType i2cSdaPin;
constexpr uartInPinType uartRxPin;
constexpr uartOutPinType uartTxPin;

extern libmcull::systick::systick<libmcuhw::systickAddress> systickPeripheral;
extern libmcull::nvic::nvic<libmcuhw::nvicAddress, libmcuhw::scbAddress> nvicPeripheral;
extern libmcull::padsBank0::padsBank0<libmcuhw::padsBank0Address> padsBank0Peripheral;
extern libmcull::gpioBank0::gpioBank0<libmcuhw::ioBank0Address> gpioBank0Peripheral;
extern libmcull::resets::resets<libmcuhw::resetsAddress> resetsPeripheral;
extern libmcull::sioGpio::sioGpio<libmcuhw::sioAddress> sioGpioPeripheral;
extern libmcull::clocks::clocks<libmcuhw::clocksAddress> clocksPeripheral;
extern libmcull::xosc::xosc<libmcuhw::xoscAddress> xoscPeripheral;
extern libmcull::pll::pll<libmcuhw::pllSysAddress> pllSysPeripheral;
extern libmcull::pll::pll<libmcuhw::pllUsbAddress> pllUsbPeripheral;
extern libmcull::spi::spi<libmcuhw::spi0Address> spiPeripheral;
extern libmcull::i2c::i2c<libmcuhw::i2c0Address> i2cPeripheral;
extern libmcull::uart::uart<libmcuhw::uart0Address> uartPeripheral;

/**
 * @brief Initialize the development board
 *
 */
void boardInit(void);

#endif