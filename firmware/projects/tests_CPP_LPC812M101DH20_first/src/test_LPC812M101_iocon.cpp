/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC812M101 IOCON peripheral
 */
#include <nuclone_LPC812M101DH20_tests.hpp>
#include <MinUnit.h>
#include <LPC812M101_teardown.hpp>
#include <common.hpp>

using namespace libMcuLL::hw::iocon;
using namespace libMcuLL::sw::iocon;

// peripheral register sets
static constexpr libMcuLL::hwAddressType ioconAddress = libMcuLL::hw::ioconAddress; /**< peripheral address */
libMcuLL::hw::iocon::peripheral *const dutRegisters{reinterpret_cast<libMcuLL::hw::iocon::peripheral *>(ioconAddress)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupIocon) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  sysconPeripheral.enablePeripheralClocks(libMcuLL::sw::syscon::peripheralClocks::IOCON);
}

/**
 * @brief Tests iocon pullup/down functions
 *
 */
MINUNIT_ADD(LPC812M101DH20IoconPull, LPC812M101CppSetupIocon, LPC812M101Teardown) {
  gpioPeripheral.input(test1Pin);
  gpioPeripheral.input(test0Pin);
  ioconPeripheral.setup(test0Pin, pullModes::PULLUP);
  ioconPeripheral.setup(test1Pin, pullModes::INACTIVE);
  minUnitCheck(gpioPeripheral.get(test1Pin) != 0);
  ioconPeripheral.setup(test0Pin, pullModes::PULLDOWN);
  minUnitCheck(gpioPeripheral.get(test1Pin) == 0);
  ioconPeripheral.setup(test0Pin, pullModes::PULLUP);
  minUnitCheck(gpioPeripheral.get(test1Pin) != 0);
}

/**
 * @brief tests for the IOCON repeater function
 */
MINUNIT_ADD(LPC812M101DH20IoconRepeater, LPC812M101CppSetupIocon, LPC812M101Teardown) {
  gpioPeripheral.input(test1Pin);
  gpioPeripheral.input(test0Pin);
  // check pulled up buskeeper
  ioconPeripheral.setup(test0Pin, pullModes::INACTIVE);
  ioconPeripheral.setup(test1Pin, pullModes::INACTIVE);
  ioconPeripheral.setup(test0Pin, pullModes::PULLUP);
  ioconPeripheral.setup(test1Pin, pullModes::REPEATER);
  ioconPeripheral.setup(test0Pin, pullModes::INACTIVE);
  minUnitCheck(gpioPeripheral.get(test0Pin) != 0);
  // check pulled down buskeeper and also flipping the pins
  ioconPeripheral.setup(test1Pin, pullModes::INACTIVE);
  ioconPeripheral.setup(test0Pin, pullModes::INACTIVE);
  ioconPeripheral.setup(test1Pin, pullModes::PULLDOWN);
  ioconPeripheral.setup(test0Pin, pullModes::REPEATER);
  ioconPeripheral.setup(test1Pin, pullModes::INACTIVE);
  minUnitCheck(gpioPeripheral.get(test1Pin) == 0);
}

/**
 * @brief tests for the IOCON open drain function
 */
MINUNIT_ADD(LPC812M101DH20IoconOpenDrain, LPC812M101CppSetupIocon, LPC812M101Teardown) {
  gpioPeripheral.input(test1Pin);
  gpioPeripheral.input(test0Pin);
  ioconPeripheral.setup(test0Pin, pullModes::INACTIVE, PIO::OD);
  ioconPeripheral.setup(test1Pin, pullModes::PULLUP);
  gpioPeripheral.output(test0Pin);
  gpioPeripheral.low(test0Pin);
  minUnitCheck(gpioPeripheral.get(test1Pin) == 0);
  gpioPeripheral.high(test0Pin);
  minUnitCheck(gpioPeripheral.get(test1Pin) != 0);
  ioconPeripheral.setup(test1Pin, pullModes::PULLDOWN);
  minUnitCheck(gpioPeripheral.get(test1Pin) == 0);
}

/* TODO:
 * Test glitch filtering, depends on timer subsystem for generating clock pulses
 * Analog pin tests, depends on ADC/Comparator, will be tested with comparator/ADC
 */