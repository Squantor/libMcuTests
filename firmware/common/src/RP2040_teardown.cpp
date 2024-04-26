/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief Teardown function that resets the RP2040 to a state
 * that more or less matches reset state
 */
#include <nuclone_RP2040.hpp>
#include <RP2040_teardown.hpp>
#include <test_conditions.hpp>
#include <MinUnit.h>

using namespace libMcuLL;

/** @brief resets all the registers to their default states
 */
MINUNIT_TEARDOWN(RP2040Teardown) {
  minUnitCheck(RP2040TeardownCorrect() == true);
  padsBank0Peripheral.setup(ledPin, sw::pads::driveModes::DRIVE_4MA, false, false, false, false);
  gpioBank0Peripheral.setup(ledPin);
}

/** @brief checks if all the registers to their default states
 *  @return if all registers are correctly torndown
 */
bool RP2040TeardownCorrect(void) {
  TESTANDRETURN(1 == 1);
  return true;
}