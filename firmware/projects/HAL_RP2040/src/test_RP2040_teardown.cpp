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
#include <RP2040_teardown.hpp>

MINUNIT_ADD(MicrocontrollerTeardown, NULL, NULL) {
  MicrocontrollerTeardown(testResults);  // passing test state directly
  minUnitCheck(MicrocontrollerTeardownCorrect() == true);
}