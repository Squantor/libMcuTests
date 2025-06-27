/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the low level LPC845M301 polled i2c peripheral
 */
#include <nuclone_LPC845BD48_small_LL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>
#include <common.hpp>

namespace hardware = libmcuhw::spi;
namespace lowlevel = libmcull::spi;

// peripheral registers
static constexpr libmcu::HwAddressType i2c_address = libmcuhw::kSpi0Address; /**< peripheral address */
libmcuhw::i2c::I2c *const i2c_registers{reinterpret_cast<libmcuhw::i2c::I2c *>(i2c_address)};

/**
 * @brief I2C setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupI2cPoll) {
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::peripheral_clocks_0::kI2c0 |
                                             libmcull::syscon::peripheral_clocks_0::kSwm |
                                             libmcull::syscon::peripheral_clocks_0::kIocon,
                                           0);
}

/**
 * @brief Tests I2C init functions
 */
MINUNIT_ADD(LPC845M301DH20I2cPollInit, LPC845M301SetupI2cPoll, LPC845M301Teardown) {
  minUnitPass();
}