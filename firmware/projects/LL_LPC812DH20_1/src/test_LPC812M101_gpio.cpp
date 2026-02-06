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
#include <minunit.h>
#include <LPC812M101_teardown.hpp>
#include <common.hpp>

using namespace libmcuhw::gpio;
using namespace libmcull::gpio;

// peripheral register sets
static constexpr libmcu::HwAddressType gpio_address = libmcuhw::GpioAddress;
libmcuhw::gpio::Gpio *const gpio_registers{reinterpret_cast<libmcuhw::gpio::Gpio *>(gpio_address)};

/**
 * @brief Gpio setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupGpio) {
  MINUNIT_CHECK(Lpc812M101TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::ClockGpio);
}

// testing all single pin methods
MINUNIT_ADD(LPC812M101DH20GpioPin, LPC812M101CppSetupGpio, LPC812M101Teardown) {
  gpio_peripheral.SetInput(test_1_pin);
  gpio_peripheral.SetOutput(test_0_pin);
  gpio_peripheral.SetHigh(test_0_pin);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_1_pin) != 0u);
  gpio_peripheral.SetLow(test_0_pin);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_1_pin) == 0u);
  // switch around pins
  gpio_peripheral.SetInput(test_0_pin);
  gpio_peripheral.SetOutput(test_1_pin);
  gpio_peripheral.SetHigh(test_1_pin);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_0_pin) != 0u);
  gpio_peripheral.SetLow(test_1_pin);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_0_pin) == 0u);
  // testing the remainder methods
  gpio_peripheral.Toggle(test_1_pin);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_0_pin) != 0u);
  gpio_peripheral.Toggle(test_1_pin);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_0_pin) == 0u);
  gpio_peripheral.SetState(test_1_pin, 1);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_0_pin) != 0u);
  gpio_peripheral.SetState(test_1_pin, 0);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_0_pin) == 0u);
}

// testing all port functions
MINUNIT_ADD(LPC812M101DH20GpioPort, LPC812M101CppSetupGpio, LPC812M101Teardown) {
  gpio_peripheral.SetPortDirection(test_0_pin, test_0_pin.gpio_pin_mask);
  gpio_peripheral.SetPortLow(test_0_pin, test_0_pin.gpio_pin_mask);
  libmcu::Delay(10);
  MINUNIT_CHECK(gpio_peripheral.GetPort(test_1_pin, test_1_pin.gpio_pin_mask) == 0x00000000);
  gpio_peripheral.SetPortHigh(test_0_pin, test_0_pin.gpio_pin_mask);
  MINUNIT_CHECK(gpio_peripheral.GetPort(test_1_pin, test_1_pin.gpio_pin_mask) == test_1_pin.gpio_pin_mask);
  // switch around pins
  gpio_peripheral.SetPortDirection(test_1_pin, test_1_pin.gpio_pin_mask);
  gpio_peripheral.SetPortHigh(test_1_pin, test_1_pin.gpio_pin_mask);
  MINUNIT_CHECK(gpio_peripheral.GetPort(test_0_pin, test_0_pin.gpio_pin_mask) == test_0_pin.gpio_pin_mask);
  gpio_peripheral.SetPortLow(test_1_pin, test_1_pin.gpio_pin_mask);
  MINUNIT_CHECK(gpio_peripheral.GetPort(test_0_pin, test_0_pin.gpio_pin_mask) == 0x00000000);
  // testing the remainder methods
  gpio_peripheral.TogglePort(test_1_pin, test_1_pin.gpio_pin_mask);
  MINUNIT_CHECK(gpio_peripheral.GetPort(test_0_pin, test_0_pin.gpio_pin_mask) == test_0_pin.gpio_pin_mask);
  gpio_peripheral.TogglePort(test_1_pin, test_1_pin.gpio_pin_mask);
  MINUNIT_CHECK(gpio_peripheral.GetPort(test_0_pin, test_0_pin.gpio_pin_mask) == 0x00000000);
}