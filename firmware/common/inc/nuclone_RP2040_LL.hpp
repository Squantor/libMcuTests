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
using gpio0PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin00, libmcuhw::IoFunctions::kSio>;
using gpio1PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin01, libmcuhw::IoFunctions::kSio>;
using uartOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin00, libmcuhw::IoFunctions::kUart0Tx>;
using uartInPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin01, libmcuhw::IoFunctions::kUArt0Rx>;
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO, PIN3 on board
using gpio2PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin03, libmcuhw::IoFunctions::kSio>;
using gpio3PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin04, libmcuhw::IoFunctions::kSio>;
using spiOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin03, libmcuhw::IoFunctions::kSpi0Tx>;
using spiInPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin04, libmcuhw::IoFunctions::kSpi0Rx>;
// connected pin 2 and 5 via 3.3K resistors, links SPI CS/SCK, PIN2 on board
using gpio4PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin02, libmcuhw::IoFunctions::kSio>;
using gpio5PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin05, libmcuhw::IoFunctions::kSio>;
using spiSckPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin02, libmcuhw::IoFunctions::kSpi0Sck>;
using spiCsPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin05, libmcuhw::IoFunctions::kSpi0Cs>;
// connected pin 16 to 2.2K pull up resistor, SDA
using i2cSdaPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin16, libmcuhw::IoFunctions::kI2c0Sda>;
// connected pin 17 to 2.2K pull up resistor, SCL
using i2cSclPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin17, libmcuhw::IoFunctions::kI2c0Scl>;

// LED connected pin 25 via a resistor to ground
using ledPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin25, libmcuhw::IoFunctions::kSio>;
// clock sense connected to pin 21
using clockOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin21, libmcuhw::IoFunctions::kClkGpout0>;

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
extern libmcull::pads::PadsBank0<libmcuhw::kPadsBank0Address> padsBank0Peripheral;
extern libmcull::gpioBank0::GpioBank0<libmcuhw::kIoBank0Address> gpioBank0Peripheral;
extern libmcull::resets::Resets<libmcuhw::kResetsAddress> resetsPeripheral;
extern libmcull::sioGpio::SioGpio<libmcuhw::kSioAddress> sioGpioPeripheral;
extern libmcull::clocks::Clocks<libmcuhw::kClocksAddress> clocksPeripheral;
extern libmcull::xosc::Xosc<libmcuhw::kXoscAddress> xoscPeripheral;
extern libmcull::pll::Pll<libmcuhw::kPllSysAddress> pllSysPeripheral;
extern libmcull::pll::Pll<libmcuhw::kPllUsbAddress> pllUsbPeripheral;
extern libmcull::spi::SpiPolled<libmcuhw::kSpi0Address> spiPeripheral;
extern libmcull::i2c::I2cPolled<libmcuhw::kI2c0Address> i2cPeripheral;
extern libmcull::uart::UartPolled<libmcuhw::kUart0Address> uartPeripheral;

/**
 * @brief Initialize the development board
 *
 */
void boardInit(void);

#endif