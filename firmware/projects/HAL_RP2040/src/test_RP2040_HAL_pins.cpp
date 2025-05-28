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

// peripheral register sets
static constexpr libmcu::HwAddressType padsBank0Location = libmcuhw::padsBank0Address;
libmcuhw::padsBank0::padsBank0 *const padsBank0Registers{reinterpret_cast<libmcuhw::padsBank0::padsBank0 *>(padsBank0Location)};
static constexpr libmcu::HwAddressType ioBank0Location = libmcuhw::ioBank0Address;
libmcuhw::gpioBank0::gpioBank0 *const ioBank0Registers{reinterpret_cast<libmcuhw::gpioBank0::gpioBank0 *>(ioBank0Location)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupHalPins) {
  pinsHal.initialize();
  minUnitPass();
}

MINUNIT_ADD(RP2040HalPins, RP2040SetupHalPins, RP2040Teardown) {
  pinsHal.setup(gpio0Pin, libmcuhal::pins::driveModes::DRIVE_8MA, libmcuhal::pins::pullModes::PULLUP,
                libmcuhal::pins::speedModes::SLEW_SLOW, true);
  minUnitCheck(padsBank0Registers->GPIO[0] == (libmcuhw::pads::GPIO::kRESERVED_MASK & 0x0000'006Au));
  minUnitCheck(ioBank0Registers->GPIO[0].CTRL == (libmcuhw::gpioBank0::CTRL::kRESERVED_MASK & 0x0000'0005u));
  pinsHal.reset(gpio0Pin);
  minUnitCheck(padsBank0Registers->GPIO[0] == (libmcuhw::pads::GPIO::kRESERVED_MASK & 0x0000'0056u));
  minUnitCheck(ioBank0Registers->GPIO[0].CTRL == (libmcuhw::gpioBank0::CTRL::kRESERVED_MASK & 0x0000'001Fu));
  pinsHal.setup(spiSckPin, libmcuhal::pins::driveModes::DRIVE_4MA, libmcuhal::pins::pullModes::NONE,
                libmcuhal::pins::speedModes::SLEW_FAST, false);
  minUnitCheck(padsBank0Registers->GPIO[2] == (libmcuhw::pads::GPIO::kRESERVED_MASK & 0x0000'0051u));
  minUnitCheck(ioBank0Registers->GPIO[2].CTRL == (libmcuhw::gpioBank0::CTRL::kRESERVED_MASK & 0x0000'0001u));
  pinsHal.reset(spiSckPin);
  minUnitCheck(padsBank0Registers->GPIO[2] == (libmcuhw::pads::GPIO::kRESERVED_MASK & 0x0000'0056u));
  minUnitCheck(ioBank0Registers->GPIO[2].CTRL == (libmcuhw::gpioBank0::CTRL::kRESERVED_MASK & 0x0000'001Fu));
  minUnitPass();
}