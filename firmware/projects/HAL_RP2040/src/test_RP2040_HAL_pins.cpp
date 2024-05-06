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
static constexpr hwAddressType padsAddress = hw::padsBank0Address;
hw::padsBank0::peripheral *const padsRegisters{reinterpret_cast<hw::padsBank0::peripheral *>(padsAddress)};
static constexpr hwAddressType ioBankAddress = hw::ioBank0Address;
hw::gpioBank0::peripheral *const gpioRegisters{reinterpret_cast<hw::gpioBank0::peripheral *>(ioBankAddress)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupHalPins) {
  pinsHal.initialize();
  minUnitPass();
}

MINUNIT_ADD(RP2040HalPins, RP2040SetupHalPins, RP2040Teardown) {
  pinsHal.setup(gpio0Pin, pins::driveModes::DRIVE_8MA, pins::pullModes::PULLUP, pins::speedModes::SLEW_SLOW, true);
  minUnitCheck(padsRegisters->GPIO[0] == (hw::pads::GPIO::RESERVED_MASK & 0x0000'006Au));
  // check pinmuxing
  pinsHal.reset(gpio0Pin);
  minUnitCheck(padsRegisters->GPIO[0] == (hw::pads::GPIO::RESERVED_MASK & 0x0000'0056u));
  // check pinmuxing
  // setup another pin to check indexing, preferably not a gpio pin but UART or something
  pinsHal.setup(spiSckPin, pins::driveModes::DRIVE_4MA, pins::pullModes::NONE, pins::speedModes::SLEW_FAST, false);
  minUnitCheck(padsRegisters->GPIO[2] == (hw::pads::GPIO::RESERVED_MASK & 0x0000'0051u));
  // check
  // reset
  // check if reset
  minUnitPass();
}