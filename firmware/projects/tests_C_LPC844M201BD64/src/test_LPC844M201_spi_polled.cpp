/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the LPC844M201 SPI peripheral polling mode
 */
#include <nuclone_LPC844M201BD64_tests.hpp>
#include <MinUnit.h>
#include <LPC844M201_teardown.hpp>
#include <common.hpp>

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC844M201SetupSpi)
{
    minUnitCheck(LPC844M201TeardownCorrect() == true);
}

MINUNIT_ADD(LPC844M201SpiRxTx, LPC844M201SetupSpi, LPC844M201Teardown)
{
    sysconEnableClocks(SYSCON, CLKCTRL0_SPI0 | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
    sysconEnableResets(SYSCON, RESETCTRL0_SPI0 | RESETCTRL0_SWM | RESETCTRL0_IOCON, RESETCTRL1_NONE);
    sysconPeripheralClockSelect(SYSCON, SPI0CLKSEL, CLKSRC_MAIN);
    SwmMovablePinAssign(SWM, SWM_SPI0_SCK, SWM_TESTPIN_0_0);
    SwmMovablePinAssign(SWM, SWM_SPI0_MOSI, SWM_TESTPIN_1_0);
    SwmMovablePinAssign(SWM, SWM_SPI0_MISO, SWM_TESTPIN_1_1);
    SwmMovablePinAssign(SWM, SWM_SPI0_SSEL0, SWM_TESTPIN_2);
    spiSetDelays(SPI0, SPI_DLY_PRE(1) | SPI_DLY_POST(2) | SPI_DLY_FRAME(3) | SPI_DLY_TRANSFER(4));
    minUnitCheck(SPI0->DLY == 0x00004321);
    spiSetDivider(SPI0, 2);
    minUnitCheck(SPI0->DIV == 0x02);
    spiSetConfig(SPI0, SPI_CFG_ENABLE | SPI_CFG_MASTER);
    // masking off some bits that are a pain to get rid off
    minUnitCheck((spiSetGetStatus(SPI0, 0x0) & 0x10F) == 0x102);
    // test 16 bit transfer with receive
    spiSetTxCtrlData(SPI0,  SPI_TXDATCTL_TXDAT(0xA55A) | 
                            SPI_TXDATCTL_TXSSEL0 | 
                            SPI_TXDATCTL_EOF | 
                            SPI_TXDATCTL_LEN(16) );
    int i = 0;
    while(i < 10000 && !(spiSetGetStatus(SPI0, 0x0) & SPI_STAT_RXRDY))
        i++;
    uint32_t rxData = spiGetRxData(SPI0);
    minUnitCheck(SPI_RXDAT_DATA(rxData) == 0xA55A);
    minUnitCheck((rxData & 0xF0000) == SPI_TXDATCTL_TXSSEL0);
    // small transfer without received data
    spiSetTxCtrlData(SPI0,  SPI_TXDATCTL_TXDAT(0x5) | 
                            SPI_TXDATCTL_TXSSEL0 | 
                            SPI_TXDATCTL_EOF | 
                            SPI_TXDATCTL_EOT | 
                            SPI_TXDATCTL_RXIGNORE |
                            SPI_TXDATCTL_LEN(4) );
    minUnitCheck((spiSetGetStatus(SPI0, 0x0) & SPI_STAT_RXRDY) == 0);
    SwmMovablePinAssign(SWM, SWM_SPI0_SSEL0, SWM_PORTPIN_Reset);
    SwmMovablePinAssign(SWM, SWM_SPI0_MOSI, SWM_PORTPIN_Reset);
    SwmMovablePinAssign(SWM, SWM_SPI0_MISO, SWM_PORTPIN_Reset);
    SwmMovablePinAssign(SWM, SWM_SPI0_SCK, SWM_PORTPIN_Reset);
    sysconDisableClocks(SYSCON, CLKCTRL0_I2C0 | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
}