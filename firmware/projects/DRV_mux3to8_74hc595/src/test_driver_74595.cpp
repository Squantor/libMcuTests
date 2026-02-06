/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the 74595 spi shift out register driver
 */
#include <nuclone_RP2040_DRV_mux_595.hpp>
#include <minunit.h>
#include <common.hpp>

using namespace libmcull;
using namespace libmcuhal;

/**
 * @brief driver setup and initialisation
 */
MINUNIT_SETUP(driver74595Setup) {
  MINUNIT_PASS();
}

MINUNIT_TEARDOWN(driver74595Teardown) {
  MINUNIT_PASS();
}

MINUNIT_ADD(driver74595Test, driver74595Setup, driver74595Teardown) {
  MINUNIT_PASS();
}