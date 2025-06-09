/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 sio gpio
 */
#include <nuclone_RP2040_LL.hpp>
#include <MinUnit.h>
#include <RP2040_LL_teardown.hpp>
#include <common.hpp>

// peripheral register sets
static constexpr libmcu::HwAddressType dutAddress = libmcuhw::kSioAddress;
libmcuhw::sio::Sio *const dutRegisters{reinterpret_cast<libmcuhw::sio::Sio *>(dutAddress)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupSio) {
  minUnitCheck(RP2040TeardownCorrect() == true);
  resetsPeripheral.Reset(libmcull::resets::kIoBank0 | libmcull::resets::kPadsBank0, 100000);
  // connect all GPIO's
  gpioBank0Peripheral.Setup(gpio0Pin);
  padsBank0Peripheral.Setup(gpio0Pin, libmcull::pads::DriveModes::k8mA, false, false, true, false);
  gpioBank0Peripheral.Setup(gpio1Pin);
  padsBank0Peripheral.Setup(gpio1Pin, libmcull::pads::DriveModes::k8mA, false, false, true, false);
}

MINUNIT_ADD(RP2040gpio, RP2040SetupSio, RP2040Teardown) {
  sioGpioPeripheral.SetInput(gpio1Pin);
  sioGpioPeripheral.SetOutput(gpio0Pin);
  sioGpioPeripheral.SetHigh(gpio0Pin);
  minUnitCheck(sioGpioPeripheral.GetLevel(gpio1Pin) != 0u);
  sioGpioPeripheral.SetLow(gpio0Pin);
  minUnitCheck(sioGpioPeripheral.GetLevel(gpio1Pin) == 0u);
  // switch around pins
  sioGpioPeripheral.SetInput(gpio0Pin);
  sioGpioPeripheral.SetOutput(gpio1Pin);
  sioGpioPeripheral.SetHigh(gpio1Pin);
  minUnitCheck(sioGpioPeripheral.GetLevel(gpio0Pin) != 0u);
  sioGpioPeripheral.SetLow(gpio1Pin);
  minUnitCheck(sioGpioPeripheral.GetLevel(gpio0Pin) == 0u);
  // testing the remainder methods
  sioGpioPeripheral.Toggle(gpio1Pin);
  minUnitCheck(sioGpioPeripheral.GetLevel(gpio0Pin) != 0u);
  sioGpioPeripheral.Toggle(gpio1Pin);
  minUnitCheck(sioGpioPeripheral.GetLevel(gpio0Pin) == 0u);
  sioGpioPeripheral.SetLevel(gpio1Pin, 1);
  minUnitCheck(sioGpioPeripheral.GetLevel(gpio0Pin) != 0u);
  sioGpioPeripheral.SetLevel(gpio1Pin, 0);
  minUnitCheck(sioGpioPeripheral.GetLevel(gpio0Pin) == 0u);
}