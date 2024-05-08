/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief main test entry point for driver tests for the 3to8 multiplexer and 74595 shift register
 */
#include <nuclone_RP2040_DRV_mux_595.hpp>
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