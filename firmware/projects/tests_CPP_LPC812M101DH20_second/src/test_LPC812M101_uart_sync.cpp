/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC812M101 UART peripheral polling mode
 */
#include <nuclone_LPC812M101DH20_tests.hpp>
#include <MinUnit.h>
#include <LPC812M101_teardown.hpp>
#include <common.hpp>

using namespace libMcuLL::sw::usart;
using namespace libMcuLL::hw::usart;

static constexpr libMcuLL::hwAddressType usart0Address = libMcuLL::hw::usart0Address;
libMcuLL::hw::usart::peripheral *const dutRegisters{reinterpret_cast<libMcuLL::hw::usart::peripheral *>(usart0Address)};

/**
 * @brief USART setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupUsartSync) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  swmPeriperhal.setup(test0Pin, uartMainRxFunction);
  swmPeriperhal.setup(test1Pin, uartMainTxFunction);
  sysconPeripheral.enablePeripheralClocks(libMcuLL::sw::syscon::peripheralClocks::UART0 |
                                          libMcuLL::sw::syscon::peripheralClocks::SWM |
                                          libMcuLL::sw::syscon::peripheralClocks::IOCON);
}

MINUNIT_ADD(LPC812M101CppUsartSyncInit, LPC812M101CppSetupUsartSync, LPC812M101Teardown) {
  uint32_t realBaudRate;
  realBaudRate = usartAsyncPeripheral.init(115200);
  minUnitCheck(realBaudRate == 117187);
  minUnitCheck((dutRegisters->CFG & CFG::RESERVED_MASK) ==
               (CFG::ENABLE | uartLength::SIZE_8 | uartParity::NONE | uartStop::STOP_1));
  dutRegisters->CFG = 0x00000000;
  realBaudRate = usartAsyncPeripheral.init(9600, uartLength::SIZE_7, uartParity::EVEN, uartStop::STOP_2);
  minUnitCheck(realBaudRate == 9615);
  minUnitCheck((dutRegisters->CFG & CFG::RESERVED_MASK) ==
               (CFG::ENABLE | uartLength::SIZE_7 | uartParity::EVEN | uartStop::STOP_2));
}

MINUNIT_ADD(LPC812M101CppUsartSyncClaiming, LPC812M101CppSetupUsartSync, LPC812M101Teardown) {
  usartAsyncPeripheral.init(115200);
  minUnitCheck(usartAsyncPeripheral.claim() == libMcuLL::results::CLAIMED);
  minUnitCheck(usartAsyncPeripheral.claim() == libMcuLL::results::IN_USE);
  minUnitCheck(usartAsyncPeripheral.unclaim() == libMcuLL::results::UNCLAIMED);
  minUnitCheck(usartAsyncPeripheral.unclaim() == libMcuLL::results::ERROR);
}

MINUNIT_ADD(LPC812M101CppUsartSyncComms, LPC812M101CppSetupUsartSync, LPC812M101Teardown) {
  std::array<std::uint8_t, 5> testDataSend{0x12, 0xFE, 0x34, 0xDC, 0x5A};
  std::array<std::uint8_t, 5> testDataReceive;
  testDataReceive.fill(0x0000u);
  swmPeriperhal.setup(test1Pin, uartMainRxFunction);
  swmPeriperhal.setup(test0Pin, uartMainTxFunction);
  sysconPeripheral.setUsartClockDivider(1);
  usartAsyncPeripheral.init(115200);
  minUnitCheck(usartAsyncPeripheral.claim() == libMcuLL::results::CLAIMED);
  minUnitCheck(usartAsyncPeripheral.startRead(testDataReceive) == libMcuLL::results::STARTED);
  minUnitCheck(usartAsyncPeripheral.startRead(testDataReceive) == libMcuLL::results::ERROR);
  minUnitCheck(usartAsyncPeripheral.startWrite(testDataSend) == libMcuLL::results::STARTED);
  minUnitCheck(usartAsyncPeripheral.startWrite(testDataSend) == libMcuLL::results::ERROR);
  libMcuLL::results readResult = libMcuLL::results::BUSY;
  libMcuLL::results writeResult = libMcuLL::results::BUSY;
  int timeout = 0;
  do {
    if (readResult == libMcuLL::results::BUSY)
      readResult = usartAsyncPeripheral.progressRead();
    if (writeResult == libMcuLL::results::BUSY)
      writeResult = usartAsyncPeripheral.progressWrite();
    timeout++;
  } while (timeout < 100000 && (writeResult == libMcuLL::results::BUSY || readResult == libMcuLL::results::BUSY));
  minUnitCheck(timeout < 100000);
  minUnitCheck(writeResult == libMcuLL::results::DONE);
  minUnitCheck(readResult == libMcuLL::results::DONE);
  minUnitCheck(testDataReceive[0] == 0x12);
  minUnitCheck(testDataReceive[1] == 0xFE);
  minUnitCheck(testDataReceive[2] == 0x34);
  minUnitCheck(testDataReceive[3] == 0xDC);
  minUnitCheck(testDataReceive[4] == 0x5A);
  minUnitCheck(usartAsyncPeripheral.unclaim() == libMcuLL::results::UNCLAIMED);
}