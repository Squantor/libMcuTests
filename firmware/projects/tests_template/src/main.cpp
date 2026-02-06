/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief main test entry point for LPC844 libMcuLL tests
 */
#include <board_tests.hpp>
#include <minunit.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"

void minunitReport(const char* string) {}

int main() {
  board_init();
  minunit_run();
  if (minunit_test_state.failures != 0) {
    minunitReport("Test failures detected\n");
  } else {
    minunitReport("All tests passed\n");
  }
  while (1)
    ;
}
