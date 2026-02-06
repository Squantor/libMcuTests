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
#include <minunit.h>
#include <LPC812M101_teardown.hpp>
#include <common.hpp>

using namespace libmcuhw::iocon;
using namespace libmcull::iocon;

// peripheral register sets
static constexpr libmcu::HwAddressType iocon_address = libmcuhw::IoconAddress; /**< peripheral address */
libmcuhw::iocon::Iocon *const iocon_registers{reinterpret_cast<libmcuhw::iocon::Iocon *>(iocon_address)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupIocon) {
  MINUNIT_CHECK(Lpc812M101TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::ClockIocon);
}

/**
 * @brief Tests iocon pullup/down functions
 *
 */
MINUNIT_ADD(LPC812M101DH20IoconPull, LPC812M101CppSetupIocon, LPC812M101Teardown) {
  gpio_peripheral.SetInput(test_1_pin);
  gpio_peripheral.SetInput(test_0_pin);
  iocon_peripheral.Setup(test_0_pin, PullModes::PULLUP);
  iocon_peripheral.Setup(test_1_pin, PullModes::INACTIVE);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_1_pin) != 0);
  iocon_peripheral.Setup(test_0_pin, PullModes::PULLDOWN);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_1_pin) == 0);
  iocon_peripheral.Setup(test_0_pin, PullModes::PULLUP);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_1_pin) != 0);
}

/**
 * @brief tests for the IOCON repeater function
 */
MINUNIT_ADD(LPC812M101DH20IoconRepeater, LPC812M101CppSetupIocon, LPC812M101Teardown) {
  gpio_peripheral.SetInput(test_1_pin);
  gpio_peripheral.SetInput(test_0_pin);
  // check pulled up buskeeper
  iocon_peripheral.Setup(test_0_pin, PullModes::INACTIVE);
  iocon_peripheral.Setup(test_1_pin, PullModes::INACTIVE);
  iocon_peripheral.Setup(test_0_pin, PullModes::PULLUP);
  iocon_peripheral.Setup(test_1_pin, PullModes::REPEATER);
  iocon_peripheral.Setup(test_0_pin, PullModes::INACTIVE);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_0_pin) != 0);
  // check pulled down buskeeper and also flipping the pins
  iocon_peripheral.Setup(test_1_pin, PullModes::INACTIVE);
  iocon_peripheral.Setup(test_0_pin, PullModes::INACTIVE);
  iocon_peripheral.Setup(test_1_pin, PullModes::PULLDOWN);
  iocon_peripheral.Setup(test_0_pin, PullModes::REPEATER);
  iocon_peripheral.Setup(test_1_pin, PullModes::INACTIVE);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_1_pin) == 0);
}

/**
 * @brief tests for the IOCON open drain function
 */
MINUNIT_ADD(LPC812M101DH20IoconOpenDrain, LPC812M101CppSetupIocon, LPC812M101Teardown) {
  gpio_peripheral.SetInput(test_1_pin);
  gpio_peripheral.SetInput(test_0_pin);
  iocon_peripheral.Setup(test_0_pin, PullModes::INACTIVE, PIO::OD);
  iocon_peripheral.Setup(test_1_pin, PullModes::PULLUP);
  gpio_peripheral.SetOutput(test_0_pin);
  gpio_peripheral.SetLow(test_0_pin);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_1_pin) == 0);
  gpio_peripheral.SetHigh(test_0_pin);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_1_pin) != 0);
  iocon_peripheral.Setup(test_1_pin, PullModes::PULLDOWN);
  MINUNIT_CHECK(gpio_peripheral.GetState(test_1_pin) == 0);
}

/* TODO:
 * Test glitch filtering, depends on timer subsystem for generating clock pulses
 * Analog pin tests, depends on ADC/Comparator, will be tested with comparator/ADC
 */