/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the LPC844M201 SPI peripheral polling mode
 */
#include <nuclone_LPC844M201BD64_tests.hpp>
#include <MinUnit.h>
#include <LPC844M201_teardown.hpp>
#include <common.hpp>

instances::spi::spi<peripherals::SPI0_cpp, instances::spi::spiChipEnables> testSpiPeripheral;

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC844M201CppSetupSpi) {
  minUnitCheck(LPC844M201TeardownCorrect() == true);
}

// TODO, do a more consistent teardown function while calling LPC844M201Teardown
MINUNIT_ADD(LPC844M201CppSpiRxTx, LPC844M201CppSetupSpi, LPC844M201Teardown) {
  sysconEnableClocks(SYSCON, CLKCTRL0_SPI0 | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
  sysconEnableResets(SYSCON, RESETCTRL0_SPI0 | RESETCTRL0_SWM | RESETCTRL0_IOCON, RESETCTRL1_NONE);
  sysconPeripheralClockSelect(SYSCON, SPI0CLKSEL, CLKSRC_MAIN);
  SwmMovablePinAssign(SWM, SWM_SPI0_SCK, SWM_TESTPIN_0_0);
  SwmMovablePinAssign(SWM, SWM_SPI0_MOSI, SWM_TESTPIN_1_0);
  SwmMovablePinAssign(SWM, SWM_SPI0_MISO, SWM_TESTPIN_1_1);
  SwmMovablePinAssign(SWM, SWM_SPI0_SSEL0, SWM_TESTPIN_2);
  __NOP();
  uint32_t actualDivider = testSpiPeripheral.init(999999u);
  minUnitCheck(actualDivider == 1000000u);
  minUnitCheck(SPI0->DIV == 11);
  minUnitCheck((spiSetGetStatus(SPI0, 0x0) & 0x10F) == 0x102);  // masking off relevant bits
  // test transfers
  std::array<uint16_t, 5> testDataSend{0x1234, 0x4567, 0x89AB, 0xCDEF, 0x5A5A};
  std::array<uint16_t, 5> testDataReceive;
  testDataReceive.fill(0x0000u);
  // test "simple"(below 17 bits) transaction
  testSpiPeripheral.transceive(instances::spi::spiChipEnables::SSEL0, testDataSend, testDataReceive, 8, true);
  minUnitCheck((testDataSend[0] & 0xFF) == testDataReceive[0]);
  // test "multi"(above 17 bits) transaction to test multi transfers
  testDataReceive.fill(0x0000u);
  testSpiPeripheral.transceive(instances::spi::spiChipEnables::SSEL0, testDataSend, testDataReceive, 24, true);
  minUnitCheck(testDataSend[0] == testDataReceive[0]);
  minUnitCheck((testDataSend[1] & 0xFF) == testDataReceive[1]);
  // test transmit, can only observe with a logic analyzer or a second SPI peripheral in slave mode (TODO)
  testSpiPeripheral.transmit(instances::spi::spiChipEnables::SSEL0, testDataSend, 36, false);
  testSpiPeripheral.transmit(instances::spi::spiChipEnables::SSEL0, testDataSend, 8, true);
  // test receive
  // disable MOSI
  SwmMovablePinAssign(SWM, SWM_SPI0_MOSI, SWM_PORTPIN_Reset);
  // enable pullup, read all ones
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_0, IOCON_MODE(IOCON_MODE_PULLUP));
  testSpiPeripheral.receive(instances::spi::spiChipEnables::SSEL0, testDataReceive, 12, true);
  minUnitCheck(0xFFFu == testDataReceive[0]);
  // enable pulldown, read all zeroes
  ioconSetupPin(IOCON, IOCON_TESTPIN_1_0, IOCON_MODE(IOCON_MODE_PULLDOWN));
  testDataReceive[0] = 0xFFFF;
  testSpiPeripheral.receive(instances::spi::spiChipEnables::SSEL0, testDataReceive, 12, true);
  minUnitCheck(0x0u == testDataReceive[0]);
  // undo setup
  SwmMovablePinAssign(SWM, SWM_SPI0_SSEL0, SWM_PORTPIN_Reset);
  SwmMovablePinAssign(SWM, SWM_SPI0_MOSI, SWM_PORTPIN_Reset);
  SwmMovablePinAssign(SWM, SWM_SPI0_MISO, SWM_PORTPIN_Reset);
  SwmMovablePinAssign(SWM, SWM_SPI0_SCK, SWM_PORTPIN_Reset);
  sysconDisableClocks(SYSCON, CLKCTRL0_I2C0 | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
}