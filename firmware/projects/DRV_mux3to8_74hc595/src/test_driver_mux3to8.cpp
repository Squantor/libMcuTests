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
  pinsHal.Setup(muxNotEnablePin, pins::DriveModes::k4Ma, pins::PullModes::kNone, pins::speedModes::kMedium, false);
  pinsHal.Setup(muxA0Pin, pins::DriveModes::k4Ma, pins::PullModes::kNone, pins::speedModes::kMedium, false);
  pinsHal.Setup(muxA1Pin, pins::DriveModes::k4Ma, pins::PullModes::kNone, pins::speedModes::kMedium, false);
  pinsHal.Setup(muxA2Pin, pins::DriveModes::k4Ma, pins::PullModes::kNone, pins::speedModes::kMedium, false);
  pinsHal.Setup(muxY0Pin, pins::DriveModes::k4Ma, pins::PullModes::kPullDown, pins::speedModes::kMedium, true);
  pinsHal.Setup(muxY1Pin, pins::DriveModes::k4Ma, pins::PullModes::kPullDown, pins::speedModes::kMedium, true);
  pinsHal.Setup(muxY2Pin, pins::DriveModes::k4Ma, pins::PullModes::kPullDown, pins::speedModes::kMedium, true);
  pinsHal.Setup(muxY7Pin, pins::DriveModes::k4Ma, pins::PullModes::kPullDown, pins::speedModes::kMedium, true);
  gpioHal.SetInput(muxY0Pin);
  gpioHal.SetInput(muxY1Pin);
  gpioHal.SetInput(muxY2Pin);
  gpioHal.SetInput(muxY7Pin);
  testMux.initialize();
  minUnitPass();
}

MINUNIT_TEARDOWN(driverMux3to8Teardown) {
  pinsHal.Reset(muxNotEnablePin);
  pinsHal.Reset(muxA0Pin);
  pinsHal.Reset(muxA1Pin);
  pinsHal.Reset(muxA2Pin);
  pinsHal.Reset(muxY0Pin);
  pinsHal.Reset(muxY1Pin);
  pinsHal.Reset(muxY2Pin);
  pinsHal.Reset(muxY7Pin);
  minUnitPass();
}

MINUNIT_ADD(driverMux3to8Test, driverMux3to8Setup, driverMux3to8Teardown) {
  minUnitCheck(gpioHal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY7Pin) != 0);
  testMux.set(false, 0);
  minUnitCheck(gpioHal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY7Pin) != 0);
  testMux.set(true, 0);
  minUnitCheck(gpioHal.GetPin(muxY0Pin) == 0);
  minUnitCheck(gpioHal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY7Pin) != 0);
  testMux.set(false, 0);
  minUnitCheck(gpioHal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY7Pin) != 0);
  testMux.set(true, 1);
  minUnitCheck(gpioHal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY1Pin) == 0);
  minUnitCheck(gpioHal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY7Pin) != 0);
  testMux.set(true, 2);
  minUnitCheck(gpioHal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY2Pin) == 0);
  minUnitCheck(gpioHal.GetPin(muxY7Pin) != 0);
  testMux.set(true, 7);
  minUnitCheck(gpioHal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY7Pin) == 0);
  testMux.set(false, 0);
  minUnitCheck(gpioHal.GetPin(muxY0Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY1Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY2Pin) != 0);
  minUnitCheck(gpioHal.GetPin(muxY7Pin) != 0);
}