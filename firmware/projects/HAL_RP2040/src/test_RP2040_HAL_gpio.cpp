/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 gpio
 */
#include <nuclone_RP2040_HAL.hpp>
#include <MinUnit.h>
#include <RP2040_HAL_teardown.hpp>
#include <common.hpp>

using namespace libMcuLL;
using namespace libMcuHal;

// peripheral register sets
static constexpr hwAddressType padsBank0Location = hw::padsBank0Address;
hw::padsBank0::peripheral *const padsBank0Registers{reinterpret_cast<hw::padsBank0::peripheral *>(padsBank0Location)};
static constexpr hwAddressType ioBank0Location = hw::ioBank0Address;
hw::gpioBank0::peripheral *const ioBank0Registers{reinterpret_cast<hw::gpioBank0::peripheral *>(ioBank0Location)};
static constexpr hwAddressType sioLocation = hw::sioAddress;
hw::sio::peripheral *const sioRegisters{reinterpret_cast<hw::sio::peripheral *>(sioLocation)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupHalgpio) {
  pinsHal.initialize();
  // setup test pins to gpio mode
  pinsHal.setup(gpio0Pin, pins::driveModes::DRIVE_4MA, pins::pullModes::NONE, pins::speedModes::SLEW_MEDIUM, true);
  pinsHal.setup(gpio1Pin, pins::driveModes::DRIVE_4MA, pins::pullModes::NONE, pins::speedModes::SLEW_MEDIUM, true);
  gpioHal.initialize();
  minUnitPass();
}

MINUNIT_ADD(RP2040HalGpio, RP2040SetupHalgpio, RP2040Teardown) {
  gpioHal.input(gpio0Pin);
  gpioHal.input(gpio1Pin);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0000u);
  gpioHal.output(gpio1Pin);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0002u);
  gpioHal.high(gpio1Pin);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0002u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpioHal.get(gpio1Pin) != 0);
  minUnitCheck(gpioHal.get(gpio0Pin) != 0);
  gpioHal.low(gpio1Pin);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpioHal.get(gpio1Pin) == 0);
  minUnitCheck(gpioHal.get(gpio0Pin) == 0);
  gpioHal.input(gpio1Pin);
  gpioHal.output(gpio0Pin);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0001u);
  gpioHal.set(gpio0Pin, 1);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0001u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0001u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpioHal.get(gpio1Pin) != 0);
  minUnitCheck(gpioHal.get(gpio0Pin) != 0);
  gpioHal.set(gpio0Pin, 0);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpioHal.get(gpio1Pin) == 0);
  minUnitCheck(gpioHal.get(gpio0Pin) == 0);
  gpioHal.toggle(gpio0Pin);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0001u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpioHal.get(gpio1Pin) != 0);
  minUnitCheck(gpioHal.get(gpio0Pin) != 0);
  gpioHal.toggle(gpio0Pin);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpioHal.get(gpio1Pin) == 0);
  minUnitCheck(gpioHal.get(gpio0Pin) == 0);
  gpioHal.input(gpio0Pin);
  gpioHal.pullmode(gpio0Pin, gpio::pullModes::NONE);
  gpioHal.pullmode(gpio1Pin, gpio::pullModes::PULLUP);
  minUnitCheck(padsBank0Registers->GPIO[0] == 0x0000'0052u);
  minUnitCheck(padsBank0Registers->GPIO[1] == 0x0000'005Au);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpioHal.get(gpio1Pin) != 0);
  minUnitCheck(gpioHal.get(gpio0Pin) != 0);
  gpioHal.pullmode(gpio1Pin, gpio::pullModes::PULLDOWN);
  minUnitCheck(padsBank0Registers->GPIO[0] == 0x0000'0052u);
  minUnitCheck(padsBank0Registers->GPIO[1] == 0x0000'0056u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpioHal.get(gpio1Pin) == 0);
  minUnitCheck(gpioHal.get(gpio0Pin) == 0);
  gpioHal.pullmode(gpio1Pin, gpio::pullModes::NONE);
  gpioHal.pullmode(gpio0Pin, gpio::pullModes::PULLUP);
  minUnitCheck(padsBank0Registers->GPIO[0] == 0x0000'005Au);
  minUnitCheck(padsBank0Registers->GPIO[1] == 0x0000'0052u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpioHal.get(gpio1Pin) != 0);
  minUnitCheck(gpioHal.get(gpio0Pin) != 0);
  gpioHal.pullmode(gpio0Pin, gpio::pullModes::PULLDOWN);
  minUnitCheck(padsBank0Registers->GPIO[0] == 0x0000'0056u);
  minUnitCheck(padsBank0Registers->GPIO[1] == 0x0000'0052u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpioHal.get(gpio1Pin) == 0);
  minUnitCheck(gpioHal.get(gpio0Pin) == 0);
}