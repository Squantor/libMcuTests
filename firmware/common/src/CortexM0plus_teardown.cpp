/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief Teardown function that resets the LPC812M101 to a state
 * that more or less matches reset state
 */
#include <nuclone_CortexM0plus_tests.hpp>
#include <CortexM0plus_teardown.hpp>
#include <test_conditions.hpp>
#include <MinUnit.h>

// peripheral register sets
static constexpr libmcu::hwAddressType systickAddress = libmcuhw::systickAddress;
libmcuhw::systick::systick *const systickRegisters{reinterpret_cast<libmcuhw::systick::systick *>(systickAddress)};
static constexpr libmcu::hwAddressType nvicAddress = libmcuhw::nvicAddress;
libmcuhw::nvic::nvic *const nvicRegisters{reinterpret_cast<libmcuhw::nvic::nvic *>(nvicAddress)};

MINUNIT_TEARDOWN(CortexM0plusTeardown) {
  systickRegisters->CSR = 0UL;
  systickRegisters->RVR = 0UL;
  systickRegisters->CVR = 0UL;
  nvicRegisters->ICER[0] = 0xFFFFFFFFUL;
  nvicRegisters->ICPR[0] = 0xFFFFFFFFUL;
  minUnitPass();
}

bool CortexM0plusTeardownCorrect(void) {
  TESTANDRETURN(systickRegisters->CSR == 0UL);
  TESTANDRETURN(systickRegisters->RVR == 0UL);
  TESTANDRETURN(systickRegisters->CVR == 0UL);
  TESTANDRETURN(nvicRegisters->ISER[0] == 0UL);
  TESTANDRETURN(nvicRegisters->ISPR[0] == 0UL);
  return true;
}