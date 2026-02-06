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
#include <minunit.h>
#include <CortexM0plus_teardown.hpp>
#include <common.hpp>

// peripheral register sets
static constexpr libmcu::HwAddressType nvicAddress = libmcuhw::NvicAddress;
libmcuhw::nvic::Nvic *const nvicDutRegisters{reinterpret_cast<libmcuhw::nvic::Nvic *>(nvicAddress)};
static constexpr libmcu::HwAddressType scbAddress = libmcuhw::ScbAddress;
libmcuhw::scb::Scb *const scbDutRegisters{reinterpret_cast<libmcuhw::scb::Scb *>(scbAddress)};

/**
 * @brief kSystick setup and initialisation
 */
MINUNIT_SETUP(CortexM0plusSetupNvic) {
  MINUNIT_CHECK(cortexm0plus_teardown_correct() == true);
}

MINUNIT_ADD(CortexM0plusNvicSetClear, CortexM0plusSetupNvic, cortexm0plus_teardown) {
  nvic_peripheral.Enable(libmcuhw::Interrupts::Dummy0);
  MINUNIT_CHECK(nvicDutRegisters->ICER[0] == 0x1);
  nvic_peripheral.Enable(libmcuhw::Interrupts::Dummy12);
  MINUNIT_CHECK(nvicDutRegisters->ICER[0] == 0x1001);
  nvic_peripheral.Disable(libmcuhw::Interrupts::Dummy0);
  MINUNIT_CHECK(nvicDutRegisters->ICER[0] == 0x1000);
  nvic_peripheral.Disable(libmcuhw::Interrupts::Dummy12);
  MINUNIT_CHECK(nvicDutRegisters->ICER[0] == 0x0);
}

MINUNIT_ADD(CortexM0plusNvicPending, CortexM0plusSetupNvic, cortexm0plus_teardown) {
  MINUNIT_CHECK(nvic_peripheral.GetPending(libmcuhw::Interrupts::Dummy0) == false);
  nvic_peripheral.SetPending(libmcuhw::Interrupts::Dummy0);
  MINUNIT_CHECK(nvic_peripheral.GetPending(libmcuhw::Interrupts::Dummy0) == true);
  MINUNIT_CHECK(nvic_peripheral.GetPending(libmcuhw::Interrupts::Dummy12) == false);
  MINUNIT_CHECK(nvicDutRegisters->ICPR[0] == 0x00000001UL);
  nvic_peripheral.SetPending(libmcuhw::Interrupts::Dummy12);
  MINUNIT_CHECK(nvic_peripheral.GetPending(libmcuhw::Interrupts::Dummy0) == true);
  MINUNIT_CHECK(nvic_peripheral.GetPending(libmcuhw::Interrupts::Dummy12) == true);
  MINUNIT_CHECK(nvicDutRegisters->ICPR[0] == 0x00001001UL);
  nvic_peripheral.ClearPending(libmcuhw::Interrupts::Dummy12);
  MINUNIT_CHECK(nvic_peripheral.GetPending(libmcuhw::Interrupts::Dummy0) == true);
  MINUNIT_CHECK(nvic_peripheral.GetPending(libmcuhw::Interrupts::Dummy12) == false);
  MINUNIT_CHECK(nvicDutRegisters->ICPR[0] == 0x00000001UL);
  nvic_peripheral.ClearPending(libmcuhw::Interrupts::Dummy0);
  MINUNIT_CHECK(nvic_peripheral.GetPending(libmcuhw::Interrupts::Dummy0) == false);
  MINUNIT_CHECK(nvic_peripheral.GetPending(libmcuhw::Interrupts::Dummy12) == false);
  MINUNIT_CHECK(nvicDutRegisters->ICPR[0] == 0x00000000UL);
}

MINUNIT_ADD(CortexM0plusNvicPriority, CortexM0plusSetupNvic, cortexm0plus_teardown) {
  MINUNIT_CHECK(nvicDutRegisters->IP[0] == 0x00000000);
  nvic_peripheral.SetPriority(libmcuhw::Interrupts::Dummy0, 2);
  MINUNIT_CHECK(nvicDutRegisters->IP[0] == 0x00000080);
  nvic_peripheral.SetPriority(libmcuhw::Interrupts::Dummy1, 3);

  MINUNIT_CHECK(nvicDutRegisters->IP[0] == 0x0000C080);
  nvic_peripheral.SetPriority(libmcuhw::Interrupts::Dummy0, 0);
  MINUNIT_CHECK(nvicDutRegisters->IP[0] == 0x0000C000);
  nvic_peripheral.SetPriority(libmcuhw::Interrupts::Dummy12, 1);
  MINUNIT_CHECK(nvicDutRegisters->IP[3] == 0x00000040);
  nvic_peripheral.SetPriority(libmcuhw::Interrupts::Dummy12, 0);
  MINUNIT_CHECK(nvicDutRegisters->IP[3] == 0x00000000);
  nvic_peripheral.SetPriority(libmcuhw::Interrupts::Dummy1, 0);
  for (int i = 0; i < 8; ++i) {
    MINUNIT_CHECK(nvicDutRegisters->IP[i] == 0x00000000);
  }
  MINUNIT_CHECK(scbDutRegisters->SHP[1] == 0x00000000);
  nvic_peripheral.SetPriority(libmcuhw::Interrupts::Systick, 2);
  MINUNIT_CHECK(scbDutRegisters->SHP[1] == 0x80000000);
  nvic_peripheral.SetPriority(libmcuhw::Interrupts::PendSv, 3);
  MINUNIT_CHECK(scbDutRegisters->SHP[1] == 0x80C00000);
  nvic_peripheral.SetPriority(libmcuhw::Interrupts::PendSv, 0);
  MINUNIT_CHECK(scbDutRegisters->SHP[1] == 0x80000000);
  nvic_peripheral.SetPriority(libmcuhw::Interrupts::Systick, 0);
  MINUNIT_CHECK(scbDutRegisters->SHP[1] == 0x00000000);
  MINUNIT_CHECK(scbDutRegisters->SHP[0] == 0x00000000);
  nvic_peripheral.SetPriority(libmcuhw::Interrupts::SvCall, 3);
  MINUNIT_CHECK(scbDutRegisters->SHP[0] == 0xC0000000);
  nvic_peripheral.SetPriority(libmcuhw::Interrupts::SvCall, 0);
  MINUNIT_CHECK(scbDutRegisters->SHP[0] == 0x00000000);
}