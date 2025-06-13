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
  pins_hal.Setup(muxNotEnablePin, pins::DriveModes::k4Ma, pins::PullModes::kNone, pins::speedModes::kMedium, false);
  pins_hal.Setup(muxA0Pin, pins::DriveModes::k4Ma, pins::PullModes::kNone, pins::speedModes::kMedium, false);
  pins_hal.Setup(muxA1Pin, pins::DriveModes::k4Ma, pins::PullModes::kNone, pins::speedModes::kMedium, false);
  pins_hal.Setup(muxA2Pin, pins::DriveModes::k4Ma, pins::PullModes::kNone, pins::speedModes::kMedium, false);
  pins_hal.Setup(muxY0Pin, pins::DriveModes::k4Ma, pins::PullModes::kPullDown, pins::speedModes::kMedium, true);
  pins_hal.Setup(muxY1Pin, pins::DriveModes::k4Ma, pins::PullModes::kPullDown, pins::speedModes::kMedium, true);
  pins_hal.Setup(muxY2Pin, pins::DriveModes::k4Ma, pins::PullModes::kPullDown, pins::speedModes::kMedium, true);
  pins_hal.Setup(muxY7Pin, pins::DriveModes::k4Ma, pins::PullModes::kPullDown, pins::speedModes::kMedium, true);
  gpio_hal.SetInput(muxY0Pin);
  gpio_hal.SetInput(muxY1Pin);
  gpio_hal.SetInput(muxY2Pin);
  gpio_hal.SetInput(muxY7Pin);
  test_mux.initialize();
  minUnitPass();
}

MINUNIT_TEARDOWN(driverMux3to8Teardown) {
  pins_hal.Reset(muxNotEnablePin);
  pins_hal.Reset(muxA0Pin);
  pins_hal.Reset(muxA1Pin);
  pins_hal.Reset(muxA2Pin);
  pins_hal.Reset(muxY0Pin);
  pins_hal.Reset(muxY1Pin);
  pins_hal.Reset(muxY2Pin);
  pins_hal.Reset(muxY7Pin);
  minUnitPass();
}

MINUNIT_ADD(driverMux3to8Test, driverMux3to8Setup, driverMux3to8Teardown) {
  minUnitCheck(gpio_hal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY7Pin) != 0);
  test_mux.set(false, 0);
  minUnitCheck(gpio_hal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY7Pin) != 0);
  test_mux.set(true, 0);
  minUnitCheck(gpio_hal.GetPin(muxY0Pin) == 0);
  minUnitCheck(gpio_hal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY7Pin) != 0);
  test_mux.set(false, 0);
  minUnitCheck(gpio_hal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY7Pin) != 0);
  test_mux.set(true, 1);
  minUnitCheck(gpio_hal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY1Pin) == 0);
  minUnitCheck(gpio_hal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY7Pin) != 0);
  test_mux.set(true, 2);
  minUnitCheck(gpio_hal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY2Pin) == 0);
  minUnitCheck(gpio_hal.GetPin(muxY7Pin) != 0);
  test_mux.set(true, 7);
  minUnitCheck(gpio_hal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY7Pin) == 0);
  test_mux.set(false, 0);
  minUnitCheck(gpio_hal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpio_hal.GetPin(muxY7Pin) != 0);
}