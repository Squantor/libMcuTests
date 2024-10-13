/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file main.cpp
 * @brief main test entry point for STM32F031 libmcu low level tests
 */
#include <nuclone_STM32F031K6T_LL.hpp>
#include <MinUnit.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"

void minunitReport(const char* string) {}

int main() {
  boardInit();
  minunitRun();
  if (minunitTestState.failures != 0) {
    minunitReport("Test failures detected\n");
  } else {
    minunitReport("All tests passed\n");
  }
  while (1)
    ;
}
