/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef RP2040_LL_TEARDOWN_HPP
#define RP2040_LL_TEARDOWN_HPP

#include <minunit.h>

/** @brief resets all the registers to their default states
 *
 */
MINUNIT_TEARDOWN(Rp2040Teardown);

/** @brief checks if all registers are in their default states
 *  @return false if a register is not properly set
 */
bool Rp2040Teardown_correct(void);

#endif