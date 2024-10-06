/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the CortexM0plus NVIC peripheral
 */
#include <nuclone_CortexM0plus_tests.hpp>
#include <MinUnit.h>
#include <CortexM0plus_teardown.hpp>
#include <common.hpp>

// peripheral register sets
static constexpr libMcu::hwAddressType nvicAddress = libMcuHw::nvicAddress;
libMcuHw::nvic::nvic *const nvicDutRegisters{reinterpret_cast<libMcuHw::nvic::nvic *>(nvicAddress)};
static constexpr libMcu::hwAddressType scbAddress = libMcuHw::scbAddress;
libMcuHw::scb::scb *const scbDutRegisters{reinterpret_cast<libMcuHw::scb::scb *>(scbAddress)};

/**
 * @brief systick setup and initialisation
 */
MINUNIT_SETUP(CortexM0plusSetupNvic) {
  minUnitCheck(CortexM0plusTeardownCorrect() == true);
}

MINUNIT_ADD(CortexM0plusNvicSetClear, CortexM0plusSetupNvic, CortexM0plusTeardown) {
  nvicPeripheral.enable(libMcuHw::interrupts::dummy0);
  minUnitCheck(nvicDutRegisters->ICER[0] == 0x1);
  nvicPeripheral.enable(libMcuHw::interrupts::dummy12);
  minUnitCheck(nvicDutRegisters->ICER[0] == 0x1001);
  nvicPeripheral.disable(libMcuHw::interrupts::dummy0);
  minUnitCheck(nvicDutRegisters->ICER[0] == 0x1000);
  nvicPeripheral.disable(libMcuHw::interrupts::dummy12);
  minUnitCheck(nvicDutRegisters->ICER[0] == 0x0);
}

MINUNIT_ADD(CortexM0plusNvicPending, CortexM0plusSetupNvic, CortexM0plusTeardown) {
  minUnitCheck(nvicPeripheral.getPending(libMcuHw::interrupts::dummy0) == false);
  nvicPeripheral.setPending(libMcuHw::interrupts::dummy0);
  minUnitCheck(nvicPeripheral.getPending(libMcuHw::interrupts::dummy0) == true);
  minUnitCheck(nvicPeripheral.getPending(libMcuHw::interrupts::dummy12) == false);
  minUnitCheck(nvicDutRegisters->ICPR[0] == 0x00000001UL);
  nvicPeripheral.setPending(libMcuHw::interrupts::dummy12);
  minUnitCheck(nvicPeripheral.getPending(libMcuHw::interrupts::dummy0) == true);
  minUnitCheck(nvicPeripheral.getPending(libMcuHw::interrupts::dummy12) == true);
  minUnitCheck(nvicDutRegisters->ICPR[0] == 0x00001001UL);
  nvicPeripheral.clearPending(libMcuHw::interrupts::dummy12);
  minUnitCheck(nvicPeripheral.getPending(libMcuHw::interrupts::dummy0) == true);
  minUnitCheck(nvicPeripheral.getPending(libMcuHw::interrupts::dummy12) == false);
  minUnitCheck(nvicDutRegisters->ICPR[0] == 0x00000001UL);
  nvicPeripheral.clearPending(libMcuHw::interrupts::dummy0);
  minUnitCheck(nvicPeripheral.getPending(libMcuHw::interrupts::dummy0) == false);
  minUnitCheck(nvicPeripheral.getPending(libMcuHw::interrupts::dummy12) == false);
  minUnitCheck(nvicDutRegisters->ICPR[0] == 0x00000000UL);
}

MINUNIT_ADD(CortexM0plusNvicPriority, CortexM0plusSetupNvic, CortexM0plusTeardown) {
  minUnitCheck(nvicDutRegisters->IP[0] == 0x00000000);
  nvicPeripheral.setPriority(libMcuHw::interrupts::dummy0, 2);
  minUnitCheck(nvicDutRegisters->IP[0] == 0x00000080);
  nvicPeripheral.setPriority(libMcuHw::interrupts::dummy1, 3);

  minUnitCheck(nvicDutRegisters->IP[0] == 0x0000C080);
  nvicPeripheral.setPriority(libMcuHw::interrupts::dummy0, 0);
  minUnitCheck(nvicDutRegisters->IP[0] == 0x0000C000);
  nvicPeripheral.setPriority(libMcuHw::interrupts::dummy12, 1);
  minUnitCheck(nvicDutRegisters->IP[3] == 0x00000040);
  nvicPeripheral.setPriority(libMcuHw::interrupts::dummy12, 0);
  minUnitCheck(nvicDutRegisters->IP[3] == 0x00000000);
  nvicPeripheral.setPriority(libMcuHw::interrupts::dummy1, 0);
  for (int i = 0; i < 8; ++i) {
    minUnitCheck(nvicDutRegisters->IP[i] == 0x00000000);
  }
  minUnitCheck(scbDutRegisters->SHP[1] == 0x00000000);
  nvicPeripheral.setPriority(libMcuHw::interrupts::systick, 2);
  minUnitCheck(scbDutRegisters->SHP[1] == 0x80000000);
  nvicPeripheral.setPriority(libMcuHw::interrupts::pendSv, 3);
  minUnitCheck(scbDutRegisters->SHP[1] == 0x80C00000);
  nvicPeripheral.setPriority(libMcuHw::interrupts::pendSv, 0);
  minUnitCheck(scbDutRegisters->SHP[1] == 0x80000000);
  nvicPeripheral.setPriority(libMcuHw::interrupts::systick, 0);
  minUnitCheck(scbDutRegisters->SHP[1] == 0x00000000);
  minUnitCheck(scbDutRegisters->SHP[0] == 0x00000000);
  nvicPeripheral.setPriority(libMcuHw::interrupts::svCall, 3);
  minUnitCheck(scbDutRegisters->SHP[0] == 0xC0000000);
  nvicPeripheral.setPriority(libMcuHw::interrupts::svCall, 0);
  minUnitCheck(scbDutRegisters->SHP[0] == 0x00000000);
}