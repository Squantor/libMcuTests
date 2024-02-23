/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the LPC844M201 SCT peripheral
 */
#include <nuclone_LPC844M201BD64_tests.hpp>
#include <MinUnit.h>
#include <LPC844M201_teardown.hpp>
#include <common.hpp>

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC844M201SetupSct)
{
    minUnitCheck(LPC844M201TeardownCorrect() == true);
}

MINUNIT_ADD(LPC844M201Sct, LPC844M201SetupSct, LPC844M201Teardown)
{
    sysconEnableClocks(SYSCON, CLKCTRL0_SCT | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
    sysconEnableResets(SYSCON, RESETCTRL0_SCT | RESETCTRL0_SWM | RESETCTRL0_IOCON, RESETCTRL1_NONE);
    minUnitCheck(1 == 1);
    sysconDisableClocks(SYSCON, RESETCTRL0_SCT | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
}