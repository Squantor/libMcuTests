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

// peripheral register sets
static constexpr libmcu::HwAddressType systickAddress = libmcuhw::kSystickAddress;
libmcuhw::systick::systick *const systickDutRegisters{reinterpret_cast<libmcuhw::systick::systick *>(systickAddress)};

volatile std::uint32_t systickIsrCount;

extern "C" {
void SysTick_Handler(void) {
  systick_peripheral.isr();
}
}

auto systickIsrLambda = []() {
  systickIsrCount = systickIsrCount + 1;
};

/**
 * @brief systick setup and initialisation
 */
MINUNIT_SETUP(CortexM0plusSetupSystick) {
  systickIsrCount = 0;
  minUnitCheck(cortexm0plus_teardown_correct() == true);
}

MINUNIT_ADD(CortexM0plusSystickInit, CortexM0plusSetupSystick, cortexm0plus_teardown) {
  systick_peripheral.init(0x123456);
  minUnitCheck(systickDutRegisters->RVR == 0x123456);
  systick_peripheral.setReload(0x654321);
  minUnitCheck(systickDutRegisters->RVR == 0x654321);
}

MINUNIT_ADD(CortexM0plusSystickStart, CortexM0plusSetupSystick, cortexm0plus_teardown) {
  systick_peripheral.init(0x1000);  // short reload value so we can check value
  systick_peripheral.start();
  std::uint32_t firstCount = systick_peripheral.getCount();
  libmcu::Delay(101);
  std::uint32_t secondCount = systick_peripheral.getCount();
  minUnitCheck(firstCount != secondCount);
  // we need longer intervals for this test
  systick_peripheral.setReload(0xFFFF);
  systick_peripheral.getZeroPass();
  while (systick_peripheral.getZeroPass() == 0) {
    libmcull::nop();
  }
  minUnitCheck(systick_peripheral.getZeroPass() == 0);
  libmcu::Delay(0xFFF);
  minUnitCheck(systick_peripheral.getZeroPass() != 0);
  systick_peripheral.stop();
  // interrupt based tests
  systick_peripheral.setReload(0xFFF);
  systick_peripheral.start(systickIsrLambda);
  libmcu::Delay(0xFFF);
  systick_peripheral.stop();
  minUnitCheck(systickIsrCount != 0);
}
