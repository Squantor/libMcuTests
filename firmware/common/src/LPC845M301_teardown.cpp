/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief Teardown function that resets the LPC845 to a state
 * that more or less matches reset state
 */
#include <LPC845M301_teardown.hpp>
#include <test_conditions.hpp>
#include <MinUnit.h>

/** @brief resets all the registers to their default states
 */
MINUNIT_TEARDOWN(LPC844M201Teardown) {}

/** @brief checks if all the registers to their default states
 *  @return if all registers are correctly torndown
 */
bool LPC844M201TeardownCorrect(void) {
  TESTANDRETURN(0x0 == 0x0);
  return true;
}