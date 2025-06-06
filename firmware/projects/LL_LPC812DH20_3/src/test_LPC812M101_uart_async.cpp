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

using namespace libmcull::usart;
using namespace libmcuhw::usart;

static constexpr libmcu::HwAddressType usart0Address = libmcuhw::usart0Address;
libmcuhw::usart::Usart *const dutRegisters{reinterpret_cast<libmcuhw::usart::Usart *>(usart0Address)};

/**
 * @brief USART setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupUsartAsync) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  swmPeriperhal.setup(test0Pin, uartMainRxFunction);
  swmPeriperhal.setup(test1Pin, uartMainTxFunction);
  sysconPeripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::kClockUart0 |
                                          libmcull::syscon::PeripheralClocks::kClockSwm |
                                          libmcull::syscon::PeripheralClocks::kClockIocon);
}

MINUNIT_ADD(LPC812M101CppUsartAsyncInit, LPC812M101CppSetupUsartAsync, LPC812M101Teardown) {
  std::uint32_t realBaudRate;
  realBaudRate = usartSyncPeripheral.init(115200);
  minUnitCheck(realBaudRate == 117187);
  minUnitCheck((dutRegisters->CFG & CFG::kRESERVED_MASK) ==
               (CFG::kENABLE | UartLengths::kSize8 | UartParities::kParityNone | UartStops::kStop1));
  dutRegisters->CFG = 0x00000000;
  realBaudRate = usartSyncPeripheral.init(9600, UartLengths::kSize7, UartParities::kParityEven, UartStops::kStop2);
  minUnitCheck(realBaudRate == 9615);
  minUnitCheck((dutRegisters->CFG & CFG::kRESERVED_MASK) ==
               (CFG::kENABLE | UartLengths::kSize7 | UartParities::kParityEven | UartStops::kStop2));
}

MINUNIT_ADD(LPC812M101CppUsartAsyncComms, LPC812M101CppSetupUsartAsync, LPC812M101Teardown) {
  std::uint32_t status;
  std::uint8_t data;
  int timeout;
  usartSyncPeripheral.init(115200);
  sysconPeripheral.SetUsartClockDivider(1);
  minUnitCheck((dutRegisters->STAT & STAT::kRESERVED_MASK) == 0x0000000E);
  minUnitCheck(usartSyncPeripheral.status() & UartStatusMasks::kTxReady);
  usartSyncPeripheral.write(0xA5);
  timeout = 0xFFFF;
  while (timeout > 0 && !(usartSyncPeripheral.status() & UartStatusMasks::kRxReady)) {
    timeout--;
  }
  minUnitCheck(timeout > 0);
  minUnitCheck(usartSyncPeripheral.status() & UartStatusMasks::kRxReady);
  usartSyncPeripheral.read(data);
  minUnitCheck(data == 0xA5);
  for (std::uint32_t i = 0; i < 256; i++) {
    usartSyncPeripheral.write(i);
    timeout = 0xFFFF;
    while (timeout > 0 && !(usartSyncPeripheral.status() & UartStatusMasks::kRxReady)) {
      timeout--;
    }
    minUnitCheck(timeout > 0);
    minUnitCheck(usartSyncPeripheral.status() & UartStatusMasks::kRxReady);
    usartSyncPeripheral.read(data, status);
    minUnitCheck(data == i);
    minUnitCheck(status == 0x00000000);
  }
}