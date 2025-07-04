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
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(
    libmcull::syscon::peripheral_clocks_0::Gpio0 | libmcull::syscon::peripheral_clocks_0::Gpio1, 0);
}

// testing all single pin methods
MINUNIT_ADD(LPC845M301DH20GpioPin, LPC845M301SetupGpio, LPC845M301Teardown) {
  gpio_peripheral.SetInput(test_pin_1);
  gpio_peripheral.SetOutput(test_pin_2);
  gpio_peripheral.SetHigh(test_pin_2);
  minUnitCheck(gpio_peripheral.GetState(test_pin_1) != 0u);
  gpio_peripheral.SetLow(test_pin_2);
  minUnitCheck(gpio_peripheral.GetState(test_pin_1) == 0u);
  // switch around pins
  gpio_peripheral.SetInput(test_pin_2);
  gpio_peripheral.SetOutput(test_pin_1);
  gpio_peripheral.SetHigh(test_pin_1);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) != 0u);
  gpio_peripheral.SetLow(test_pin_1);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) == 0u);
  // testing the remainder methods
  gpio_peripheral.Toggle(test_pin_1);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) != 0u);
  gpio_peripheral.Toggle(test_pin_1);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) == 0u);
  gpio_peripheral.SetState(test_pin_1, 1);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) != 0u);
  gpio_peripheral.SetState(test_pin_1, 0);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) == 0u);
}

// testing all port functions
MINUNIT_ADD(LPC845M301DH20GpioPort, LPC845M301SetupGpio, LPC845M301Teardown) {
  gpio_peripheral.SetPortDirection(test_pin_2_port, 0);
  gpio_peripheral.SetPortDirection(test_pin_1_port, test_pin_1.gpioPinMask);
  gpio_peripheral.SetPortLow(test_pin_1_port, test_pin_1.gpioPinMask);
  minUnitCheck(gpio_peripheral.GetPortState(test_pin_2_port, test_pin_2.gpioPinMask) == 0x00000000);
  gpio_peripheral.SetPortHigh(test_pin_1_port, test_pin_1.gpioPinMask);
  minUnitCheck(gpio_peripheral.GetPortState(test_pin_2_port, test_pin_2.gpioPinMask) == test_pin_2.gpioPinMask);
  // switch around pins
  gpio_peripheral.SetPortDirection(test_pin_1_port, 0);
  gpio_peripheral.SetPortDirection(test_pin_2_port, test_pin_2.gpioPinMask);
  gpio_peripheral.SetPortHigh(test_pin_2_port, test_pin_2.gpioPinMask);
  minUnitCheck(gpio_peripheral.GetPortState(test_pin_1_port, test_pin_1.gpioPinMask) == test_pin_1.gpioPinMask);
  gpio_peripheral.SetPortLow(test_pin_2_port, test_pin_2.gpioPinMask);
  minUnitCheck(gpio_peripheral.GetPortState(test_pin_1_port, test_pin_1.gpioPinMask) == 0x00000000);
  // testing the remainder methods
  gpio_peripheral.TogglePort(test_pin_2_port, test_pin_2.gpioPinMask);
  minUnitCheck(gpio_peripheral.GetPortState(test_pin_1_port, test_pin_1.gpioPinMask) == test_pin_1.gpioPinMask);
  gpio_peripheral.TogglePort(test_pin_2_port, test_pin_2.gpioPinMask);
  minUnitCheck(gpio_peripheral.GetPortState(test_pin_1_port, test_pin_1.gpioPinMask) == 0x00000000);
}