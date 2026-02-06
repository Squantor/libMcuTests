/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief main test entry point for RP2040 libMcuLL tests
 */
#include <nuclone_RP2040_LL.hpp>
#include <minunit.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"

void minunitReport(const char* string) {}

void minunitFailCallback() {
  LIBMCULL_BKPT(0x00);
}

int main() {
  board_init();
  minunit_run();
  if (minunit_test_state.failures != 0) {
    minunitReport("Test failures detected\n");
  } else {
    minunitReport("All tests passed\n");
  }
  while (1)
    LIBMCULL_BKPT(0xFE);
}