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
 * @brief USART setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupUsartAsync) {
  MINUNIT_CHECK(Lpc812M101TeardownCorrect() == true);
  swm_peripheral.setup(test_0_pin, uart_main_rx_function);
  swm_peripheral.setup(test_1_pin, uart_main_tx_function);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::ClockUart0 |
                                           libmcull::syscon::PeripheralClocks::ClockSwm |
                                           libmcull::syscon::PeripheralClocks::ClockIocon);
}

MINUNIT_ADD(LPC812M101CppUsartAsyncInit, LPC812M101CppSetupUsartAsync, LPC812M101Teardown) {
  std::uint32_t realBaudRate;
  realBaudRate = usart_polled_peripheral.Init(115200);
  MINUNIT_CHECK(realBaudRate == 117187);
  MINUNIT_CHECK((dutRegisters->CFG & CFG::RESERVED_MASK) ==
                (CFG::ENABLE | UartLengths::Size8 | UartParities::ParityNone | UartStops::Stop1));
  dutRegisters->CFG = 0x00000000;
  realBaudRate = usart_polled_peripheral.Init(9600, UartLengths::Size7, UartParities::ParityEven, UartStops::Stop2);
  MINUNIT_CHECK(realBaudRate == 9615);
  MINUNIT_CHECK((dutRegisters->CFG & CFG::RESERVED_MASK) ==
                (CFG::ENABLE | UartLengths::Size7 | UartParities::ParityEven | UartStops::Stop2));
}

MINUNIT_ADD(LPC812M101CppUsartAsyncComms, LPC812M101CppSetupUsartAsync, LPC812M101Teardown) {
  std::uint32_t status;
  std::uint8_t data;
  int timeout;
  usart_polled_peripheral.Init(115200);
  syscon_peripheral.SetUsartClockDivider(1);
  MINUNIT_CHECK((dutRegisters->STAT & STAT::RESERVED_MASK) == 0x0000000E);
  MINUNIT_CHECK(usart_polled_peripheral.Status() & UartStatusMasks::TxReady);
  usart_polled_peripheral.Transmit(0xA5);
  timeout = 0xFFFF;
  while (timeout > 0 && !(usart_polled_peripheral.Status() & UartStatusMasks::RxReady)) {
    timeout--;
  }
  MINUNIT_CHECK(timeout > 0);
  MINUNIT_CHECK(usart_polled_peripheral.Status() & UartStatusMasks::RxReady);
  usart_polled_peripheral.Receive(data);
  MINUNIT_CHECK(data == 0xA5);
  for (std::uint32_t i = 0; i < 256; i++) {
    usart_polled_peripheral.Transmit(i);
    timeout = 0xFFFF;
    while (timeout > 0 && !(usart_polled_peripheral.Status() & UartStatusMasks::RxReady)) {
      timeout--;
    }
    MINUNIT_CHECK(timeout > 0);
    MINUNIT_CHECK(usart_polled_peripheral.Status() & UartStatusMasks::RxReady);
    usart_polled_peripheral.Receive(data, status);
    MINUNIT_CHECK(data == i);
    MINUNIT_CHECK(status == 0x00000000);
  }
}