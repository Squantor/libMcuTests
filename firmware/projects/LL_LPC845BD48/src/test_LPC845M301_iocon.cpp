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
static constexpr libmcu::HwAddressType iocon_address = libmcuhw::kIoconAddress; /**< peripheral address */
libmcuhw::iocon::Iocon *const iocon_registers{reinterpret_cast<libmcuhw::iocon::Iocon *>(iocon_address)};

/**
 * @brief Iocon setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupIocon) {
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::peripheral_clocks_0::kIocon |
                                             libmcull::syscon::peripheral_clocks_0::kGpio0 |
                                             libmcull::syscon::peripheral_clocks_0::kGpio1,
                                           0);
}

/**
 * @brief Tests iocon pullup/down functions
 *
 */
MINUNIT_ADD(LPC845M301DH20IoconPull, LPC845M301SetupIocon, LPC845M301Teardown) {
  gpio_peripheral.SetInput(test_pin_2);
  gpio_peripheral.SetInput(test_pin_1);
  iocon_peripheral.Setup(test_pin_1, PullModes::PULLUP);
  iocon_peripheral.Setup(test_pin_2, PullModes::INACTIVE);
  libmcu::Delay(100);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) != 0);
  iocon_peripheral.Setup(test_pin_1, PullModes::PULLDOWN);
  libmcu::Delay(100);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) == 0);
  iocon_peripheral.Setup(test_pin_1, PullModes::PULLUP);
  libmcu::Delay(100);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) != 0);
}

/**
 * @brief tests for the IOCON repeater function
 */
MINUNIT_ADD(LPC845M301DH20IoconRepeater, LPC845M301SetupIocon, LPC845M301Teardown) {
  gpio_peripheral.SetInput(test_pin_2);
  gpio_peripheral.SetInput(test_pin_1);
  // check pulled up buskeeper
  iocon_peripheral.Setup(test_pin_1, PullModes::INACTIVE);
  iocon_peripheral.Setup(test_pin_2, PullModes::INACTIVE);
  iocon_peripheral.Setup(test_pin_1, PullModes::PULLUP);
  iocon_peripheral.Setup(test_pin_2, PullModes::REPEATER);
  libmcu::Delay(100);
  iocon_peripheral.Setup(test_pin_1, PullModes::INACTIVE);
  minUnitCheck(gpio_peripheral.GetState(test_pin_1) != 0);
  // check pulled down buskeeper and also flipping the pins
  iocon_peripheral.Setup(test_pin_2, PullModes::INACTIVE);
  iocon_peripheral.Setup(test_pin_1, PullModes::INACTIVE);
  iocon_peripheral.Setup(test_pin_2, PullModes::PULLDOWN);
  iocon_peripheral.Setup(test_pin_1, PullModes::REPEATER);
  libmcu::Delay(100);
  iocon_peripheral.Setup(test_pin_2, PullModes::INACTIVE);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) == 0);
}

/**
 * @brief tests for the IOCON open drain function
 */
MINUNIT_ADD(LPC845M301DH20IoconOpenDrain, LPC845M301SetupIocon, LPC845M301Teardown) {
  gpio_peripheral.SetInput(test_pin_2);
  gpio_peripheral.SetInput(test_pin_1);
  iocon_peripheral.Setup(test_pin_1, PullModes::INACTIVE, PIO::kOD);
  iocon_peripheral.Setup(test_pin_2, PullModes::PULLUP);
  gpio_peripheral.SetOutput(test_pin_1);
  gpio_peripheral.SetLow(test_pin_1);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) == 0);
  gpio_peripheral.SetHigh(test_pin_1);
  libmcu::Delay(100);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) != 0);
  iocon_peripheral.Setup(test_pin_2, PullModes::PULLDOWN);
  libmcu::Delay(100);
  minUnitCheck(gpio_peripheral.GetState(test_pin_2) == 0);
}

/* TODO:
 * Test glitch filtering, depends on timer subsystem for generating clock pulses
 * Analog pin tests, depends on ADC/Comparator, will be tested with comparator/ADC
 */