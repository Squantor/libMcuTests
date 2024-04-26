/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 gpio
 */
#include <nuclone_RP2040.hpp>
#include <MinUnit.h>
#include <RP2040_teardown.hpp>
#include <common.hpp>

using namespace libMcuLL;

// peripheral register sets
static constexpr hwAddressType dutAddress = hw::gpioBank0Address;
hw::gpioBank0::peripheral *const dutRegisters{reinterpret_cast<hw::gpioBank0::peripheral *>(dutAddress)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupGpio) {
  minUnitCheck(RP2040TeardownCorrect() == true);
  resetsPeripheral.reset(sw::resets::IO_BANK0 | sw::resets::PADS_BANK0, 100000);
  // connect all GPIO's
  gpioBank0Peripheral.setup(gpio0Pin);
  gpioBank0Peripheral.setup(gpio1Pin);
  gpioBank0Peripheral.setup(gpio2Pin);
  gpioBank0Peripheral.setup(gpio3Pin);
  gpioBank0Peripheral.setup(gpio4Pin);
  gpioBank0Peripheral.setup(gpio5Pin);
}

MINUNIT_ADD(RP2040gpio, RP2040SetupGpio, RP2040Teardown) {
  minUnitCheck(dutRegisters->GPIO[0].CTRL == 0x0000'0005u);
  minUnitCheck(dutRegisters->GPIO[1].CTRL == 0x0000'0005u);
  minUnitCheck(dutRegisters->GPIO[2].CTRL == 0x0000'0005u);
  minUnitCheck(dutRegisters->GPIO[3].CTRL == 0x0000'0005u);
  minUnitCheck(dutRegisters->GPIO[4].CTRL == 0x0000'0005u);
  minUnitCheck(dutRegisters->GPIO[5].CTRL == 0x0000'0005u);
  minUnitPass();
}