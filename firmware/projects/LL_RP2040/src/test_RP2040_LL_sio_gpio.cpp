/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 sio gpio
 */
#include <nuclone_RP2040_LL.hpp>
#include <MinUnit.h>
#include <RP2040_LL_teardown.hpp>
#include <common.hpp>

// peripheral register sets
static constexpr libmcu::HwAddressType dutAddress = libmcuhw::sioAddress;
libmcuhw::sio::sio *const dutRegisters{reinterpret_cast<libmcuhw::sio::sio *>(dutAddress)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupSio) {
  minUnitCheck(RP2040TeardownCorrect() == true);
  resetsPeripheral.reset(libmcull::resets::IO_BANK0 | libmcull::resets::PADS_BANK0, 100000);
  // connect all GPIO's
  gpioBank0Peripheral.setup(gpio0Pin);
  padsBank0Peripheral.setup(gpio0Pin, libmcull::pads::driveModes::DRIVE_8MA, false, false, true, false);
  gpioBank0Peripheral.setup(gpio1Pin);
  padsBank0Peripheral.setup(gpio1Pin, libmcull::pads::driveModes::DRIVE_8MA, false, false, true, false);
}

MINUNIT_ADD(RP2040gpio, RP2040SetupSio, RP2040Teardown) {
  sioGpioPeripheral.input(gpio1Pin);
  sioGpioPeripheral.output(gpio0Pin);
  sioGpioPeripheral.high(gpio0Pin);
  minUnitCheck(sioGpioPeripheral.get(gpio1Pin) != 0u);
  sioGpioPeripheral.low(gpio0Pin);
  minUnitCheck(sioGpioPeripheral.get(gpio1Pin) == 0u);
  // switch around pins
  sioGpioPeripheral.input(gpio0Pin);
  sioGpioPeripheral.output(gpio1Pin);
  sioGpioPeripheral.high(gpio1Pin);
  minUnitCheck(sioGpioPeripheral.get(gpio0Pin) != 0u);
  sioGpioPeripheral.low(gpio1Pin);
  minUnitCheck(sioGpioPeripheral.get(gpio0Pin) == 0u);
  // testing the remainder methods
  sioGpioPeripheral.toggle(gpio1Pin);
  minUnitCheck(sioGpioPeripheral.get(gpio0Pin) != 0u);
  sioGpioPeripheral.toggle(gpio1Pin);
  minUnitCheck(sioGpioPeripheral.get(gpio0Pin) == 0u);
  sioGpioPeripheral.set(gpio1Pin, 1);
  minUnitCheck(sioGpioPeripheral.get(gpio0Pin) != 0u);
  sioGpioPeripheral.set(gpio1Pin, 0);
  minUnitCheck(sioGpioPeripheral.get(gpio0Pin) == 0u);
}