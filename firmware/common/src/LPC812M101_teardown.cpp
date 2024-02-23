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
#include <nuclone_LPC812M101DH20_tests.hpp>
#include <LPC812M101_teardown.hpp>
#include <test_conditions.hpp>
#include <MinUnit.h>

// peripheral register sets
static constexpr libMcuLL::hwAddressType sysconAddress = libMcuLL::hw::sysconAddress;
static constexpr libMcuLL::hwAddressType swmAddress = libMcuLL::hw::swmAddress;
static constexpr libMcuLL::hwAddressType fmcAddress = libMcuLL::hw::fmcAddress;
libMcuLL::hw::syscon::peripheral *const sysconRegisters{reinterpret_cast<libMcuLL::hw::syscon::peripheral *>(sysconAddress)};
libMcuLL::hw::swm::peripheral *const swmRegisters{reinterpret_cast<libMcuLL::hw::swm::peripheral *>(swmAddress)};
libMcuLL::hw::fmc::peripheral *const fmcRegisters{reinterpret_cast<libMcuLL::hw::fmc::peripheral *>(fmcAddress)};

/** @brief resets all the registers to their default states
 *
 */
MINUNIT_TEARDOWN(LPC812M101Teardown) {
  sysconRegisters->SYSAHBCLKCTRL = 0x000FFFFF;  // enable all peripherals
  sysconRegisters->PRESETCTRL = 0x00000800;     // reset all peripherals we want to reset
  sysconRegisters->PRESETCTRL = 0x00001FFF;     // unassert resets
  sysconRegisters->PDRUNCFG = 0x0000ED50;       // we deviate here from the datasheet as we use the PLL and crystal oscillator
  for (int i = 0; i < 9; i++) {
    swmRegisters->PINASSIGN[i] = 0xFFFFFFFF;  // clear all pin assignments
  }
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  sysconRegisters->SYSAHBCLKCTRL = 0x000000DF;  // disable all peripherals we dont need
}

/**
 * @brief checks if all the registers to their default states
 * @return if all registers are correctly reset
 *
 * We assume that all peripherals are enabled, this is not tested
 *
 */
bool LPC812M101TeardownCorrect(void) {
  TESTANDRETURN((fmcRegisters->FLASHCFG & libMcuLL::hw::fmc::FLASHCFG::RESERVED_MASK) == 0x00000001);
  TESTANDRETURN(sysconRegisters->PRESETCTRL == 0x00001FFF);
  TESTANDRETURN(sysconRegisters->PDRUNCFG == 0x0000ED50);
  for (int i = 0; i < 9; i++) {
    TESTANDRETURN(swmRegisters->PINASSIGN[i] == 0xFFFFFFFF);
  }
  return true;
}