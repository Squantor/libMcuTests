/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the LPC844M201 UART peripheral polling mode
 */
#include <nuclone_LPC844M201BD64_tests.hpp>
#include <MinUnit.h>
#include <LPC844M201_teardown.hpp>
#include <common.hpp>

/**
 * @brief UART setup and initialisation
 */
MINUNIT_SETUP(LPC844M201SetupUsart)
{
    minUnitCheck(LPC844M201TeardownCorrect() == true);
}

MINUNIT_ADD(LPC844M201UsartTxRx, LPC844M201SetupUsart, LPC844M201Teardown)
{
    sysconEnableClocks(SYSCON, CLKCTRL0_UART0 | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
    sysconEnableResets(SYSCON, RESETCTRL0_UART0 | RESETCTRL0_SWM | RESETCTRL0_IOCON, RESETCTRL1_NONE);
    sysconPeripheralClockSelect(SYSCON, UART0CLKSEL, CLKSRC_MAIN);
    SwmMovablePinAssign(SWM, SWM_USART0_TXD, SWM_TESTPIN_0_0);
    SwmMovablePinAssign(SWM, SWM_USART0_RXD, SWM_TESTPIN_0_1);
    // make sure we use a baud rate that is not an exact match
    minUnitCheck(usartSetBaud(USART0, CLOCK_MAIN, 9600) == 9615);
    usartSetConfig(USART0, DATALEN_8, PARITY_NONE, STOPLEN_1, 0);
    usartTXEnable(USART0);
    usartSendData(USART0, 0xA5);
    // loop until we got something
    int i = 0;
    while(i < 100000 && !(usartGetStatus(USART0) & USART_STAT_RXRDY))
        i++;
    minUnitCheck(i < 100000);
    minUnitCheck(usartReadData(USART0) == 0xA5);
    usartTXDisable(USART0);
    usartDisable(USART0);
    SwmMovablePinAssign(SWM, SWM_USART0_TXD, SWM_PORTPIN_Reset);
    SwmMovablePinAssign(SWM, SWM_USART0_RXD, SWM_PORTPIN_Reset);
    sysconDisableClocks(SYSCON, CLKCTRL0_UART0 | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
}