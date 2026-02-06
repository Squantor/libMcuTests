/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief Teardown function that resets the LPC845 to a state
 * that more or less matches reset state
 */
#include <nxp/libmcu_LPC845M301BD48_ll.hpp>
#include <LPC845M301_teardown.hpp>
#include <test_conditions.hpp>
#include <minunit.h>

// peripheral registers
static constexpr libmcu::HwAddressType nvic_address = libmcuhw::NvicAddress; /**< peripheral address */
libmcuhw::nvic::Nvic *nvic_registers{reinterpret_cast<libmcuhw::nvic::Nvic *>(nvic_address)};

/** @brief resets all the registers to their default states
 */
MINUNIT_TEARDOWN(LPC845M301Teardown) {
  nvic_registers->ICER[0] = 0xFFFFFFFF;
  nvic_registers->ICPR[0] = 0xFFFFFFFF;
  MINUNIT_PASS();
}

/** @brief checks if all the registers to their default states
 *  @return if all registers are correctly torndown
 */
bool Lpc845M301TeardownCorrect(void) {
  TESTANDRETURN(nvic_registers->ISER[0] == 0x0);
  TESTANDRETURN(nvic_registers->ISPR[0] == 0x0);
  return true;
}