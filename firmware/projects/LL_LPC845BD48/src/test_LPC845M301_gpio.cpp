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
static constexpr libmcu::HwAddressType gpio_address = libmcuhw::kGpioAddress;
libmcuhw::gpio::Gpio *const gpio_registers{reinterpret_cast<libmcuhw::gpio::Gpio *>(gpio_address)};

/**
 * @brief Gpio setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupGpio) {
  minUnitCheck(LPC845M301TeardownCorrect() == true);
  sysconPeripheral.EnablePeripheralClocks(
    libmcull::syscon::peripheral_clocks_0::kGpio0 | libmcull::syscon::peripheral_clocks_0::kGpio1, 0);
}

// testing all single pin methods
MINUNIT_ADD(LPC845M301DH20GpioPin, LPC845M301SetupGpio, LPC845M301Teardown) {
  gpioPeripheral.SetInput(testPin1);
  gpioPeripheral.SetOutput(testPin2);
  gpioPeripheral.SetHigh(testPin2);
  minUnitCheck(gpioPeripheral.GetState(testPin1) != 0u);
  gpioPeripheral.SetLow(testPin2);
  minUnitCheck(gpioPeripheral.GetState(testPin1) == 0u);
  // switch around pins
  gpioPeripheral.SetInput(testPin2);
  gpioPeripheral.SetOutput(testPin1);
  gpioPeripheral.SetHigh(testPin1);
  minUnitCheck(gpioPeripheral.GetState(testPin2) != 0u);
  gpioPeripheral.SetLow(testPin1);
  minUnitCheck(gpioPeripheral.GetState(testPin2) == 0u);
  // testing the remainder methods
  gpioPeripheral.Toggle(testPin1);
  minUnitCheck(gpioPeripheral.GetState(testPin2) != 0u);
  gpioPeripheral.Toggle(testPin1);
  minUnitCheck(gpioPeripheral.GetState(testPin2) == 0u);
  gpioPeripheral.SetState(testPin1, 1);
  minUnitCheck(gpioPeripheral.GetState(testPin2) != 0u);
  gpioPeripheral.SetState(testPin1, 0);
  minUnitCheck(gpioPeripheral.GetState(testPin2) == 0u);
}

// testing all port functions
MINUNIT_ADD(LPC845M301DH20GpioPort, LPC845M301SetupGpio, LPC845M301Teardown) {
  gpioPeripheral.SetPortDirection(testPin2Port, 0);
  gpioPeripheral.SetPortDirection(testPin1Port, testPin1.gpioPinMask);
  gpioPeripheral.SetPortLow(testPin1Port, testPin1.gpioPinMask);
  minUnitCheck(gpioPeripheral.GetPortState(testPin2Port, testPin2.gpioPinMask) == 0x00000000);
  gpioPeripheral.SetPortHigh(testPin1Port, testPin1.gpioPinMask);
  minUnitCheck(gpioPeripheral.GetPortState(testPin2Port, testPin2.gpioPinMask) == testPin2.gpioPinMask);
  // switch around pins
  gpioPeripheral.SetPortDirection(testPin1Port, 0);
  gpioPeripheral.SetPortDirection(testPin2Port, testPin2.gpioPinMask);
  gpioPeripheral.SetPortHigh(testPin2Port, testPin2.gpioPinMask);
  minUnitCheck(gpioPeripheral.GetPortState(testPin1Port, testPin1.gpioPinMask) == testPin1.gpioPinMask);
  gpioPeripheral.SetPortLow(testPin2Port, testPin2.gpioPinMask);
  minUnitCheck(gpioPeripheral.GetPortState(testPin1Port, testPin1.gpioPinMask) == 0x00000000);
  // testing the remainder methods
  gpioPeripheral.TogglePort(testPin2Port, testPin2.gpioPinMask);
  minUnitCheck(gpioPeripheral.GetPortState(testPin1Port, testPin1.gpioPinMask) == testPin1.gpioPinMask);
  gpioPeripheral.TogglePort(testPin2Port, testPin2.gpioPinMask);
  minUnitCheck(gpioPeripheral.GetPortState(testPin1Port, testPin1.gpioPinMask) == 0x00000000);
}