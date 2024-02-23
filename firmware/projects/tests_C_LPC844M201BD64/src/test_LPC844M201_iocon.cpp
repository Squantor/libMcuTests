/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the LPC844M201 IOCON peripheral
 */
#include <nuclone_LPC844M201BD64_tests.hpp>
#include <MinUnit.h>
#include <LPC844M201_teardown.hpp>
#include <common.hpp>

/**
 * @brief Iocon setup and initialisation
 */
MINUNIT_SETUP(LPC844M201SetupIocon) {
  minUnitCheck(LPC844M201TeardownCorrect() == true);
  sysconEnableClocks(SYSCON, CLKCTRL0_GPIO0 | CLKCTRL0_GPIO1 | CLKCTRL0_IOCON, CLKCTRL1_NONE);
  sysconEnableResets(SYSCON, RESETCTRL0_GPIO0 | RESETCTRL0_GPIO1 | RESETCTRL0_IOCON, 0x00);
}

/**
 * @brief tests for the IOCON pullup and down function
 */
MINUNIT_ADD(LPC844M201IoconPull, LPC844M201SetupIocon, LPC844M201Teardown) {
  gpioSetPortDir(GPIO, GPIO_PORT0, 0);
  gpioSetPortDir(GPIO, GPIO_PORT1, 0);
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_0, IOCON_MODE(IOCON_MODE_INACTIVE));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_0, IOCON_MODE(IOCON_MODE_INACTIVE));
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_0_0, PIN_TESTPIN_0_0) == true);
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_1_0, PIN_TESTPIN_1_0) == true);
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_1, IOCON_MODE(IOCON_MODE_PULLDOWN));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_1, IOCON_MODE(IOCON_MODE_PULLDOWN));
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_0_0, PIN_TESTPIN_0_0) == false);
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_1_0, PIN_TESTPIN_1_0) == false);
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_1, IOCON_MODE(IOCON_MODE_PULLUP));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_1, IOCON_MODE(IOCON_MODE_PULLUP));
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_0_0, PIN_TESTPIN_0_0) == true);
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_1_0, PIN_TESTPIN_1_0) == true);
}

/**
 * @brief tests for the IOCON repeater function
 */
MINUNIT_ADD(LPC844M201IoconRepeater, LPC844M201SetupIocon, LPC844M201Teardown) {
  gpioSetPortDir(GPIO, PORT_TESTPIN_0_0, 0);
  gpioSetPortDir(GPIO, PORT_TESTPIN_0_1, 0);
  // check pulled up buskeeper
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_0, IOCON_MODE(IOCON_MODE_INACTIVE));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_0, IOCON_MODE(IOCON_MODE_INACTIVE));
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_1, IOCON_MODE(IOCON_MODE_INACTIVE));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_1, IOCON_MODE(IOCON_MODE_INACTIVE));
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_1, IOCON_MODE(IOCON_MODE_PULLUP));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_1, IOCON_MODE(IOCON_MODE_PULLUP));
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_0, IOCON_MODE(IOCON_MODE_REPEATER));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_0, IOCON_MODE(IOCON_MODE_REPEATER));
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_1, IOCON_MODE(IOCON_MODE_INACTIVE));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_1, IOCON_MODE(IOCON_MODE_INACTIVE));
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_0_1, PIN_TESTPIN_0_1) == true);
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_1_1, PIN_TESTPIN_1_1) == true);
  // check pulled down buskeeper and also flipping the pins
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_0, IOCON_MODE(IOCON_MODE_INACTIVE));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_0, IOCON_MODE(IOCON_MODE_INACTIVE));
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_1, IOCON_MODE(IOCON_MODE_INACTIVE));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_1, IOCON_MODE(IOCON_MODE_INACTIVE));
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_0, IOCON_MODE(IOCON_MODE_PULLDOWN));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_0, IOCON_MODE(IOCON_MODE_PULLDOWN));
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_1, IOCON_MODE(IOCON_MODE_REPEATER));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_1, IOCON_MODE(IOCON_MODE_REPEATER));
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_0, IOCON_MODE(IOCON_MODE_INACTIVE));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_0, IOCON_MODE(IOCON_MODE_INACTIVE));
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_0_0, PIN_TESTPIN_0_0) == false);
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_1_0, PIN_TESTPIN_1_0) == false);
}

/**
 * @brief tests for the IOCON open drain function
 */
MINUNIT_ADD(LPC844M201IoconOpenDrain, LPC844M201SetupIocon, LPC844M201Teardown) {
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_0, IOCON_MODE(IOCON_MODE_INACTIVE) | IOCON_OD(true));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_0, IOCON_MODE(IOCON_MODE_INACTIVE) | IOCON_OD(true));
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_1, IOCON_MODE(IOCON_MODE_PULLUP));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_1, IOCON_MODE(IOCON_MODE_PULLUP));
  gpioSetPortDIROutput(GPIO, PORT_TESTPIN_0_0, BITPOS(PIN_TESTPIN_0_0));
  gpioPortClear(GPIO, PORT_TESTPIN_0_0, BITPOS(PIN_TESTPIN_0_0));
  gpioSetPortDIROutput(GPIO, PORT_TESTPIN_1_0, BITPOS(PIN_TESTPIN_1_0));
  gpioPortClear(GPIO, PORT_TESTPIN_1_0, BITPOS(PIN_TESTPIN_1_0));
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_0_1, PIN_TESTPIN_0_1) == false);
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_1_1, PIN_TESTPIN_1_1) == false);
  gpioPortSet(GPIO, PORT_TESTPIN_0_0, BITPOS(PIN_TESTPIN_0_0));
  gpioPortSet(GPIO, PORT_TESTPIN_1_0, BITPOS(PIN_TESTPIN_1_0));
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_0_1, PIN_TESTPIN_0_1) == true);
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_1_1, PIN_TESTPIN_1_1) == true);
  ioconSetupPin(IOCON, IOCON_TESTPIN_0_1, IOCON_MODE(IOCON_MODE_PULLDOWN));
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_1, IOCON_MODE(IOCON_MODE_PULLDOWN));
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_0_1, PIN_TESTPIN_0_1) == false);
  minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_1_1, PIN_TESTPIN_1_1) == false);
}

/* Some tests that are still to do:
 * Test glitch filtering, depends on timer subsystem
 * Analog pin tests, depends on ADC/Comparator
 */