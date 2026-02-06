/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 teardown
 */
#include <nuclone_RP2040_LL.hpp>
#include <minunit.h>
#include <RP2040_LL_teardown.hpp>

MINUNIT_ADD(microcontroller_teardown, NULL, NULL) {
  microcontroller_teardown(testResults);  // passing test state directly
  MINUNIT_CHECK(microcontroller_teardown_correct() == true);
}