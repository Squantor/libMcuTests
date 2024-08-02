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
// connected pin 1 and 2 via 3.3K resistors, links UART RX/TX, PIN1 on board
using pin1Type = libMcu::hw::pin<libMcu::hw::IOports::PORT1, libMcu::hw::IOpins::PIN08>;
using pin2Type = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN17>;
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO, PIN3 on board
using pin3Type = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN13>;
using pin4Type = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN18>;
// connected pin 5 and 6 via 3.3K resistors, links SPI CS/SCK, PIN2 on board
using pin5Type = libMcu::hw::pin<libMcu::hw::IOports::PORT1, libMcu::hw::IOpins::PIN09>;
using pin6Type = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN19>;
// connected pin 7 and 8 via 3.3K resistors
using pin7Type = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN04>;
using pin8Type = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN20>;
// SDA pin with 2.2K pull up resistor
using i2cSdaPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN11>;
// SCL pin with 2.2K pull up resistor
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
constexpr pin1Type testPin1;
constexpr pin2Type testPin2;
constexpr pin3Type testPin3;
constexpr pin4Type testPin4;
constexpr pin5Type testPin5;
constexpr pin6Type testPin6;
constexpr pin7Type testPin7;
constexpr pin8Type testPin8;

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