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

using namespace libMcuLL;
using namespace libmcuhal;
using namespace libMcuDriver::mux;

/**
 * @brief driver setup and initialisation
 */
MINUNIT_SETUP(driverMux3to8Setup) {
  pinsHal.setup(muxNotEnablePin, pins::driveModes::DRIVE_4MA, pins::pullModes::NONE, pins::speedModes::SLEW_MEDIUM, false);
  pinsHal.setup(muxA0Pin, pins::driveModes::DRIVE_4MA, pins::pullModes::NONE, pins::speedModes::SLEW_MEDIUM, false);
  pinsHal.setup(muxA1Pin, pins::driveModes::DRIVE_4MA, pins::pullModes::NONE, pins::speedModes::SLEW_MEDIUM, false);
  pinsHal.setup(muxA2Pin, pins::driveModes::DRIVE_4MA, pins::pullModes::NONE, pins::speedModes::SLEW_MEDIUM, false);
  pinsHal.setup(muxY0Pin, pins::driveModes::DRIVE_4MA, pins::pullModes::PULLDOWN, pins::speedModes::SLEW_MEDIUM, true);
  pinsHal.setup(muxY1Pin, pins::driveModes::DRIVE_4MA, pins::pullModes::PULLDOWN, pins::speedModes::SLEW_MEDIUM, true);
  pinsHal.setup(muxY2Pin, pins::driveModes::DRIVE_4MA, pins::pullModes::PULLDOWN, pins::speedModes::SLEW_MEDIUM, true);
  pinsHal.setup(muxY7Pin, pins::driveModes::DRIVE_4MA, pins::pullModes::PULLDOWN, pins::speedModes::SLEW_MEDIUM, true);
  gpioHal.input(muxY0Pin);
  gpioHal.input(muxY1Pin);
  gpioHal.input(muxY2Pin);
  gpioHal.input(muxY7Pin);
  testMux.initialize();
  minUnitPass();
}

MINUNIT_TEARDOWN(driverMux3to8Teardown) {
  pinsHal.reset(muxNotEnablePin);
  pinsHal.reset(muxA0Pin);
  pinsHal.reset(muxA1Pin);
  pinsHal.reset(muxA2Pin);
  pinsHal.reset(muxY0Pin);
  pinsHal.reset(muxY1Pin);
  pinsHal.reset(muxY2Pin);
  pinsHal.reset(muxY7Pin);
  minUnitPass();
}

MINUNIT_ADD(driverMux3to8Test, driverMux3to8Setup, driverMux3to8Teardown) {
  minUnitCheck(gpioHal.get(muxY0Pin) != 0);
  minUnitCheck(gpioHal.get(muxY1Pin) != 0);
  minUnitCheck(gpioHal.get(muxY2Pin) != 0);
  minUnitCheck(gpioHal.get(muxY7Pin) != 0);
  testMux.set(false, 0);
  minUnitCheck(gpioHal.get(muxY0Pin) != 0);
  minUnitCheck(gpioHal.get(muxY1Pin) != 0);
  minUnitCheck(gpioHal.get(muxY2Pin) != 0);
  minUnitCheck(gpioHal.get(muxY7Pin) != 0);
  testMux.set(true, 0);
  minUnitCheck(gpioHal.get(muxY0Pin) == 0);
  minUnitCheck(gpioHal.get(muxY1Pin) != 0);
  minUnitCheck(gpioHal.get(muxY2Pin) != 0);
  minUnitCheck(gpioHal.get(muxY7Pin) != 0);
  testMux.set(false, 0);
  minUnitCheck(gpioHal.get(muxY0Pin) != 0);
  minUnitCheck(gpioHal.get(muxY1Pin) != 0);
  minUnitCheck(gpioHal.get(muxY2Pin) != 0);
  minUnitCheck(gpioHal.get(muxY7Pin) != 0);
  testMux.set(true, 1);
  minUnitCheck(gpioHal.get(muxY0Pin) != 0);
  minUnitCheck(gpioHal.get(muxY1Pin) == 0);
  minUnitCheck(gpioHal.get(muxY2Pin) != 0);
  minUnitCheck(gpioHal.get(muxY7Pin) != 0);
  testMux.set(true, 2);
  minUnitCheck(gpioHal.get(muxY0Pin) != 0);
  minUnitCheck(gpioHal.get(muxY1Pin) != 0);
  minUnitCheck(gpioHal.get(muxY2Pin) == 0);
  minUnitCheck(gpioHal.get(muxY7Pin) != 0);
  testMux.set(true, 7);
  minUnitCheck(gpioHal.get(muxY0Pin) != 0);
  minUnitCheck(gpioHal.get(muxY1Pin) != 0);
  minUnitCheck(gpioHal.get(muxY2Pin) != 0);
  minUnitCheck(gpioHal.get(muxY7Pin) == 0);
  testMux.set(false, 0);
  minUnitCheck(gpioHal.get(muxY0Pin) != 0);
  minUnitCheck(gpioHal.get(muxY1Pin) != 0);
  minUnitCheck(gpioHal.get(muxY2Pin) != 0);
  minUnitCheck(gpioHal.get(muxY7Pin) != 0);
}