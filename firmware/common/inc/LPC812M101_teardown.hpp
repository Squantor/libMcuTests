/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef LPC812M101_TEARDOWN_HPP
#define LPC812M101_TEARDOWN_HPP

#include <minunit.h>

/** @brief resets all the registers to their default states
 *
 */
MINUNIT_TEARDOWN(LPC812M101Teardown);

/** @brief checks if all registers are in their default states
 *  @return false if a register is not properly set
 */
bool Lpc812M101TeardownCorrect(void);

#endif