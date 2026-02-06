/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the LPC844M201 teardown
 */
#include <board_tests.hpp>
#include <minunit.h>
#include <microcontroller_teardown.hpp>

MINUNIT_ADD(microcontroller_teardown, NULL, NULL) {
  microcontroller_teardown(testResults);  // passing test state directly
  MINUNIT_CHECK(microcontroller_teardown_correct() == true);
}