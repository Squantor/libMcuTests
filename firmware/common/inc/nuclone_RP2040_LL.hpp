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
using gpio0PinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN00, libMcuHw::IOfuncts::SIO>;
using gpio1PinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN01, libMcuHw::IOfuncts::SIO>;
using uartOutPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN00, libMcuHw::IOfuncts::UART0_TX>;
using uartInPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN01, libMcuHw::IOfuncts::UART0_RX>;
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO, PIN3 on board
using gpio2PinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN03, libMcuHw::IOfuncts::SIO>;
using gpio3PinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN04, libMcuHw::IOfuncts::SIO>;
using spiOutPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN03, libMcuHw::IOfuncts::SPI0_TX>;
using spiInPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN04, libMcuHw::IOfuncts::SPI0_RX>;
// connected pin 2 and 5 via 3.3K resistors, links SPI CS/SCK, PIN2 on board
using gpio4PinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN02, libMcuHw::IOfuncts::SIO>;
using gpio5PinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN05, libMcuHw::IOfuncts::SIO>;
using spiSckPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN02, libMcuHw::IOfuncts::SPI0_SCK>;
using spiCsPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN05, libMcuHw::IOfuncts::SPI0_CS>;
// connected pin 16 to 2.2K pull up resistor, SDA
using i2cSdaPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN16, libMcuHw::IOfuncts::I2C0_SDA>;
// connected pin 17 to 2.2K pull up resistor, SCL
using i2cSclPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN17, libMcuHw::IOfuncts::I2C0_SCL>;

// LED connected pin 25 via a resistor to ground
using ledPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN25, libMcuHw::IOfuncts::SIO>;
// clock sense connected to pin 21
using clockOutPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN21, libMcuHw::IOfuncts::CLK_GPOUT0>;

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
extern libMcuLL::padsBank0::padsBank0<libMcuHw::padsBank0Address> padsBank0Peripheral;
extern libMcuLL::gpioBank0::gpioBank0<libMcuHw::ioBank0Address> gpioBank0Peripheral;
extern libMcuLL::resets::resets<libMcuHw::resetsAddress> resetsPeripheral;
extern libMcuLL::sioGpio::sioGpio<libMcuHw::sioAddress> sioGpioPeripheral;
extern libMcuLL::clocks::clocks<libMcuHw::clocksAddress> clocksPeripheral;
extern libMcuLL::xosc::xosc<libMcuHw::xoscAddress> xoscPeripheral;
extern libMcuLL::pll::pll<libMcuHw::pllSysAddress> pllSysPeripheral;
extern libMcuLL::pll::pll<libMcuHw::pllUsbAddress> pllUsbPeripheral;
extern libMcuLL::spi::spi<libMcuHw::spi0Address> spiPeripheral;
extern libMcuLL::i2c::i2c<libMcuHw::i2c0Address> i2cPeripheral;
extern libMcuLL::uart::uart<libMcuHw::uart0Address> uartPeripheral;

/**
 * @brief Initialize the development board
 *
 */
void boardInit(void);

#endif