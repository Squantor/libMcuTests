/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief main test entry point for RP2040 libMcuLL tests
 */
#include <nuclone_RP2040.hpp>
#include <MinUnit.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * @brief reporting function for uart output
 *
 * Maybe interesting to send to a large RAM buffer?
 *
 * @param string
 */
void minunitReport(const char* string) {}

/**
 * @brief main program entry point
 *
 * @return int
 */
int main() {
  boardInit();
  minunitRun();
  if (minunitTestState.failures != 0) {
    minunitReport("Test failures detected\n");
  } else {
    minunitReport("All tests passed\n");
  }
  while (1) __BKPT(1);
}