/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 teardown
 */
#include <nuclone_RP2040.hpp>
#include <MinUnit.h>
#include <Rp2040Teardown.hpp>

MINUNIT_ADD(microcontroller_teardown, NULL, NULL) {
  microcontroller_teardown(testResults);  // passing test state directly
  minUnitCheck(microcontroller_teardown_correct() == true);
}