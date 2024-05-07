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

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupHalPins) {
  pinsHal.initialize();
  minUnitPass();
}

MINUNIT_ADD(RP2040HalPins, RP2040SetupHalPins, RP2040Teardown) {
  pinsHal.setup(gpio0Pin, pins::driveModes::DRIVE_8MA, pins::pullModes::PULLUP, pins::speedModes::SLEW_SLOW, true);
  minUnitCheck(padsBank0Registers->GPIO[0] == (hw::pads::GPIO::RESERVED_MASK & 0x0000'006Au));
  minUnitCheck(ioBank0Registers->GPIO[0].CTRL == (hw::gpioBank0::CTRL::RESERVED_MASK & 0x0000'0005u));
  pinsHal.reset(gpio0Pin);
  minUnitCheck(padsBank0Registers->GPIO[0] == (hw::pads::GPIO::RESERVED_MASK & 0x0000'0056u));
  minUnitCheck(ioBank0Registers->GPIO[0].CTRL == (hw::gpioBank0::CTRL::RESERVED_MASK & 0x0000'001Fu));
  pinsHal.setup(spiSckPin, pins::driveModes::DRIVE_4MA, pins::pullModes::NONE, pins::speedModes::SLEW_FAST, false);
  minUnitCheck(padsBank0Registers->GPIO[2] == (hw::pads::GPIO::RESERVED_MASK & 0x0000'0051u));
  minUnitCheck(ioBank0Registers->GPIO[2].CTRL == (hw::gpioBank0::CTRL::RESERVED_MASK & 0x0000'0001u));
  pinsHal.reset(spiSckPin);
  minUnitCheck(padsBank0Registers->GPIO[2] == (hw::pads::GPIO::RESERVED_MASK & 0x0000'0056u));
  minUnitCheck(ioBank0Registers->GPIO[2].CTRL == (hw::gpioBank0::CTRL::RESERVED_MASK & 0x0000'001Fu));
  minUnitPass();
}