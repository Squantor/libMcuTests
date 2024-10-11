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
constexpr spiSckPinType spiSckPin;

extern libMcuLL::systick::systick<libMcuHw::systickAddress> systickPeripheral;
extern libMcuLL::nvic::nvic<libMcuHw::nvicAddress, libMcuHw::scbAddress> nvicPeripheral;
extern libMcuLL::resets::resets<libMcuHw::resetsAddress> resetsPeripheral;

extern libMcuHal::pins::pins<libMcuHw::padsBank0Address, libMcuHw::ioBank0Address> pinsHal;
extern libMcuHal::gpio::gpio<libMcuHw::padsBank0Address, libMcuHw::ioBank0Address, libMcuHw::sioAddress> gpioHal;

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