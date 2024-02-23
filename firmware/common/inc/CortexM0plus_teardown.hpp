/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef CORTEXM0PLUS_TEARDOWN_HPP
#define CORTEXM0PLUS_TEARDOWN_HPP

#include <MinUnit.h>

/** @brief resets all the registers to their default states
 *
 */
MINUNIT_TEARDOWN(CortexM0plusTeardown);

/** @brief checks if all registers are in their default states
 *  @return false if a register is not properly set
 */
bool CortexM0plusTeardownCorrect(void);

#endif