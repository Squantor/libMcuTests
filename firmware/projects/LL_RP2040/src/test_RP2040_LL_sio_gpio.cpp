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
  minUnitCheck(Rp2040Teardown_correct() == true);
  resets_peripheral.Reset(libmcull::resets::kIoBank0 | libmcull::resets::kPadsBank0, 100000);
  // connect all GPIO's
  gpio_bank0_peripheral.Setup(gpio0Pin);
  pads_bank0_peripheral.Setup(gpio0Pin, libmcull::pads::DriveModes::k8mA, libmcull::pads::PullModes::kNone, true, false);
  gpio_bank0_peripheral.Setup(gpio1Pin);
  pads_bank0_peripheral.Setup(gpio1Pin, libmcull::pads::DriveModes::k8mA, libmcull::pads::PullModes::kNone, true, false);
}

MINUNIT_ADD(RP2040gpio, RP2040SetupSio, Rp2040Teardown) {
  sio_gpio_peripheral.SetInput(gpio1Pin);
  sio_gpio_peripheral.SetOutput(gpio0Pin);
  sio_gpio_peripheral.SetHigh(gpio0Pin);
  minUnitCheck(sio_gpio_peripheral.GetLevel(gpio1Pin) != 0u);
  sio_gpio_peripheral.SetLow(gpio0Pin);
  minUnitCheck(sio_gpio_peripheral.GetLevel(gpio1Pin) == 0u);
  // switch around pins
  sio_gpio_peripheral.SetInput(gpio0Pin);
  sio_gpio_peripheral.SetOutput(gpio1Pin);
  sio_gpio_peripheral.SetHigh(gpio1Pin);
  minUnitCheck(sio_gpio_peripheral.GetLevel(gpio0Pin) != 0u);
  sio_gpio_peripheral.SetLow(gpio1Pin);
  minUnitCheck(sio_gpio_peripheral.GetLevel(gpio0Pin) == 0u);
  // testing the remainder methods
  sio_gpio_peripheral.Toggle(gpio1Pin);
  minUnitCheck(sio_gpio_peripheral.GetLevel(gpio0Pin) != 0u);
  sio_gpio_peripheral.Toggle(gpio1Pin);
  minUnitCheck(sio_gpio_peripheral.GetLevel(gpio0Pin) == 0u);
  sio_gpio_peripheral.SetLevel(gpio1Pin, 1);
  minUnitCheck(sio_gpio_peripheral.GetLevel(gpio0Pin) != 0u);
  sio_gpio_peripheral.SetLevel(gpio1Pin, 0);
  minUnitCheck(sio_gpio_peripheral.GetLevel(gpio0Pin) == 0u);
}