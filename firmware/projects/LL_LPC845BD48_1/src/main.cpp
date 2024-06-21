/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief main test entry point for LPC844 libMcuLL tests
 */
#include <nuclone_LPC845BD48_small_LL.hpp>
#include <MinUnit.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"

void minunitReport(const char* string) {}

void minunitFailCallback() {
  LIBMCULL_BKPT(0x00);
}

int main() {
  boardInit();
  minunitRun();
  if (minunitTestState.failures != 0) {
    minunitReport("Test failures detected\n");
  } else {
    minunitReport("All tests passed\n");
  }
  while (1)
    LIBMCULL_BKPT(0xFE);
}
