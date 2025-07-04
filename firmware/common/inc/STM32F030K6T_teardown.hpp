/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef MICROCONTROLLER_TEARDOWN_HPP
#define MICROCONTROLLER_TEARDOWN_HPP

#include <MinUnit.h>

/** @brief resets all the registers to their default states
 *
 */
MINUNIT_TEARDOWN(microcontroller_teardown);

/** @brief checks if all registers are in their default states
 *  @return false if a register is not properly set
 */
bool microcontroller_teardown_correct(void);

#endif