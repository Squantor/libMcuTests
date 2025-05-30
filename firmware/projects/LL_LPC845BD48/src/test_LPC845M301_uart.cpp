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
  minUnitCheck(LPC845M301TeardownCorrect() == true);
  swmPeriperhal.setup(testPin1, uartMainRxFunction);
  swmPeriperhal.setup(testPin2, uartMainTxFunction);
  sysconPeripheral.enablePeripheralClocks(libmcull::syscon::peripheralClocks0::UART0 | libmcull::syscon::peripheralClocks0::SWM |
                                            libmcull::syscon::peripheralClocks0::IOCON,
                                          0);
}

MINUNIT_ADD(LPC845M301UsartInit, LPC845M301SetupUsart, LPC845M301Teardown) {
  std::uint32_t realBaudRate;
  realBaudRate = usartPeripheral.Init<uart0ClockConfig>(115200);
  minUnitCheck(realBaudRate == 117187);
  minUnitCheck((dut_registers->CFG & CFG::kRESERVED_MASK) ==
               (CFG::kENABLE | CFG::kDATALEN8BIT | CFG::kPARITY_NONE | CFG::kSTOPBIT1));
  dut_registers->CFG = 0x00000000;
  realBaudRate = usartPeripheral.Init<uart0ClockConfig>(9600, UartParities::kEven, UartStops::kStop2, UartLengths::kSize7);
  minUnitCheck(realBaudRate == 9615);
  minUnitCheck((dut_registers->CFG & CFG::kRESERVED_MASK) ==
               (CFG::kENABLE | CFG::kDATALEN7BIT | CFG::kPARITY_EVEN | CFG::kSTOPBIT2));
}

MINUNIT_ADD(LPC845M301UsartComms, LPC845M301SetupUsart, LPC845M301Teardown) {
  std::uint32_t status;
  std::uint8_t data;
  int timeout;
  usartPeripheral.Init<uart0ClockConfig>(115200);
  sysconPeripheral.peripheralClockSource(libmcull::syscon::ClockSourceSelects::UART0, libmcull::syscon::clockSources::MAIN);
  minUnitCheck((dut_registers->STAT & STAT::kRESERVED_MASK) == 0x0000001E);
  minUnitCheck(usartPeripheral.Status() & UartStatuses::kTxReady);
  usartPeripheral.Write(0xA5);
  timeout = 0xFFFF;
  while (timeout > 0 && !(usartPeripheral.Status() & UartStatuses::kRxReady)) {
    timeout--;
  }
  minUnitCheck(timeout > 0);
  minUnitCheck(usartPeripheral.Status() & UartStatuses::kRxReady);
  usartPeripheral.Read(data);
  minUnitCheck(data == 0xA5);
  for (std::uint32_t i = 0; i < 256; i++) {
    usartPeripheral.Write(i);
    timeout = 0xFFFF;
    while (timeout > 0 && !(usartPeripheral.Status() & UartStatuses::kRxReady)) {
      timeout--;
    }
    minUnitCheck(timeout > 0);
    minUnitCheck(usartPeripheral.Status() & UartStatuses::kRxReady);
    usartPeripheral.Read(data, status);
    minUnitCheck(data == i);
    minUnitCheck(status == 0x00000000);
  }
}