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

static constexpr libmcu::HwAddressType usart0Address = libmcuhw::kUsart0Address;
libmcuhw::usart::Usart *const dut_registers{reinterpret_cast<libmcuhw::usart::Usart *>(usart0Address)};

/**
 * @brief USART setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupUsart) {
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
  swm_peripheral.Setup(test_pin_1, uart_main_rx_function);
  swm_peripheral.Setup(test_pin_2, uart_main_tx_function);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::peripheral_clocks_0::kUart0 |
                                             libmcull::syscon::peripheral_clocks_0::kSwm |
                                             libmcull::syscon::peripheral_clocks_0::kIocon,
                                           0);
}

MINUNIT_ADD(LPC845M301UsartInit, LPC845M301SetupUsart, LPC845M301Teardown) {
  std::uint32_t realBaudRate;
  realBaudRate = usart_polled_peripheral.Init<uart_0_clock_config>(115200);
  minUnitCheck(realBaudRate == 117187);
  minUnitCheck((dut_registers->CFG & CFG::RESERVED_MASK) ==
               (CFG::kENABLE | CFG::kDATALEN8BIT | CFG::kPARITY_NONE | CFG::kSTOPBIT1));
  dut_registers->CFG = 0x00000000;
  realBaudRate =
    usart_polled_peripheral.Init<uart_0_clock_config>(9600, UartParities::kEven, UartStops::kStop2, UartLengths::kSize7);
  minUnitCheck(realBaudRate == 9615);
  minUnitCheck((dut_registers->CFG & CFG::RESERVED_MASK) ==
               (CFG::kENABLE | CFG::kDATALEN7BIT | CFG::kPARITY_EVEN | CFG::kSTOPBIT2));
}

MINUNIT_ADD(LPC845M301UsartComms, LPC845M301SetupUsart, LPC845M301Teardown) {
  std::uint32_t status;
  std::uint8_t data;
  int timeout;
  usart_polled_peripheral.Init<uart_0_clock_config>(115200);
  syscon_peripheral.PeripheralClockSource(libmcull::syscon::ClockSourceSelects::kUart0, libmcull::syscon::ClockSources::kMain);
  minUnitCheck((dut_registers->STAT & STAT::RESERVED_MASK) == 0x0000001E);
  minUnitCheck(usart_polled_peripheral.Status() & UartStatuses::kTxReady);
  usart_polled_peripheral.Write(0xA5);
  timeout = 0xFFFF;
  while (timeout > 0 && !(usart_polled_peripheral.Status() & UartStatuses::kRxReady)) {
    timeout--;
  }
  minUnitCheck(timeout > 0);
  minUnitCheck(usart_polled_peripheral.Status() & UartStatuses::kRxReady);
  usart_polled_peripheral.Read(data);
  minUnitCheck(data == 0xA5);
  for (std::uint32_t i = 0; i < 256; i++) {
    usart_polled_peripheral.Write(i);
    timeout = 0xFFFF;
    while (timeout > 0 && !(usart_polled_peripheral.Status() & UartStatuses::kRxReady)) {
      timeout--;
    }
    minUnitCheck(timeout > 0);
    minUnitCheck(usart_polled_peripheral.Status() & UartStatuses::kRxReady);
    usart_polled_peripheral.Read(data, status);
    minUnitCheck(data == i);
    minUnitCheck(status == 0x00000000);
  }
}