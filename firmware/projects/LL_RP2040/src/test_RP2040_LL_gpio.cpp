/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 gpio
 */
#include <nuclone_RP2040_LL.hpp>
#include <MinUnit.h>
#include <RP2040_LL_teardown.hpp>
#include <common.hpp>

// peripheral register sets
static constexpr libmcu::HwAddressType dutAddress = libmcuhw::kIoBank0Address;
libmcuhw::gpio_bank0::GpioBank0 *const dutRegisters{reinterpret_cast<libmcuhw::gpio_bank0::GpioBank0 *>(dutAddress)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupGpio) {
  minUnitCheck(RP2040TeardownCorrect() == true);
  resetsPeripheral.Reset(libmcull::resets::kIoBank0 | libmcull::resets::kPadsBank0, 100000);
  // connect all GPIO's
  gpioBank0Peripheral.Setup(gpio0Pin);
  gpioBank0Peripheral.Setup(gpio1Pin);
  gpioBank0Peripheral.Setup(gpio2Pin);
  gpioBank0Peripheral.Setup(gpio3Pin);
  gpioBank0Peripheral.Setup(gpio4Pin);
  gpioBank0Peripheral.Setup(gpio5Pin);
}

MINUNIT_ADD(RP2040gpio, RP2040SetupGpio, RP2040Teardown) {
  minUnitCheck(dutRegisters->GPIO[0].CTRL == 0x0000'0005u);
  minUnitCheck(dutRegisters->GPIO[1].CTRL == 0x0000'0005u);
  minUnitCheck(dutRegisters->GPIO[2].CTRL == 0x0000'0005u);
  minUnitCheck(dutRegisters->GPIO[3].CTRL == 0x0000'0005u);
  minUnitCheck(dutRegisters->GPIO[4].CTRL == 0x0000'0005u);
  minUnitCheck(dutRegisters->GPIO[5].CTRL == 0x0000'0005u);
  minUnitPass();
}