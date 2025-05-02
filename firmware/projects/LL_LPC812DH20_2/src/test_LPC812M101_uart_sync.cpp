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

using namespace libmcull::sw::usart;
using namespace libmcuhw::usart;

static constexpr libmcu::hwAddressType usart0Address = libmcuhw::usart0Address;
libmcuhw::usart::Usart *const dutRegisters{reinterpret_cast<libmcuhw::usart::Usart *>(usart0Address)};

/**
 * @brief USART setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupUsartSync) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  swmPeriperhal.setup(test0Pin, uartMainRxFunction);
  swmPeriperhal.setup(test1Pin, uartMainTxFunction);
  sysconPeripheral.enablePeripheralClocks(libmcull::sw::syscon::peripheralClocks::UART0 |
                                          libmcull::sw::syscon::peripheralClocks::SWM |
                                          libmcull::sw::syscon::peripheralClocks::IOCON);
}

MINUNIT_ADD(LPC812M101CppUsartSyncInit, LPC812M101CppSetupUsartSync, LPC812M101Teardown) {
  uint32_t realBaudRate;
  realBaudRate = usartAsyncPeripheral.init(115200);
  minUnitCheck(realBaudRate == 117187);
  minUnitCheck((dutRegisters->CFG & CFG::kRESERVED_MASK) ==
               (CFG::kENABLE | uartLength::SIZE_8 | uartParity::NONE | uartStop::STOP_1));
  dutRegisters->CFG = 0x00000000;
  realBaudRate = usartAsyncPeripheral.init(9600, uartLength::SIZE_7, uartParity::EVEN, uartStop::STOP_2);
  minUnitCheck(realBaudRate == 9615);
  minUnitCheck((dutRegisters->CFG & CFG::kRESERVED_MASK) ==
               (CFG::kENABLE | uartLength::SIZE_7 | uartParity::EVEN | uartStop::STOP_2));
}

MINUNIT_ADD(LPC812M101CppUsartSyncClaiming, LPC812M101CppSetupUsartSync, LPC812M101Teardown) {
  usartAsyncPeripheral.init(115200);
  minUnitCheck(usartAsyncPeripheral.claim() == libmcu::Results::CLAIMED);
  minUnitCheck(usartAsyncPeripheral.claim() == libmcu::Results::IN_USE);
  minUnitCheck(usartAsyncPeripheral.unclaim() == libmcu::Results::UNCLAIMED);
  minUnitCheck(usartAsyncPeripheral.unclaim() == libmcu::Results::ERROR);
}

MINUNIT_ADD(LPC812M101CppUsartSyncComms, LPC812M101CppSetupUsartSync, LPC812M101Teardown) {
  std::array<std::uint8_t, 5> testDataSend{0x12, 0xFE, 0x34, 0xDC, 0x5A};
  std::array<std::uint8_t, 5> testDataReceive;
  testDataReceive.fill(0x0000u);
  swmPeriperhal.setup(test1Pin, uartMainRxFunction);
  swmPeriperhal.setup(test0Pin, uartMainTxFunction);
  sysconPeripheral.setUsartClockDivider(1);
  usartAsyncPeripheral.init(115200);
  minUnitCheck(usartAsyncPeripheral.claim() == libmcu::Results::CLAIMED);
  minUnitCheck(usartAsyncPeripheral.startRead(testDataReceive) == libmcu::Results::STARTED);
  minUnitCheck(usartAsyncPeripheral.startRead(testDataReceive) == libmcu::Results::ERROR);
  minUnitCheck(usartAsyncPeripheral.startWrite(testDataSend) == libmcu::Results::STARTED);
  minUnitCheck(usartAsyncPeripheral.startWrite(testDataSend) == libmcu::Results::ERROR);
  libmcu::Results readResult = libmcu::Results::BUSY;
  libmcu::Results writeResult = libmcu::Results::BUSY;
  int timeout = 0;
  do {
    if (readResult == libmcu::Results::BUSY)
      readResult = usartAsyncPeripheral.progressRead();
    if (writeResult == libmcu::Results::BUSY)
      writeResult = usartAsyncPeripheral.progressWrite();
    timeout++;
  } while (timeout < 100000 && (writeResult == libmcu::Results::BUSY || readResult == libmcu::Results::BUSY));
  minUnitCheck(timeout < 100000);
  minUnitCheck(writeResult == libmcu::Results::DONE);
  minUnitCheck(readResult == libmcu::Results::DONE);
  minUnitCheck(testDataReceive[0] == 0x12);
  minUnitCheck(testDataReceive[1] == 0xFE);
  minUnitCheck(testDataReceive[2] == 0x34);
  minUnitCheck(testDataReceive[3] == 0xDC);
  minUnitCheck(testDataReceive[4] == 0x5A);
  minUnitCheck(usartAsyncPeripheral.unclaim() == libmcu::Results::UNCLAIMED);
}