/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef CORTEXM0PLUS_TEARDOWN_HPP
#define CORTEXM0PLUS_TEARDOWN_HPP

#include <minunit.h>

/** @brief resets all the registers to their default states
 *
 */
MINUNIT_TEARDOWN(cortexm0plus_teardown);

/** @brief checks if all registers are in their default states
 *  @return false if a register is not properly set
 */
bool cortexm0plus_teardown_correct(void);

#endif