/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the CortexM0plus systick peripheral
 */
#include <nuclone_CortexM0plus_tests.hpp>
#include <MinUnit.h>
#include <CortexM0plus_teardown.hpp>
#include <common.hpp>

using namespace libMcuLL::hw::scb;
using namespace libMcuLL::sw::scb;

// peripheral register sets
static constexpr libMcuLL::hwAddressType scbAddress = libMcuLL::hw::scbAddress;
libMcuLL::hw::scb::peripheral *const scbDutRegisters{reinterpret_cast<libMcuLL::hw::scb::peripheral *>(scbAddress)};

alignas(256) static std::array<std::uint32_t, 48> vectorTable;

/**
 * @brief systick setup and initialisation
 */
MINUNIT_SETUP(CortexM0plusSetupScb) {
  minUnitCheck(CortexM0plusTeardownCorrect() == true);
}

MINUNIT_ADD(CortexM0plusScbVtor, CortexM0plusSetupScb, CortexM0plusTeardown) {
  minUnitCheck(scbPeripheral.getVtorMask() == libMcuLL::hw::vtor::addressMask);
  minUnitCheck((reinterpret_cast<std::uint32_t>(vectorTable.data()) & 0xFF) == 0);
  minUnitCheck(scbDutRegisters->VTOR == 0x00000000);
  scbPeripheral.setVtor(vectorTable.data());
  minUnitCheck(scbDutRegisters->VTOR == reinterpret_cast<std::uint32_t>(vectorTable.data()));
  scbDutRegisters->VTOR = 0;
  minUnitCheck(scbDutRegisters->VTOR == 0x00000000);
}

MINUNIT_ADD(CortexM0plusScbSleep, CortexM0plusSetupScb, CortexM0plusTeardown) {
  minUnitCheck(scbDutRegisters->SCR == 0x00000000UL);
  scbPeripheral.setSleepBehaviour(true, false, true);
  minUnitCheck(scbDutRegisters->SCR == 0x12);
  scbPeripheral.setSleepBehaviour(false, false, false);
  minUnitCheck(scbDutRegisters->SCR == 0x00);
}
