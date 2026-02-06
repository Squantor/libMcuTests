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
#include <minunit.h>
#include <CortexM0plus_teardown.hpp>
#include <common.hpp>

// peripheral register sets
static constexpr libmcu::HwAddressType scbAddress = libmcuhw::ScbAddress;
libmcuhw::scb::Scb *const scbDutRegisters{reinterpret_cast<libmcuhw::scb::Scb *>(scbAddress)};

alignas(256) static std::array<std::uint32_t, 48> vectorTable;

/**
 * @brief systick setup and initialisation
 */
MINUNIT_SETUP(CortexM0plusSetupScb) {
  MINUNIT_CHECK(cortexm0plus_teardown_correct() == true);
}

MINUNIT_ADD(CortexM0plusScbVtor, CortexM0plusSetupScb, cortexm0plus_teardown) {
  MINUNIT_CHECK(scb_peripheral.getVtorMask() == libmcuhw::vtor::AddressMask);
  MINUNIT_CHECK((reinterpret_cast<std::uint32_t>(vectorTable.data()) & 0xFF) == 0);
  MINUNIT_CHECK(scbDutRegisters->VTOR == 0x00000000);
  scb_peripheral.setVtor(vectorTable.data());
  MINUNIT_CHECK(scbDutRegisters->VTOR == reinterpret_cast<std::uint32_t>(vectorTable.data()));
  scbDutRegisters->VTOR = 0;
  MINUNIT_CHECK(scbDutRegisters->VTOR == 0x00000000);
}

MINUNIT_ADD(CortexM0plusScbSleep, CortexM0plusSetupScb, cortexm0plus_teardown) {
  MINUNIT_CHECK(scbDutRegisters->SCR == 0x00000000UL);
  scb_peripheral.setSleepBehaviour(true, false, true);
  MINUNIT_CHECK(scbDutRegisters->SCR == 0x12);
  scb_peripheral.setSleepBehaviour(false, false, false);
  MINUNIT_CHECK(scbDutRegisters->SCR == 0x00);
}
