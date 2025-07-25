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
static constexpr libmcu::HwAddressType dutAddress = libmcuhw::PadsBank0Address;
libmcuhw::padsBank0::PadsBank0 *const dutRegisters{reinterpret_cast<libmcuhw::padsBank0::PadsBank0 *>(dutAddress)};
static constexpr libmcu::HwAddressType ioBankAddress = libmcuhw::IoBank0Address;
libmcuhw::gpio_bank0::GpioBank0 *const gpioRegisters{reinterpret_cast<libmcuhw::gpio_bank0::GpioBank0 *>(ioBankAddress)};

/**
 * @brief pads setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupPads) {
  minUnitCheck(Rp2040Teardown_correct() == true);
  resets_peripheral.Reset(libmcull::resets::IoBank0 | libmcull::resets::PadsBank0, 100000);
}

MINUNIT_ADD(RP2040pads, RP2040SetupPads, Rp2040Teardown) {
  // check pullup on one pin pair
  pads_bank0_peripheral.Setup(gpio0Pin, libmcull::pads::DriveModes::Current4mA, libmcull::pads::PullModes::None, false, false);
  pads_bank0_peripheral.Setup(gpio1Pin, libmcull::pads::DriveModes::Current4mA, libmcull::pads::PullModes::PullUp, false, false);
  // check register settings
  minUnitCheck(dutRegisters->GPIO[0] == 0x00'00'00'50u);
  minUnitCheck(dutRegisters->GPIO[1] == 0x00'00'00'58u);
  libmcu::Delay(100);  // wait time until everything propagates
  // check INFROMPAD flag in GPIO status
  minUnitCheck((gpioRegisters->GPIO[0].STATUS & libmcuhw::gpio_bank0::STATUS::RESERVED_MASK) == 0x05'0A'00'00u);
  minUnitCheck((gpioRegisters->GPIO[1].STATUS & libmcuhw::gpio_bank0::STATUS::RESERVED_MASK) == 0x05'0A'00'00u);
  pads_bank0_peripheral.Setup(gpio0Pin, libmcull::pads::DriveModes::Current4mA, libmcull::pads::PullModes::PullDown, false, false);
  pads_bank0_peripheral.Setup(gpio1Pin, libmcull::pads::DriveModes::Current4mA, libmcull::pads::PullModes::None, false, false);
  minUnitCheck(dutRegisters->GPIO[1] == 0x00'00'00'50u);
  libmcu::Delay(100);
  minUnitCheck((gpioRegisters->GPIO[0].STATUS & libmcuhw::gpio_bank0::STATUS::RESERVED_MASK) == 0x00'00'00'00u);
  minUnitCheck((gpioRegisters->GPIO[1].STATUS & libmcuhw::gpio_bank0::STATUS::RESERVED_MASK) == 0x00'00'00'00u);
}

MINUNIT_ADD(RP2040PadsPullModes, RP2040SetupPads, Rp2040Teardown) {
  pads_bank0_peripheral.Setup(gpio0Pin, libmcull::pads::PullModes::PullUp);
  pads_bank0_peripheral.Setup(gpio1Pin, libmcull::pads::PullModes::PullDown);
  minUnitCheck(dutRegisters->GPIO[0] == 0x00'00'00'58u);
  minUnitCheck(dutRegisters->GPIO[1] == 0x00'00'00'54u);
}