/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef RP2040_TEARDOWN_HPP
#define RP2040_TEARDOWN_HPP

#include <MinUnit.h>

/** @brief resets all the registers to their default states
 * 
 */
MINUNIT_TEARDOWN(MicrocontrollerTeardown);


/** @brief checks if all registers are in their default states
 *  @return false if a register is not properly set
 */
bool MicrocontrollerTeardownCorrect(void);

#endif