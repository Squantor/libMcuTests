/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC812M101 GPIO peripheral
 */
#include <nuclone_LPC812M101DH20_tests.hpp>
#include <MinUnit.h>
#include <LPC812M101_teardown.hpp>
#include <common.hpp>

using namespace libmcuhw::gpio;
using namespace libmcull::gpio;

// peripheral register sets
static constexpr libmcu::HwAddressType gpio_address = libmcuhw::kGpioAddress;
libmcuhw::gpio::Gpio *const gpio_registers{reinterpret_cast<libmcuhw::gpio::Gpio *>(gpio_address)};

/**
 * @brief Gpio setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupGpio) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  sysconPeripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::kClockGpio);
}

// testing all single pin methods
MINUNIT_ADD(LPC812M101DH20GpioPin, LPC812M101CppSetupGpio, LPC812M101Teardown) {
  gpioPeripheral.SetInput(test1Pin);
  gpioPeripheral.SetOutput(test0Pin);
  gpioPeripheral.SetHigh(test0Pin);
  minUnitCheck(gpioPeripheral.GetState(test1Pin) != 0u);
  gpioPeripheral.SetLow(test0Pin);
  minUnitCheck(gpioPeripheral.GetState(test1Pin) == 0u);
  // switch around pins
  gpioPeripheral.SetInput(test0Pin);
  gpioPeripheral.SetOutput(test1Pin);
  gpioPeripheral.SetHigh(test1Pin);
  minUnitCheck(gpioPeripheral.GetState(test0Pin) != 0u);
  gpioPeripheral.SetLow(test1Pin);
  minUnitCheck(gpioPeripheral.GetState(test0Pin) == 0u);
  // testing the remainder methods
  gpioPeripheral.Toggle(test1Pin);
  minUnitCheck(gpioPeripheral.GetState(test0Pin) != 0u);
  gpioPeripheral.Toggle(test1Pin);
  minUnitCheck(gpioPeripheral.GetState(test0Pin) == 0u);
  gpioPeripheral.SetState(test1Pin, 1);
  minUnitCheck(gpioPeripheral.GetState(test0Pin) != 0u);
  gpioPeripheral.SetState(test1Pin, 0);
  minUnitCheck(gpioPeripheral.GetState(test0Pin) == 0u);
}

// testing all port functions
MINUNIT_ADD(LPC812M101DH20GpioPort, LPC812M101CppSetupGpio, LPC812M101Teardown) {
  gpioPeripheral.SetPortDirection(test0Pin, test0Pin.gpio_pin_mask);
  gpioPeripheral.SetPortLow(test0Pin, test0Pin.gpio_pin_mask);
  libmcu::Delay(10);
  minUnitCheck(gpioPeripheral.GetPort(test1Pin, test1Pin.gpio_pin_mask) == 0x00000000);
  gpioPeripheral.SetPortHigh(test0Pin, test0Pin.gpio_pin_mask);
  minUnitCheck(gpioPeripheral.GetPort(test1Pin, test1Pin.gpio_pin_mask) == test1Pin.gpio_pin_mask);
  // switch around pins
  gpioPeripheral.SetPortDirection(test1Pin, test1Pin.gpio_pin_mask);
  gpioPeripheral.SetPortHigh(test1Pin, test1Pin.gpio_pin_mask);
  minUnitCheck(gpioPeripheral.GetPort(test0Pin, test0Pin.gpio_pin_mask) == test0Pin.gpio_pin_mask);
  gpioPeripheral.SetPortLow(test1Pin, test1Pin.gpio_pin_mask);
  minUnitCheck(gpioPeripheral.GetPort(test0Pin, test0Pin.gpio_pin_mask) == 0x00000000);
  // testing the remainder methods
  gpioPeripheral.TogglePort(test1Pin, test1Pin.gpio_pin_mask);
  minUnitCheck(gpioPeripheral.GetPort(test0Pin, test0Pin.gpio_pin_mask) == test0Pin.gpio_pin_mask);
  gpioPeripheral.TogglePort(test1Pin, test1Pin.gpio_pin_mask);
  minUnitCheck(gpioPeripheral.GetPort(test0Pin, test0Pin.gpio_pin_mask) == 0x00000000);
}