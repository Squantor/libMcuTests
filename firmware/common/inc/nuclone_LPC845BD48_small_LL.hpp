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

#define CLOCK_XTAL (12000000u)
#define CLOCK_EXT_IN (0u)
#define CLOCK_CPU (12000000u)
#define CLOCK_AHB (12000000u)
#define CLOCK_MAIN (12000000u)

#include <nxp/libmcu_LPC845M301BD48.hpp>

#define TICKS_PER_S 10

// pin types
// Crystal osillator pins
using xtalInPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN08>;
using xtalOutPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT0, libMcu::hw::IOpins::PIN09>;
using testPinType = libMcu::hw::pin<libMcu::hw::IOports::PORT1, libMcu::hw::IOpins::PIN05>;

// function types
using xtalInFunctionType = libMcu::hw::swm::pinFunction<libMcu::hw::swm::pinFunctions::XTALIN>;
using xtalOutFunctionType = libMcu::hw::swm::pinFunction<libMcu::hw::swm::pinFunctions::XTALOUT>;
using clockOutFunctionType = libMcu::hw::swm::pinFunction<libMcu::hw::swm::pinFunctions::CLKOUT_O>;

// pin instances
constexpr xtalInPinType xtalInPin;
constexpr xtalOutPinType xtalOutPin;
constexpr testPinType testPin;

// function instances
constexpr xtalInFunctionType xtalInFunction;
constexpr xtalOutFunctionType xtalOutFunction;
constexpr clockOutFunctionType clockOutFunction;

// peripheral externs
extern libMcu::ll::iocon::iocon<libMcu::hw::ioconAddress> ioconPeripheral;
extern libMcu::ll::swm::swm<libMcu::hw::swmAddress> swmPeriperhal;
extern libMcu::ll::gpio::gpio<libMcu::hw::gpioAddress> gpioPeripheral;
extern libMcu::ll::syscon::syscon<libMcu::hw::sysconAddress> sysconPeripheral;

void boardInit(void);

#endif