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
static constexpr libmcu::HwAddressType systickAddress = libmcuhw::SystickAddress;
libmcuhw::systick::systick *const systickDutRegisters{reinterpret_cast<libmcuhw::systick::systick *>(systickAddress)};

volatile std::uint32_t systickIsrCount;

extern "C" {
void SysTick_Handler(void) {
  systick_peripheral.Isr();
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
  systick_peripheral.Init(0x123456);
  minUnitCheck(systickDutRegisters->RVR == 0x123456);
  systick_peripheral.SetReload(0x654321);
  minUnitCheck(systickDutRegisters->RVR == 0x654321);
}

MINUNIT_ADD(CortexM0plusSystickStart, CortexM0plusSetupSystick, cortexm0plus_teardown) {
  systick_peripheral.Init(0x1000);  // short reload value so we can check value
  systick_peripheral.Start();
  std::uint32_t firstCount = systick_peripheral.GetCount();
  libmcu::Delay(101);
  std::uint32_t secondCount = systick_peripheral.GetCount();
  minUnitCheck(firstCount != secondCount);
  // we need longer intervals for this test
  systick_peripheral.SetReload(0xFFFF);
  systick_peripheral.GetZeroPass();
  while (systick_peripheral.GetZeroPass() == 0) {
    libmcull::nop();
  }
  minUnitCheck(systick_peripheral.GetZeroPass() == 0);
  libmcu::Delay(0xFFF);
  minUnitCheck(systick_peripheral.GetZeroPass() != 0);
  systick_peripheral.Stop();
  // interrupt based tests
  systick_peripheral.SetReload(0xFFF);
  systick_peripheral.Start(systickIsrLambda);
  libmcu::Delay(0xFFF);
  systick_peripheral.Stop();
  minUnitCheck(systickIsrCount != 0);
}
