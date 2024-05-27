/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 I2C
 */
#include <nuclone_RP2040_LL.hpp>
#include <MinUnit.h>
#include <RP2040_LL_teardown.hpp>
#include <common.hpp>

using namespace libMcuLL;

// peripheral register sets
static constexpr hwAddressType spi0Address = hw::spi0Address;
hw::spi::peripheral *const spi0Registers{reinterpret_cast<hw::spi::peripheral *>(spi0Address)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040LLSetupI2C) {
  minUnitCheck(RP2040TeardownCorrect() == true);
  resetsPeripheral.reset(sw::resets::IO_BANK0 | sw::resets::PADS_BANK0 | sw::resets::I2C0, 100000);
  // connect all GPIO's
}

MINUNIT_ADD(RP2040LLI2cSetup, RP2040LLSetupI2C, RP2040Teardown) {
  minUnitPass();
}
