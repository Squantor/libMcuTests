/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the LPC844M201 teardown
 */
#include <nuclone_LPC844M201BD64_tests.hpp>
#include <MinUnit.h>
#include <LPC844M201_teardown.hpp>

MINUNIT_ADD(LPC844M201teardown, NULL, NULL)
{
    LPC844M201Teardown(testResults); // passing test state directly
    minUnitCheck(LPC844M201TeardownCorrect() == true);
}