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
#include <minunit.h>
#include <LPC812M101_teardown.hpp>
#include <common.hpp>

using namespace libmcull::usart;
using namespace libmcuhw::usart;

static constexpr libmcu::HwAddressType usart0Address = libmcuhw::Usart0Address;
libmcuhw::usart::Usart *const dutRegisters{reinterpret_cast<libmcuhw::usart::Usart *>(usart0Address)};

/**
 * @brief USART setup and Initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupUsartSync) {
  MINUNIT_CHECK(Lpc812M101TeardownCorrect() == true);
  swm_peripheral.setup(test_0_pin, uart_main_rx_function);
  swm_peripheral.setup(test_1_pin, uart_main_tx_function);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::ClockUart0 |
                                           libmcull::syscon::PeripheralClocks::ClockSwm |
                                           libmcull::syscon::PeripheralClocks::ClockIocon);
}

MINUNIT_ADD(LPC812M101CppUsartSyncInit, LPC812M101CppSetupUsartSync, LPC812M101Teardown) {
  uint32_t realBaudRate;
  realBaudRate = usartAsyncPeripheral.Init(115200);
  MINUNIT_CHECK(realBaudRate == 117187);
  MINUNIT_CHECK((dutRegisters->CFG & CFG::RESERVED_MASK) ==
                (CFG::ENABLE | UartLengths::Size8 | UartParities::ParityNone | UartStops::Stop1));
  dutRegisters->CFG = 0x00000000;
  realBaudRate = usartAsyncPeripheral.Init(9600, UartLengths::Size7, UartParities::ParityEven, UartStops::Stop2);
  MINUNIT_CHECK(realBaudRate == 9615);
  MINUNIT_CHECK((dutRegisters->CFG & CFG::RESERVED_MASK) ==
                (CFG::ENABLE | UartLengths::Size7 | UartParities::ParityEven | UartStops::Stop2));
}

MINUNIT_ADD(LPC812M101CppUsartSyncClaiming, LPC812M101CppSetupUsartSync, LPC812M101Teardown) {
  usartAsyncPeripheral.Init(115200);
  MINUNIT_CHECK(usartAsyncPeripheral.Claim() == libmcu::Results::Claimed);
  MINUNIT_CHECK(usartAsyncPeripheral.Claim() == libmcu::Results::InUse);
  MINUNIT_CHECK(usartAsyncPeripheral.Unclaim() == libmcu::Results::Unclaimed);
  MINUNIT_CHECK(usartAsyncPeripheral.Unclaim() == libmcu::Results::Error);
}

MINUNIT_ADD(LPC812M101CppUsartSyncComms, LPC812M101CppSetupUsartSync, LPC812M101Teardown) {
  std::array<std::uint8_t, 5> testDataSend{0x12, 0xFE, 0x34, 0xDC, 0x5A};
  std::array<std::uint8_t, 5> testDataReceive;
  testDataReceive.fill(0x0000u);
  swm_peripheral.setup(test_1_pin, uart_main_rx_function);
  swm_peripheral.setup(test_0_pin, uart_main_tx_function);
  syscon_peripheral.SetUsartClockDivider(1);
  usartAsyncPeripheral.Init(115200);
  MINUNIT_CHECK(usartAsyncPeripheral.Claim() == libmcu::Results::Claimed);
  MINUNIT_CHECK(usartAsyncPeripheral.Receive(testDataReceive) == libmcu::Results::Started);
  MINUNIT_CHECK(usartAsyncPeripheral.Receive(testDataReceive) == libmcu::Results::Error);
  MINUNIT_CHECK(usartAsyncPeripheral.Transmit(testDataSend) == libmcu::Results::Started);
  MINUNIT_CHECK(usartAsyncPeripheral.Transmit(testDataSend) == libmcu::Results::Error);
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
  MINUNIT_CHECK(timeout < 100000);
  MINUNIT_CHECK(writeResult == libmcu::Results::Done);
  MINUNIT_CHECK(readResult == libmcu::Results::Done);
  MINUNIT_CHECK(testDataReceive[0] == 0x12);
  MINUNIT_CHECK(testDataReceive[1] == 0xFE);
  MINUNIT_CHECK(testDataReceive[2] == 0x34);
  MINUNIT_CHECK(testDataReceive[3] == 0xDC);
  MINUNIT_CHECK(testDataReceive[4] == 0x5A);
  MINUNIT_CHECK(usartAsyncPeripheral.Unclaim() == libmcu::Results::Unclaimed);
}