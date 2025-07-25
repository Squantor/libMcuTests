/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file test_LPC845M301_async_i2c.cpp
 * @brief tests for LPC845M301 HAL I2C interrupt mode
 */

#include <nuclone_LPC845BD48_small_HAL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>

using namespace libmcuhw::i2c;
using namespace libmcuhal::i2c;

static constexpr libmcuhw::HwAddressType usart0_address = libmcuhw::kUsart0Address;
libmcuhw::usart::Usart *const dut_registers{reinterpret_cast<libmcuhw::usart::Usart *>(usart0_address)};

/**
 * @brief LPC845M301 HAL I2C setup
 */
MINUNIT_SETUP(Lpc845m301SetupI2C) {
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::peripheral_clocks_0::I2c0 |
                                             libmcull::syscon::peripheral_clocks_0::Swm |
                                             libmcull::syscon::peripheral_clocks_0::Iocon,
                                           0);
  // swmPeriperhal.setup(test_pin_1, uartMainRxFunction);
  // swmPeriperhal.setup(test_pin_2, uartMainTxFunction);
}

/**
 * @brief Tests UART HAL init functionality
 */
MINUNIT_ADD(Lpc845m301I2cIntInit, Lpc845m301SetupI2C, LPC845M301Teardown) {
  minUnitPass();
}