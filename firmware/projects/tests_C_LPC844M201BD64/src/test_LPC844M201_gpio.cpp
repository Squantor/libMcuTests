/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the LPC844M201 GPIO0/1 peripheral
 */
#include <nuclone_LPC844M201BD64_tests.hpp>
#include <MinUnit.h>
#include <LPC844M201_teardown.hpp>
#include <common.hpp>

MINUNIT_SETUP(LPC844M201SetupGpio)
{
    minUnitCheck(LPC844M201TeardownCorrect() == true); // check if the MCU is in its reset state
    sysconEnableClocks(SYSCON, CLKCTRL0_GPIO0 | CLKCTRL0_GPIO1, CLKCTRL1_NONE);
    sysconEnableResets(SYSCON, RESETCTRL0_GPIO0, 0x00);
}

MINUNIT_ADD(LPC844M201GpioPin, LPC844M201SetupGpio, LPC844M201Teardown)
{
    gpioSetPinDIRInput(GPIO, PORT_TESTPIN_0_1, PIN_TESTPIN_0_1);
    gpioSetPinDIROutput(GPIO, PORT_TESTPIN_0_0, PIN_TESTPIN_0_0);
    gpioPinWrite(GPIO, PORT_TESTPIN_0_0, PIN_TESTPIN_0_0, 0);
    minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_0_1, PIN_TESTPIN_0_1) == 0);
    gpioPinWrite(GPIO, PORT_TESTPIN_0_0, PIN_TESTPIN_0_0, 1);
    minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_0_1, PIN_TESTPIN_0_1) != 0);
    // switch around
    gpioSetPinDIRInput(GPIO, PORT_TESTPIN_0_0, PIN_TESTPIN_0_0);
    gpioSetPinDIROutput(GPIO, PORT_TESTPIN_0_1, PIN_TESTPIN_0_1);
    gpioPinWrite(GPIO, PORT_TESTPIN_0_1, PIN_TESTPIN_0_1, 0);
    minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_0_0, PIN_TESTPIN_0_0) == 0);
    gpioPinWrite(GPIO, PORT_TESTPIN_0_1, PIN_TESTPIN_0_1, 1);
    minUnitCheck(gpioPinRead(GPIO, PORT_TESTPIN_0_0, PIN_TESTPIN_0_0) != 0);
}

// same tests as pins but checking a whole port
MINUNIT_ADD(LPC844M201GpioPort, LPC844M201SetupGpio, LPC844M201Teardown)
{
    // setup port0 as outputs and set it low
    gpioSetPortDir(GPIO, PORT_TESTPIN_0_0, BITPOS(PIN_TESTPIN_0_0) | BITPOS(PIN_TESTPIN_1_0));
    gpioPortWrite(GPIO, PORT_TESTPIN_0_0, gpioPortRead(GPIO, PORT_TESTPIN_0_0) & ~BITPOS(PIN_TESTPIN_0_0) & ~BITPOS(PIN_TESTPIN_1_0));
    // sense low levels on pits
    minUnitCheck((gpioPortRead(GPIO, 
        PORT_TESTPIN_0_0) & (BITPOS(PIN_TESTPIN_0_0) | BITPOS(PIN_TESTPIN_1_0))) == 0x00000000);
    // set output as high 
    gpioPortWrite(GPIO, PORT_TESTPIN_0_0, gpioPortRead(GPIO, PORT_TESTPIN_0_0) | BITPOS(PIN_TESTPIN_0_0) | BITPOS(PIN_TESTPIN_1_0));
    // sense if inputs are high
    minUnitCheck((gpioPortRead(GPIO, 
        PORT_TESTPIN_0_0) & (BITPOS(PIN_TESTPIN_0_0) | BITPOS(PIN_TESTPIN_1_0))) == (BITPOS(PIN_TESTPIN_0_0) | BITPOS(PIN_TESTPIN_1_0)));
    // switch around pins
    gpioSetPortDir(GPIO, PORT_TESTPIN_0_0, 0x00000000);
    gpioSetPortDir(GPIO, PORT_TESTPIN_0_1, BITPOS(PIN_TESTPIN_0_1) | BITPOS(PIN_TESTPIN_1_1));
    gpioPortWrite(GPIO, PORT_TESTPIN_0_1, gpioPortRead(GPIO, PORT_TESTPIN_0_1) & ~BITPOS(PIN_TESTPIN_0_1) & ~BITPOS(PIN_TESTPIN_1_1));
    // sense low levels on pits
    minUnitCheck((gpioPortRead(GPIO, 
        PORT_TESTPIN_0_1) & (BITPOS(PIN_TESTPIN_0_1) | BITPOS(PIN_TESTPIN_1_1))) == 0x00000000);
    // set output as high 
    gpioPortWrite(GPIO, PORT_TESTPIN_0_1, gpioPortRead(GPIO, PORT_TESTPIN_0_1) | BITPOS(PIN_TESTPIN_0_1) | BITPOS(PIN_TESTPIN_1_1));
    // sense if inputs are high
    minUnitCheck((gpioPortRead(GPIO, 
        PORT_TESTPIN_0_1) & (BITPOS(PIN_TESTPIN_0_1) | BITPOS(PIN_TESTPIN_1_1))) == (BITPOS(PIN_TESTPIN_0_1) | BITPOS(PIN_TESTPIN_1_1)));
}