/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief definitions for LPC845 small nuclone LL tester
 */
#ifndef NUCLONE_LPC845BD48_SMALL_LL_HPP
#define NUCLONE_LPC845BD48_SMALL_LL_HPP

#define CLOCK_XTAL (12'000'000u)
#define CLOCK_EXT_IN (0u)
#define CLOCK_CPU (30'000'000u)
#define CLOCK_AHB (30'000'000u)
#define CLOCK_MAIN (60'000'000u)

#include <nxp/libmcu_LPC845M301BD48.hpp>

#define TICKS_PER_S 10

// pin types
// Crystal osillator pins
using xtalInPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN08>;
using xtalOutPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN09>;
using testPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT1, libMcu::hw::IOpins::PIN05>;
// connected pin 0 and 1 via 3.3K resistors, links UART RX/TX, PIN1 on board
// using gpio0PinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN00, libMcu::hw::IOfuncts::SIO>;
// using gpio1PinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN01, libMcu::hw::IOfuncts::SIO>;
// using uartOutPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN00, libMcu::hw::IOfuncts::UART0_TX>;
// using uartInPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN01, libMcu::hw::IOfuncts::UART0_RX>;
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO, PIN3 on board
// using gpio2PinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN03, libMcu::hw::IOfuncts::SIO>;
// using gpio3PinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN04, libMcu::hw::IOfuncts::SIO>;
// using spiOutPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN03, libMcu::hw::IOfuncts::SPI0_TX>;
// using spiInPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN04, libMcu::hw::IOfuncts::SPI0_RX>;
// connected pin 2 and 5 via 3.3K resistors, links SPI CS/SCK, PIN2 on board
// using gpio4PinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN02, libMcu::hw::IOfuncts::SIO>;
// using gpio5PinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN05, libMcu::hw::IOfuncts::SIO>;
// using spiSckPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN02, libMcu::hw::IOfuncts::SPI0_SCK>;
// using spiCsPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN05, libMcu::hw::IOfuncts::SPI0_CS>;
// connected pin 16 to 2.2K pull up resistor, SDA
using i2cSdaPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN11>;
// connected pin 17 to 2.2K pull up resistor, SCL
using i2cSclPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN10>;

// function types
using xtalInFunctionType = libMcu::hw::swm::pinFunction<libMcu::hw::swm::pinFunctions::XTALIN>;
using xtalOutFunctionType = libMcu::hw::swm::pinFunction<libMcu::hw::swm::pinFunctions::XTALOUT>;
using clockOutFunctionType = libMcu::hw::swm::pinFunction<libMcu::hw::swm::pinFunctions::CLKOUT_O>;
using mainI2cSdaFunctionType = libMcu::hw::swm::pinFunction<libMcu::hw::swm::pinFunctions::I2C0_SDA_IO>;
using mainI2cSclFunctionType = libMcu::hw::swm::pinFunction<libMcu::hw::swm::pinFunctions::I2C0_SCL_IO>;

// pin instances
constexpr xtalInPinType xtalInPin;
constexpr xtalOutPinType xtalOutPin;
constexpr testPinType testPin;
constexpr i2cSclPinType i2cSclPin;
constexpr i2cSdaPinType i2cSdaPin;

// function instances
constexpr xtalInFunctionType xtalInFunction;
constexpr xtalOutFunctionType xtalOutFunction;
constexpr clockOutFunctionType clockOutFunction;
constexpr mainI2cSclFunctionType i2cMainSclFunction;
constexpr mainI2cSdaFunctionType i2cMainSdaFunction;

// peripheral externs
extern libMcu::ll::iocon::iocon<libMcu::hw::ioconAddress> ioconPeripheral;
extern libMcu::ll::swm::swm<libMcu::hw::swmAddress> swmPeriperhal;
extern libMcu::ll::gpio::gpio<libMcu::hw::gpioAddress> gpioPeripheral;
extern libMcu::ll::syscon::syscon<libMcu::hw::sysconAddress> sysconPeripheral;

void boardInit(void);

#endif