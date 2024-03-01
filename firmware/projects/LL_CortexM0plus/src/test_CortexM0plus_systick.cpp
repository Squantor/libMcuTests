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

using namespace libMcuLL::hw::systick;
using namespace libMcuLL::sw::systick;

auto systickIsrLambda = []() { systickIsrCount = systickIsrCount + 1; };

// peripheral register sets
static constexpr libMcuLL::hwAddressType systickAddress = libMcuLL::hw::systickAddress;
libMcuLL::hw::systick::peripheral *const systickDutRegisters{reinterpret_cast<libMcuLL::hw::systick::peripheral *>(systickAddress)};

volatile std::uint32_t systickIsrCount;

extern "C" {
void SysTick_Handler(void) {
  systickPeripheral.isr();
}
}

/**
 * @brief systick setup and initialisation
 */
MINUNIT_SETUP(CortexM0plusSetupSystick) {
  systickIsrCount = 0;
  minUnitCheck(CortexM0plusTeardownCorrect() == true);
}

MINUNIT_ADD(CortexM0plusSystickInit, CortexM0plusSetupSystick, CortexM0plusTeardown) {
  systickPeripheral.init(0x123456);
  minUnitCheck(systickDutRegisters->RVR == 0x123456);
  systickPeripheral.setReload(0x654321);
  minUnitCheck(systickDutRegisters->RVR == 0x654321);
}

MINUNIT_ADD(CortexM0plusSystickStart, CortexM0plusSetupSystick, CortexM0plusTeardown) {
  systickPeripheral.init(0x1000);  // short reload value so we can check value
  systickPeripheral.start();
  std::uint32_t firstCount = systickPeripheral.getCount();
  crudeDelay(101);
  std::uint32_t secondCount = systickPeripheral.getCount();
  minUnitCheck(firstCount != secondCount);
  // we need longer intervals for this test
  systickPeripheral.setReload(0xFFFF);
  systickPeripheral.getZeroPass();
  while (systickPeripheral.getZeroPass() == 0) {
    libMcuLL::sw::nop();
  }
  minUnitCheck(systickPeripheral.getZeroPass() == 0);
  crudeDelay(0xFFF);
  minUnitCheck(systickPeripheral.getZeroPass() != 0);
  systickPeripheral.stop();
  // interrupt based tests
  systickPeripheral.setReload(0xFFF);
  systickPeripheral.start(systickIsrLambda);
  crudeDelay(0xFFF);
  systickPeripheral.stop();
  minUnitCheck(systickIsrCount != 0);
}
