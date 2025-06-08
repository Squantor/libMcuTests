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

using namespace libmcuhw::iocon;
using namespace libmcull::iocon;

// peripheral register sets
static constexpr libmcu::HwAddressType iocon_address = libmcuhw::kIoconAddress; /**< peripheral address */
libmcuhw::iocon::Iocon *const iocon_registers{reinterpret_cast<libmcuhw::iocon::Iocon *>(iocon_address)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupIocon) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  sysconPeripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::kClockIocon);
}

/**
 * @brief Tests iocon pullup/down functions
 *
 */
MINUNIT_ADD(LPC812M101DH20IoconPull, LPC812M101CppSetupIocon, LPC812M101Teardown) {
  gpioPeripheral.SetInput(test1Pin);
  gpioPeripheral.SetInput(test0Pin);
  ioconPeripheral.Setup(test0Pin, PullModes::PULLUP);
  ioconPeripheral.Setup(test1Pin, PullModes::INACTIVE);
  minUnitCheck(gpioPeripheral.GetState(test1Pin) != 0);
  ioconPeripheral.Setup(test0Pin, PullModes::PULLDOWN);
  minUnitCheck(gpioPeripheral.GetState(test1Pin) == 0);
  ioconPeripheral.Setup(test0Pin, PullModes::PULLUP);
  minUnitCheck(gpioPeripheral.GetState(test1Pin) != 0);
}

/**
 * @brief tests for the IOCON repeater function
 */
MINUNIT_ADD(LPC812M101DH20IoconRepeater, LPC812M101CppSetupIocon, LPC812M101Teardown) {
  gpioPeripheral.SetInput(test1Pin);
  gpioPeripheral.SetInput(test0Pin);
  // check pulled up buskeeper
  ioconPeripheral.Setup(test0Pin, PullModes::INACTIVE);
  ioconPeripheral.Setup(test1Pin, PullModes::INACTIVE);
  ioconPeripheral.Setup(test0Pin, PullModes::PULLUP);
  ioconPeripheral.Setup(test1Pin, PullModes::REPEATER);
  ioconPeripheral.Setup(test0Pin, PullModes::INACTIVE);
  minUnitCheck(gpioPeripheral.GetState(test0Pin) != 0);
  // check pulled down buskeeper and also flipping the pins
  ioconPeripheral.Setup(test1Pin, PullModes::INACTIVE);
  ioconPeripheral.Setup(test0Pin, PullModes::INACTIVE);
  ioconPeripheral.Setup(test1Pin, PullModes::PULLDOWN);
  ioconPeripheral.Setup(test0Pin, PullModes::REPEATER);
  ioconPeripheral.Setup(test1Pin, PullModes::INACTIVE);
  minUnitCheck(gpioPeripheral.GetState(test1Pin) == 0);
}

/**
 * @brief tests for the IOCON open drain function
 */
MINUNIT_ADD(LPC812M101DH20IoconOpenDrain, LPC812M101CppSetupIocon, LPC812M101Teardown) {
  gpioPeripheral.SetInput(test1Pin);
  gpioPeripheral.SetInput(test0Pin);
  ioconPeripheral.Setup(test0Pin, PullModes::INACTIVE, PIO::OD);
  ioconPeripheral.Setup(test1Pin, PullModes::PULLUP);
  gpioPeripheral.SetOutput(test0Pin);
  gpioPeripheral.SetLow(test0Pin);
  minUnitCheck(gpioPeripheral.GetState(test1Pin) == 0);
  gpioPeripheral.SetHigh(test0Pin);
  minUnitCheck(gpioPeripheral.GetState(test1Pin) != 0);
  ioconPeripheral.Setup(test1Pin, PullModes::PULLDOWN);
  minUnitCheck(gpioPeripheral.GetState(test1Pin) == 0);
}

/* TODO:
 * Test glitch filtering, depends on timer subsystem for generating clock pulses
 * Analog pin tests, depends on ADC/Comparator, will be tested with comparator/ADC
 */