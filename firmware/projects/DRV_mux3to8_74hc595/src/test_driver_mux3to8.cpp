/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the 3 to 8 multiplexer driver
 */
#include <nuclone_RP2040_DRV_mux_595.hpp>
#include <MinUnit.h>
#include <common.hpp>

using namespace libmcull;
using namespace libmcuhal;
using namespace libMcuDriver::mux;

/**
 * @brief driver setup and initialisation
 */
MINUNIT_SETUP(driverMux3to8Setup) {
  gpio_bank0_hal.Init();
  gpio_bank0_peripheral.Setup(mux_not_enable_pin);
  pads_bank0_peripheral.Setup(mux_not_enable_pin, pads::DriveModes::Current4mA, pads::PullModes::None, false, false);
  gpio_bank0_peripheral.Setup(mux_A0_pin);
  pads_bank0_peripheral.Setup(mux_A0_pin, pads::DriveModes::Current4mA, pads::PullModes::None, false, false);
  pads_bank0_peripheral.Setup(mux_A1_pin, pads::DriveModes::Current4mA, pads::PullModes::None, false, false);
  pads_bank0_peripheral.Setup(mux_A2_pin, pads::DriveModes::Current4mA, pads::PullModes::None, false, false);
  pads_bank0_peripheral.Setup(mux_Y0_pin, pads::DriveModes::Current4mA, pads::PullModes::PullDown, false, true);
  pads_bank0_peripheral.Setup(mux_Y1_pin, pads::DriveModes::Current4mA, pads::PullModes::PullDown, false, true);
  pads_bank0_peripheral.Setup(mux_Y2_pin, pads::DriveModes::Current4mA, pads::PullModes::PullDown, false, true);
  pads_bank0_peripheral.Setup(mux_Y7_pin, pads::DriveModes::Current4mA, pads::PullModes::PullDown, false, true);
  gpio_bank0_hal.SetupInput(mux_Y0_pin);
  gpio_bank0_hal.SetupInput(mux_Y1_pin);
  gpio_bank0_hal.SetupInput(mux_Y2_pin);
  gpio_bank0_hal.SetupInput(mux_Y7_pin);
  test_mux.initialize();
  minUnitPass();
}

MINUNIT_TEARDOWN(driverMux3to8Teardown) {
  minUnitPass();
}

MINUNIT_ADD(driverMux3to8Test, driverMux3to8Setup, driverMux3to8Teardown) {
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y0_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y1_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y2_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y7_pin) != 0);
  test_mux.set(false, 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y0_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y1_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y2_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y7_pin) != 0);
  test_mux.set(true, 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y0_pin) == 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y1_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y2_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y7_pin) != 0);
  test_mux.set(false, 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y0_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y1_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y2_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y7_pin) != 0);
  test_mux.set(true, 1);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y0_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y1_pin) == 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y2_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y7_pin) != 0);
  test_mux.set(true, 2);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y0_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y1_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y2_pin) == 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y7_pin) != 0);
  test_mux.set(true, 7);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y0_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y1_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y2_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y7_pin) == 0);
  test_mux.set(false, 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y0_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y1_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y2_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(mux_Y7_pin) != 0);
}