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

using namespace libMcuHw::usart;
using namespace libMcuLL::usart;

static constexpr libMcu::hwAddressType usart0Address = libMcuHw::usart0Address;
libMcuHw::usart::usart *const dut_registers{reinterpret_cast<libMcuHw::usart::usart *>(usart0Address)};

/**
 * @brief USART setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupUsart) {
  minUnitCheck(LPC845M301TeardownCorrect() == true);
  swmPeriperhal.setup(testPin1, uartMainRxFunction);
  swmPeriperhal.setup(testPin2, uartMainTxFunction);
  sysconPeripheral.enablePeripheralClocks(libMcuLL::syscon::peripheralClocks0::UART0 | libMcuLL::syscon::peripheralClocks0::SWM |
                                            libMcuLL::syscon::peripheralClocks0::IOCON,
                                          0);
}

MINUNIT_ADD(LPC845M301UsartInit, LPC845M301SetupUsart, LPC845M301Teardown) {
  std::uint32_t realBaudRate;
  realBaudRate = usartPeripheral.init<uart0ClockConfig>(115200);
  minUnitCheck(realBaudRate == 117187);
  minUnitCheck((dut_registers->CFG & CFG::RESERVED_MASK) == (CFG::ENABLE | CFG::DATALEN8BIT | CFG::PARITY_NONE | CFG::STOPBIT1));
  dut_registers->CFG = 0x00000000;
  realBaudRate = usartPeripheral.init<uart0ClockConfig>(9600, uartLength::SIZE_7, uartParity::EVEN, uartStop::STOP_2);
  minUnitCheck(realBaudRate == 9615);
  minUnitCheck((dut_registers->CFG & CFG::RESERVED_MASK) == (CFG::ENABLE | CFG::DATALEN7BIT | CFG::PARITY_EVEN | CFG::STOPBIT2));
}

MINUNIT_ADD(LPC845M301UsartComms, LPC845M301SetupUsart, LPC845M301Teardown) {
  std::uint32_t status;
  std::uint8_t data;
  int timeout;
  usartPeripheral.init<uart0ClockConfig>(115200);
  sysconPeripheral.peripheralClockSource(libMcuLL::syscon::clockSourceSelects::UART0, libMcuLL::syscon::clockSources::MAIN);
  minUnitCheck((dut_registers->STAT & STAT::RESERVED_MASK) == 0x0000001E);
  minUnitCheck(usartPeripheral.status() & uartStatus::TXRDY);
  usartPeripheral.write(0xA5);
  timeout = 0xFFFF;
  while (timeout > 0 && !(usartPeripheral.status() & uartStatus::RXRDY)) {
    timeout--;
  }
  minUnitCheck(timeout > 0);
  minUnitCheck(usartPeripheral.status() & uartStatus::RXRDY);
  usartPeripheral.read(data);
  minUnitCheck(data == 0xA5);
  for (std::uint32_t i = 0; i < 256; i++) {
    usartPeripheral.write(i);
    timeout = 0xFFFF;
    while (timeout > 0 && !(usartPeripheral.status() & uartStatus::RXRDY)) {
      timeout--;
    }
    minUnitCheck(timeout > 0);
    minUnitCheck(usartPeripheral.status() & uartStatus::RXRDY);
    usartPeripheral.read(data, status);
    minUnitCheck(data == i);
    minUnitCheck(status == 0x00000000);
  }
}