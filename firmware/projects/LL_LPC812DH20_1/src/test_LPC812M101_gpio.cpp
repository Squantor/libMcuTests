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

using namespace libMcuLL::hw::gpio;
using namespace libMcuLL::sw::gpio;

// peripheral register sets
static constexpr libMcuLL::hwAddressType gpioAddress = libMcuLL::hw::gpioAddress;
libMcuLL::hw::gpio::peripheral *const dutRegisters{reinterpret_cast<libMcuLL::hw::gpio::peripheral *>(gpioAddress)};

/**
 * @brief Gpio setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupGpio) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  sysconPeripheral.enablePeripheralClocks(libMcuLL::sw::syscon::peripheralClocks::GPIO);
}

// testing all single pin methods
MINUNIT_ADD(LPC812M101DH20GpioPin, LPC812M101CppSetupGpio, LPC812M101Teardown) {
  gpioPeripheral.input(test1Pin);
  gpioPeripheral.output(test0Pin);
  gpioPeripheral.high(test0Pin);
  minUnitCheck(gpioPeripheral.get(test1Pin) != 0u);
  gpioPeripheral.low(test0Pin);
  minUnitCheck(gpioPeripheral.get(test1Pin) == 0u);
  // switch around pins
  gpioPeripheral.input(test0Pin);
  gpioPeripheral.output(test1Pin);
  gpioPeripheral.high(test1Pin);
  minUnitCheck(gpioPeripheral.get(test0Pin) != 0u);
  gpioPeripheral.low(test1Pin);
  minUnitCheck(gpioPeripheral.get(test0Pin) == 0u);
  // testing the remainder methods
  gpioPeripheral.toggle(test1Pin);
  minUnitCheck(gpioPeripheral.get(test0Pin) != 0u);
  gpioPeripheral.toggle(test1Pin);
  minUnitCheck(gpioPeripheral.get(test0Pin) == 0u);
  gpioPeripheral.set(test1Pin, 1);
  minUnitCheck(gpioPeripheral.get(test0Pin) != 0u);
  gpioPeripheral.set(test1Pin, 0);
  minUnitCheck(gpioPeripheral.get(test0Pin) == 0u);
}

// testing all port functions
MINUNIT_ADD(LPC812M101DH20GpioPort, LPC812M101CppSetupGpio, LPC812M101Teardown) {
  gpioPeripheral.portDirection(test0Pin, test0Pin.gpioPinMask);
  gpioPeripheral.portLow(test0Pin, test0Pin.gpioPinMask);
  crudeDelay(10);
  minUnitCheck(gpioPeripheral.portGet(test1Pin, test1Pin.gpioPinMask) == 0x00000000);
  gpioPeripheral.portHigh(test0Pin, test0Pin.gpioPinMask);
  minUnitCheck(gpioPeripheral.portGet(test1Pin, test1Pin.gpioPinMask) == test1Pin.gpioPinMask);
  // switch around pins
  gpioPeripheral.portDirection(test1Pin, test1Pin.gpioPinMask);
  gpioPeripheral.portHigh(test1Pin, test1Pin.gpioPinMask);
  minUnitCheck(gpioPeripheral.portGet(test0Pin, test0Pin.gpioPinMask) == test0Pin.gpioPinMask);
  gpioPeripheral.portLow(test1Pin, test1Pin.gpioPinMask);
  minUnitCheck(gpioPeripheral.portGet(test0Pin, test0Pin.gpioPinMask) == 0x00000000);
  // testing the remainder methods
  gpioPeripheral.portToggle(test1Pin, test1Pin.gpioPinMask);
  minUnitCheck(gpioPeripheral.portGet(test0Pin, test0Pin.gpioPinMask) == test0Pin.gpioPinMask);
  gpioPeripheral.portToggle(test1Pin, test1Pin.gpioPinMask);
  minUnitCheck(gpioPeripheral.portGet(test0Pin, test0Pin.gpioPinMask) == 0x00000000);
}