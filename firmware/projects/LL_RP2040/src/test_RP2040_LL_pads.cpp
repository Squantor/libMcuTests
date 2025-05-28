/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 gpio
 */
#include <nuclone_RP2040_LL.hpp>
#include <MinUnit.h>
#include <RP2040_LL_teardown.hpp>
#include <common.hpp>

// peripheral register sets
static constexpr libmcu::HwAddressType dutAddress = libmcuhw::padsBank0Address;
libmcuhw::padsBank0::padsBank0 *const dutRegisters{reinterpret_cast<libmcuhw::padsBank0::padsBank0 *>(dutAddress)};
static constexpr libmcu::HwAddressType ioBankAddress = libmcuhw::ioBank0Address;
libmcuhw::gpioBank0::gpioBank0 *const gpioRegisters{reinterpret_cast<libmcuhw::gpioBank0::gpioBank0 *>(ioBankAddress)};

/**
 * @brief pads setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupPads) {
  minUnitCheck(RP2040TeardownCorrect() == true);
  resetsPeripheral.reset(libmcull::resets::IO_BANK0 | libmcull::resets::PADS_BANK0, 100000);
}

MINUNIT_ADD(RP2040pads, RP2040SetupPads, RP2040Teardown) {
  // check pullup on one pin pair
  padsBank0Peripheral.setup(gpio0Pin, libmcull::pads::driveModes::DRIVE_4MA, false, false, false, false);
  padsBank0Peripheral.setup(gpio1Pin, libmcull::pads::driveModes::DRIVE_4MA, true, false, false, false);
  // check register settings
  minUnitCheck(dutRegisters->GPIO[0] == 0x00'00'00'50u);
  minUnitCheck(dutRegisters->GPIO[1] == 0x00'00'00'58u);
  libmcu::Delay(100);  // wait time until everything propagates
  // check INFROMPAD flag in GPIO status
  minUnitCheck((gpioRegisters->GPIO[0].STATUS & libmcuhw::gpioBank0::STATUS::kRESERVED_MASK) == 0x05'0A'00'00u);
  minUnitCheck((gpioRegisters->GPIO[1].STATUS & libmcuhw::gpioBank0::STATUS::kRESERVED_MASK) == 0x05'0A'00'00u);
  padsBank0Peripheral.setup(gpio0Pin, libmcull::pads::driveModes::DRIVE_4MA, false, true, false, false);
  padsBank0Peripheral.setup(gpio1Pin, libmcull::pads::driveModes::DRIVE_4MA, false, false, false, false);
  minUnitCheck(dutRegisters->GPIO[1] == 0x00'00'00'50u);
  libmcu::Delay(100);
  minUnitCheck((gpioRegisters->GPIO[0].STATUS & libmcuhw::gpioBank0::STATUS::kRESERVED_MASK) == 0x00'00'00'00u);
  minUnitCheck((gpioRegisters->GPIO[1].STATUS & libmcuhw::gpioBank0::STATUS::kRESERVED_MASK) == 0x00'00'00'00u);
}