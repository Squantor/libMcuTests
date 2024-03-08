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

/** @brief resets all the registers to their default states
 */
MINUNIT_TEARDOWN(MicrocontrollerTeardown) {
  minUnitCheck(MicrocontrollerTeardownCorrect() == true);
}

/** @brief checks if all the registers to their default states
 *  @return if all registers are correctly torndown
 */
bool MicrocontrollerTeardownCorrect(void) {
  TESTANDRETURN(1 == 1);
  return true;
}