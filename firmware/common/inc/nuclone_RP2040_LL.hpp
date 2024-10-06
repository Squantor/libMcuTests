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
using gpio0PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN00, libMcuLL::hw::IOfuncts::SIO>;
using gpio1PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN01, libMcuLL::hw::IOfuncts::SIO>;
using uartOutPinType =
  libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN00, libMcuLL::hw::IOfuncts::UART0_TX>;
using uartInPinType =
  libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN01, libMcuLL::hw::IOfuncts::UART0_RX>;
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO, PIN3 on board
using gpio2PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN03, libMcuLL::hw::IOfuncts::SIO>;
using gpio3PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN04, libMcuLL::hw::IOfuncts::SIO>;
using spiOutPinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN03, libMcuLL::hw::IOfuncts::SPI0_TX>;
using spiInPinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN04, libMcuLL::hw::IOfuncts::SPI0_RX>;
// connected pin 2 and 5 via 3.3K resistors, links SPI CS/SCK, PIN2 on board
using gpio4PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN02, libMcuLL::hw::IOfuncts::SIO>;
using gpio5PinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN05, libMcuLL::hw::IOfuncts::SIO>;
using spiSckPinType =
  libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN02, libMcuLL::hw::IOfuncts::SPI0_SCK>;
using spiCsPinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN05, libMcuLL::hw::IOfuncts::SPI0_CS>;
// connected pin 16 to 2.2K pull up resistor, SDA
using i2cSdaPinType =
  libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN16, libMcuLL::hw::IOfuncts::I2C0_SDA>;
// connected pin 17 to 2.2K pull up resistor, SCL
using i2cSclPinType =
  libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN17, libMcuLL::hw::IOfuncts::I2C0_SCL>;

// LED connected pin 25 via a resistor to ground
using ledPinType = libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN25, libMcuLL::hw::IOfuncts::SIO>;
// clock sense connected to pin 21
using clockOutPinType =
  libMcuLL::hw::pin<libMcuLL::hw::IOports::PORT0, libMcuLL::hw::IOpins::PIN21, libMcuLL::hw::IOfuncts::CLK_GPOUT0>;

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

extern libMcuLL::systick::systick<libMcuHw::systickAddress> systickPeripheral;
extern libMcuLL::nvic::nvic<libMcuHw::nvicAddress, libMcuHw::scbAddress> nvicPeripheral;
extern libMcuLL::sw::padsBank0::padsBank0<libMcuHw::padsBank0Address> padsBank0Peripheral;
extern libMcuLL::sw::gpioBank0::gpioBank0<libMcuHw::ioBank0Address> gpioBank0Peripheral;
extern libMcuLL::sw::resets::resets<libMcuHw::resetsAddress> resetsPeripheral;
extern libMcuLL::sw::sioGpio::sioGpio<libMcuHw::sioAddress> sioGpioPeripheral;
extern libMcuLL::sw::clocks::clocks<libMcuHw::clocksAddress> clocksPeripheral;
extern libMcuLL::sw::xosc::xosc<libMcuHw::xoscAddress> xoscPeripheral;
extern libMcuLL::sw::pll::pll<libMcuHw::pllSysAddress> pllSysPeripheral;
extern libMcuLL::sw::pll::pll<libMcuHw::pllUsbAddress> pllUsbPeripheral;
extern libMcuLL::sw::spi::spi<libMcuHw::spi0Address> spiPeripheral;
extern libMcuLL::sw::i2c::i2c<libMcuHw::i2c0Address> i2cPeripheral;
extern libMcuLL::sw::uart::uart<libMcuHw::uart0Address> uartPeripheral;

/**
 * @brief Initialize the development board
 *
 */
void boardInit(void);

#endif