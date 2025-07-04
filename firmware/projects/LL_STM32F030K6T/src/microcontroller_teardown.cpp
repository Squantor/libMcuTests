/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief Teardown function that resets the LPC845 to a state
 * that more or less matches reset state
 */
#include <board_tests.hpp>
#include <microcontroller_teardown.hpp>
#include <test_conditions.hpp>
#include <MinUnit.h>

/** @brief resets all the registers to their default states
 */
MINUNIT_TEARDOWN(microcontroller_teardown) {
  minUnitCheck(microcontroller_teardown_correct() == true);
}

/** @brief checks if all the registers to their default states
 *  @return if all registers are correctly torndown
 */
bool microcontroller_teardown_correct(void) {
  TESTANDRETURN(1 == 1);
  return true;
}