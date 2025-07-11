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

static constexpr libmcu::HwAddressType usart0Address = libmcuhw::kUsart0Address;
libmcuhw::usart::Usart *const dutRegisters{reinterpret_cast<libmcuhw::usart::Usart *>(usart0Address)};

/**
 * @brief USART setup and Initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupUsartSync) {
  minUnitCheck(Lpc812M101TeardownCorrect() == true);
  swm_peripheral.setup(test_0_pin, uart_main_rx_function);
  swm_peripheral.setup(test_1_pin, uart_main_tx_function);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::kClockUart0 |
                                           libmcull::syscon::PeripheralClocks::kClockSwm |
                                           libmcull::syscon::PeripheralClocks::kClockIocon);
}

MINUNIT_ADD(LPC812M101CppUsartSyncInit, LPC812M101CppSetupUsartSync, LPC812M101Teardown) {
  uint32_t realBaudRate;
  realBaudRate = usartAsyncPeripheral.Init(115200);
  minUnitCheck(realBaudRate == 117187);
  minUnitCheck((dutRegisters->CFG & CFG::RESERVED_MASK) ==
               (CFG::kENABLE | UartLengths::kSize8 | UartParities::kParityNone | UartStops::kStop1));
  dutRegisters->CFG = 0x00000000;
  realBaudRate = usartAsyncPeripheral.Init(9600, UartLengths::kSize7, UartParities::kParityEven, UartStops::kStop2);
  minUnitCheck(realBaudRate == 9615);
  minUnitCheck((dutRegisters->CFG & CFG::RESERVED_MASK) ==
               (CFG::kENABLE | UartLengths::kSize7 | UartParities::kParityEven | UartStops::kStop2));
}

MINUNIT_ADD(LPC812M101CppUsartSyncClaiming, LPC812M101CppSetupUsartSync, LPC812M101Teardown) {
  usartAsyncPeripheral.Init(115200);
  minUnitCheck(usartAsyncPeripheral.Claim() == libmcu::Results::Claimed);
  minUnitCheck(usartAsyncPeripheral.Claim() == libmcu::Results::InUse);
  minUnitCheck(usartAsyncPeripheral.Unclaim() == libmcu::Results::Unclaimed);
  minUnitCheck(usartAsyncPeripheral.Unclaim() == libmcu::Results::Error);
}

MINUNIT_ADD(LPC812M101CppUsartSyncComms, LPC812M101CppSetupUsartSync, LPC812M101Teardown) {
  std::array<std::uint8_t, 5> testDataSend{0x12, 0xFE, 0x34, 0xDC, 0x5A};
  std::array<std::uint8_t, 5> testDataReceive;
  testDataReceive.fill(0x0000u);
  swm_peripheral.setup(test_1_pin, uart_main_rx_function);
  swm_peripheral.setup(test_0_pin, uart_main_tx_function);
  syscon_peripheral.SetUsartClockDivider(1);
  usartAsyncPeripheral.Init(115200);
  minUnitCheck(usartAsyncPeripheral.Claim() == libmcu::Results::Claimed);
  minUnitCheck(usartAsyncPeripheral.Receive(testDataReceive) == libmcu::Results::Started);
  minUnitCheck(usartAsyncPeripheral.Receive(testDataReceive) == libmcu::Results::Error);
  minUnitCheck(usartAsyncPeripheral.Transmit(testDataSend) == libmcu::Results::Started);
  minUnitCheck(usartAsyncPeripheral.Transmit(testDataSend) == libmcu::Results::Error);
  libmcu::Results readResult = libmcu::Results::Busy;
  libmcu::Results writeResult = libmcu::Results::Busy;
  int timeout = 0;
  do {
    if (readResult == libmcu::Results::Busy)
      readResult = usartAsyncPeripheral.ProgressReceive();
    if (writeResult == libmcu::Results::Busy)
      writeResult = usartAsyncPeripheral.ProgressTransmit();
    timeout++;
  } while (timeout < 100000 && (writeResult == libmcu::Results::Busy || readResult == libmcu::Results::Busy));
  minUnitCheck(timeout < 100000);
  minUnitCheck(writeResult == libmcu::Results::Done);
  minUnitCheck(readResult == libmcu::Results::Done);
  minUnitCheck(testDataReceive[0] == 0x12);
  minUnitCheck(testDataReceive[1] == 0xFE);
  minUnitCheck(testDataReceive[2] == 0x34);
  minUnitCheck(testDataReceive[3] == 0xDC);
  minUnitCheck(testDataReceive[4] == 0x5A);
  minUnitCheck(usartAsyncPeripheral.Unclaim() == libmcu::Results::Unclaimed);
}