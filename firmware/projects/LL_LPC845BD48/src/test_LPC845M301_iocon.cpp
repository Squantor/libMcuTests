/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC845M301 IOCON peripheral
 */
#include <nuclone_LPC845BD48_small_LL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>
#include <common.hpp>

using namespace libmcuhw::iocon;
using namespace libmcull::iocon;

// peripheral register sets
static constexpr libmcu::hwAddressType ioconAddress = libmcuhw::ioconAddress; /**< peripheral address */
libmcuhw::iocon::iocon *const dutRegisters{reinterpret_cast<libmcuhw::iocon::iocon *>(ioconAddress)};

/**
 * @brief Iocon setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupIocon) {
  minUnitCheck(LPC845M301TeardownCorrect() == true);
  sysconPeripheral.enablePeripheralClocks(libmcull::syscon::peripheralClocks0::IOCON | libmcull::syscon::peripheralClocks0::GPIO0 |
                                            libmcull::syscon::peripheralClocks0::GPIO1,
                                          0);
}

/**
 * @brief Tests iocon pullup/down functions
 *
 */
MINUNIT_ADD(LPC845M301DH20IoconPull, LPC845M301SetupIocon, LPC845M301Teardown) {
  gpioPeripheral.input(testPin2);
  gpioPeripheral.input(testPin1);
  ioconPeripheral.setup(testPin1, pullModes::PULLUP);
  ioconPeripheral.setup(testPin2, pullModes::INACTIVE);
  libmcu::Delay(100);
  minUnitCheck(gpioPeripheral.get(testPin2) != 0);
  ioconPeripheral.setup(testPin1, pullModes::PULLDOWN);
  libmcu::Delay(100);
  minUnitCheck(gpioPeripheral.get(testPin2) == 0);
  ioconPeripheral.setup(testPin1, pullModes::PULLUP);
  libmcu::Delay(100);
  minUnitCheck(gpioPeripheral.get(testPin2) != 0);
}

/**
 * @brief tests for the IOCON repeater function
 */
MINUNIT_ADD(LPC845M301DH20IoconRepeater, LPC845M301SetupIocon, LPC845M301Teardown) {
  gpioPeripheral.input(testPin2);
  gpioPeripheral.input(testPin1);
  // check pulled up buskeeper
  ioconPeripheral.setup(testPin1, pullModes::INACTIVE);
  ioconPeripheral.setup(testPin2, pullModes::INACTIVE);
  ioconPeripheral.setup(testPin1, pullModes::PULLUP);
  ioconPeripheral.setup(testPin2, pullModes::REPEATER);
  libmcu::Delay(100);
  ioconPeripheral.setup(testPin1, pullModes::INACTIVE);
  minUnitCheck(gpioPeripheral.get(testPin1) != 0);
  // check pulled down buskeeper and also flipping the pins
  ioconPeripheral.setup(testPin2, pullModes::INACTIVE);
  ioconPeripheral.setup(testPin1, pullModes::INACTIVE);
  ioconPeripheral.setup(testPin2, pullModes::PULLDOWN);
  ioconPeripheral.setup(testPin1, pullModes::REPEATER);
  libmcu::Delay(100);
  ioconPeripheral.setup(testPin2, pullModes::INACTIVE);
  minUnitCheck(gpioPeripheral.get(testPin2) == 0);
}

/**
 * @brief tests for the IOCON open drain function
 */
MINUNIT_ADD(LPC845M301DH20IoconOpenDrain, LPC845M301SetupIocon, LPC845M301Teardown) {
  gpioPeripheral.input(testPin2);
  gpioPeripheral.input(testPin1);
  ioconPeripheral.setup(testPin1, pullModes::INACTIVE, PIO::OD);
  ioconPeripheral.setup(testPin2, pullModes::PULLUP);
  gpioPeripheral.output(testPin1);
  gpioPeripheral.low(testPin1);
  minUnitCheck(gpioPeripheral.get(testPin2) == 0);
  gpioPeripheral.high(testPin1);
  libmcu::Delay(100);
  minUnitCheck(gpioPeripheral.get(testPin2) != 0);
  ioconPeripheral.setup(testPin2, pullModes::PULLDOWN);
  libmcu::Delay(100);
  minUnitCheck(gpioPeripheral.get(testPin2) == 0);
}

/* TODO:
 * Test glitch filtering, depends on timer subsystem for generating clock pulses
 * Analog pin tests, depends on ADC/Comparator, will be tested with comparator/ADC
 */