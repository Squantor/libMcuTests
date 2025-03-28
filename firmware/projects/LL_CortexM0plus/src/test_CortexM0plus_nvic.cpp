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
static constexpr libmcu::hwAddressType nvicAddress = libmcuhw::nvicAddress;
libmcuhw::nvic::nvic *const nvicDutRegisters{reinterpret_cast<libmcuhw::nvic::nvic *>(nvicAddress)};
static constexpr libmcu::hwAddressType scbAddress = libmcuhw::scbAddress;
libmcuhw::scb::scb *const scbDutRegisters{reinterpret_cast<libmcuhw::scb::scb *>(scbAddress)};

/**
 * @brief systick setup and initialisation
 */
MINUNIT_SETUP(CortexM0plusSetupNvic) {
  minUnitCheck(CortexM0plusTeardownCorrect() == true);
}

MINUNIT_ADD(CortexM0plusNvicSetClear, CortexM0plusSetupNvic, CortexM0plusTeardown) {
  nvicPeripheral.enable(libmcuhw::interrupts::dummy0);
  minUnitCheck(nvicDutRegisters->ICER[0] == 0x1);
  nvicPeripheral.enable(libmcuhw::interrupts::dummy12);
  minUnitCheck(nvicDutRegisters->ICER[0] == 0x1001);
  nvicPeripheral.disable(libmcuhw::interrupts::dummy0);
  minUnitCheck(nvicDutRegisters->ICER[0] == 0x1000);
  nvicPeripheral.disable(libmcuhw::interrupts::dummy12);
  minUnitCheck(nvicDutRegisters->ICER[0] == 0x0);
}

MINUNIT_ADD(CortexM0plusNvicPending, CortexM0plusSetupNvic, CortexM0plusTeardown) {
  minUnitCheck(nvicPeripheral.getPending(libmcuhw::interrupts::dummy0) == false);
  nvicPeripheral.setPending(libmcuhw::interrupts::dummy0);
  minUnitCheck(nvicPeripheral.getPending(libmcuhw::interrupts::dummy0) == true);
  minUnitCheck(nvicPeripheral.getPending(libmcuhw::interrupts::dummy12) == false);
  minUnitCheck(nvicDutRegisters->ICPR[0] == 0x00000001UL);
  nvicPeripheral.setPending(libmcuhw::interrupts::dummy12);
  minUnitCheck(nvicPeripheral.getPending(libmcuhw::interrupts::dummy0) == true);
  minUnitCheck(nvicPeripheral.getPending(libmcuhw::interrupts::dummy12) == true);
  minUnitCheck(nvicDutRegisters->ICPR[0] == 0x00001001UL);
  nvicPeripheral.clearPending(libmcuhw::interrupts::dummy12);
  minUnitCheck(nvicPeripheral.getPending(libmcuhw::interrupts::dummy0) == true);
  minUnitCheck(nvicPeripheral.getPending(libmcuhw::interrupts::dummy12) == false);
  minUnitCheck(nvicDutRegisters->ICPR[0] == 0x00000001UL);
  nvicPeripheral.clearPending(libmcuhw::interrupts::dummy0);
  minUnitCheck(nvicPeripheral.getPending(libmcuhw::interrupts::dummy0) == false);
  minUnitCheck(nvicPeripheral.getPending(libmcuhw::interrupts::dummy12) == false);
  minUnitCheck(nvicDutRegisters->ICPR[0] == 0x00000000UL);
}

MINUNIT_ADD(CortexM0plusNvicPriority, CortexM0plusSetupNvic, CortexM0plusTeardown) {
  minUnitCheck(nvicDutRegisters->IP[0] == 0x00000000);
  nvicPeripheral.setPriority(libmcuhw::interrupts::dummy0, 2);
  minUnitCheck(nvicDutRegisters->IP[0] == 0x00000080);
  nvicPeripheral.setPriority(libmcuhw::interrupts::dummy1, 3);

  minUnitCheck(nvicDutRegisters->IP[0] == 0x0000C080);
  nvicPeripheral.setPriority(libmcuhw::interrupts::dummy0, 0);
  minUnitCheck(nvicDutRegisters->IP[0] == 0x0000C000);
  nvicPeripheral.setPriority(libmcuhw::interrupts::dummy12, 1);
  minUnitCheck(nvicDutRegisters->IP[3] == 0x00000040);
  nvicPeripheral.setPriority(libmcuhw::interrupts::dummy12, 0);
  minUnitCheck(nvicDutRegisters->IP[3] == 0x00000000);
  nvicPeripheral.setPriority(libmcuhw::interrupts::dummy1, 0);
  for (int i = 0; i < 8; ++i) {
    minUnitCheck(nvicDutRegisters->IP[i] == 0x00000000);
  }
  minUnitCheck(scbDutRegisters->SHP[1] == 0x00000000);
  nvicPeripheral.setPriority(libmcuhw::interrupts::systick, 2);
  minUnitCheck(scbDutRegisters->SHP[1] == 0x80000000);
  nvicPeripheral.setPriority(libmcuhw::interrupts::pendSv, 3);
  minUnitCheck(scbDutRegisters->SHP[1] == 0x80C00000);
  nvicPeripheral.setPriority(libmcuhw::interrupts::pendSv, 0);
  minUnitCheck(scbDutRegisters->SHP[1] == 0x80000000);
  nvicPeripheral.setPriority(libmcuhw::interrupts::systick, 0);
  minUnitCheck(scbDutRegisters->SHP[1] == 0x00000000);
  minUnitCheck(scbDutRegisters->SHP[0] == 0x00000000);
  nvicPeripheral.setPriority(libmcuhw::interrupts::svCall, 3);
  minUnitCheck(scbDutRegisters->SHP[0] == 0xC0000000);
  nvicPeripheral.setPriority(libmcuhw::interrupts::svCall, 0);
  minUnitCheck(scbDutRegisters->SHP[0] == 0x00000000);
}