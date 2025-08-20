/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC845M301 UART peripheral polling mode
 */
#include <nuclone_LPC845BD48_small_LL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>
#include <common.hpp>

using namespace libmcuhw::usart;
using namespace libmcull::usart;

static constexpr libmcu::HwAddressType usart0_address = libmcuhw::Usart0Address;
libmcuhw::usart::Usart *const dut_registers{reinterpret_cast<libmcuhw::usart::Usart *>(usart0_address)};

/**
 * @brief USART setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupUsart) {
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::peripheral_clocks_0::Uart0 |
                                             libmcull::syscon::peripheral_clocks_0::Swm |
                                             libmcull::syscon::peripheral_clocks_0::Iocon,
                                           0);
  syscon_peripheral.PeripheralClockSource(libmcull::syscon::ClockSourceSelects::Uart0, libmcull::syscon::ClockSources::Main);
  swm_peripheral.Setup(test_pin_1, uart_main_rx_function);
  swm_peripheral.Setup(test_pin_2, uart_main_tx_function);
}

MINUNIT_ADD(LPC845M301UsartInit, LPC845M301SetupUsart, LPC845M301Teardown) {
  std::uint32_t realBaudRate;
  realBaudRate = usart_polled_peripheral.Init<uart_0_clock_config>(115200);
  minUnitCheck(realBaudRate == 117187);
  minUnitCheck((dut_registers->CFG & CFG::RESERVED_MASK) == (CFG::ENABLE | CFG::DATALEN8BIT | CFG::PARITY_NONE | CFG::STOPBIT1));
  dut_registers->CFG = 0x00000000;
  realBaudRate = usart_polled_peripheral.Init<uart_0_clock_config>(9600, UartParities::Even, UartStops::Stop2, UartLengths::Size7);
  minUnitCheck(realBaudRate == 9615);
  minUnitCheck((dut_registers->CFG & CFG::RESERVED_MASK) == (CFG::ENABLE | CFG::DATALEN7BIT | CFG::PARITY_EVEN | CFG::STOPBIT2));
}

MINUNIT_ADD(LPC845M301UsartComms, LPC845M301SetupUsart, LPC845M301Teardown) {
  std::uint32_t status;
  std::uint8_t data;
  int timeout;
  usart_polled_peripheral.Init<uart_0_clock_config>(115200);
  minUnitCheck((dut_registers->STAT & STAT::RESERVED_MASK) == 0x0000001E);
  minUnitCheck(usart_polled_peripheral.Status() & UartStatuses::TxReady);
  usart_polled_peripheral.Write(0xA5);
  timeout = 0xFFFF;
  while (timeout > 0 && !(usart_polled_peripheral.Status() & UartStatuses::RxReady)) {
    timeout--;
  }
  minUnitCheck(timeout > 0);
  minUnitCheck(usart_polled_peripheral.Status() & UartStatuses::RxReady);
  usart_polled_peripheral.Read(data);
  minUnitCheck(data == 0xA5);
  for (std::uint32_t i = 0; i < 256; i++) {
    usart_polled_peripheral.Write(i);
    timeout = 0xFFFF;
    while (timeout > 0 && !(usart_polled_peripheral.Status() & UartStatuses::RxReady)) {
      timeout--;
    }
    minUnitCheck(timeout > 0);
    minUnitCheck(usart_polled_peripheral.Status() & UartStatuses::RxReady);
    usart_polled_peripheral.Read(data, status);
    minUnitCheck(data == i);
    minUnitCheck(status == 0x00000000);
  }
}