/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC845M301 GPIO peripheral
 */
#include <nuclone_LPC845BD48_small_LL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>
#include <common.hpp>

using namespace libmcuhw::gpio;
using namespace libmcull::gpio;

// peripheral register sets
static constexpr libmcu::HwAddressType gpio_address = libmcuhw::gpioAddress;
libmcuhw::gpio::Gpio *const gpio_registers{reinterpret_cast<libmcuhw::gpio::Gpio *>(gpio_address)};

/**
 * @brief Gpio setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupGpio) {
  minUnitCheck(LPC845M301TeardownCorrect() == true);
  sysconPeripheral.enablePeripheralClocks(libmcull::syscon::peripheralClocks0::GPIO0 | libmcull::syscon::peripheralClocks0::GPIO1,
                                          0);
}

// testing all single pin methods
MINUNIT_ADD(LPC845M301DH20GpioPin, LPC845M301SetupGpio, LPC845M301Teardown) {
  gpioPeripheral.input(testPin1);
  gpioPeripheral.output(testPin2);
  gpioPeripheral.high(testPin2);
  minUnitCheck(gpioPeripheral.get(testPin1) != 0u);
  gpioPeripheral.low(testPin2);
  minUnitCheck(gpioPeripheral.get(testPin1) == 0u);
  // switch around pins
  gpioPeripheral.input(testPin2);
  gpioPeripheral.output(testPin1);
  gpioPeripheral.high(testPin1);
  minUnitCheck(gpioPeripheral.get(testPin2) != 0u);
  gpioPeripheral.low(testPin1);
  minUnitCheck(gpioPeripheral.get(testPin2) == 0u);
  // testing the remainder methods
  gpioPeripheral.toggle(testPin1);
  minUnitCheck(gpioPeripheral.get(testPin2) != 0u);
  gpioPeripheral.toggle(testPin1);
  minUnitCheck(gpioPeripheral.get(testPin2) == 0u);
  gpioPeripheral.set(testPin1, 1);
  minUnitCheck(gpioPeripheral.get(testPin2) != 0u);
  gpioPeripheral.set(testPin1, 0);
  minUnitCheck(gpioPeripheral.get(testPin2) == 0u);
}

// testing all port functions
MINUNIT_ADD(LPC845M301DH20GpioPort, LPC845M301SetupGpio, LPC845M301Teardown) {
  gpioPeripheral.portDirection(testPin2Port, 0);
  gpioPeripheral.portDirection(testPin1Port, testPin1.gpioPinMask);
  gpioPeripheral.SetPortLow(testPin1Port, testPin1.gpioPinMask);
  minUnitCheck(gpioPeripheral.GetPort(testPin2Port, testPin2.gpioPinMask) == 0x00000000);
  gpioPeripheral.SetPortHigh(testPin1Port, testPin1.gpioPinMask);
  minUnitCheck(gpioPeripheral.GetPort(testPin2Port, testPin2.gpioPinMask) == testPin2.gpioPinMask);
  // switch around pins
  gpioPeripheral.portDirection(testPin1Port, 0);
  gpioPeripheral.portDirection(testPin2Port, testPin2.gpioPinMask);
  gpioPeripheral.SetPortHigh(testPin2Port, testPin2.gpioPinMask);
  minUnitCheck(gpioPeripheral.GetPort(testPin1Port, testPin1.gpioPinMask) == testPin1.gpioPinMask);
  gpioPeripheral.SetPortLow(testPin2Port, testPin2.gpioPinMask);
  minUnitCheck(gpioPeripheral.GetPort(testPin1Port, testPin1.gpioPinMask) == 0x00000000);
  // testing the remainder methods
  gpioPeripheral.TogglePort(testPin2Port, testPin2.gpioPinMask);
  minUnitCheck(gpioPeripheral.GetPort(testPin1Port, testPin1.gpioPinMask) == testPin1.gpioPinMask);
  gpioPeripheral.TogglePort(testPin2Port, testPin2.gpioPinMask);
  minUnitCheck(gpioPeripheral.GetPort(testPin1Port, testPin1.gpioPinMask) == 0x00000000);
}