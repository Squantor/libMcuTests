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
#include <MinUnit.h>
#include <microcontroller_teardown.hpp>

MINUNIT_ADD(MicrocontrollerTeardown, NULL, NULL)
{
    MicrocontrollerTeardown(testResults); // passing test state directly
    minUnitCheck(MicrocontrollerTeardownCorrect() == true);
}