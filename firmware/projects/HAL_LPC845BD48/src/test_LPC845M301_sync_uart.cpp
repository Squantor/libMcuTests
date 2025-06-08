/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file test_LPC845M301_sync_uart.cpp
 * @brief tests for LPC845M301 HAL synchronous UART
 */

#include <nuclone_LPC845BD48_small_HAL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>

using namespace libmcuhw::usart;
using namespace libmcuhal::usart;

static constexpr libmcuhw::HwAddressType usart0_address = libmcuhw::kUsart0Address;
libmcuhw::usart::Usart *const dut_registers{reinterpret_cast<libmcuhw::usart::Usart *>(usart0_address)};

/**
 * @brief LPC845M301 HAL UART setup
 */
MINUNIT_SETUP(LPC845M301SetupUart) {
  minUnitCheck(LPC845M301TeardownCorrect() == true);
  sysconPeripheral.EnablePeripheralClocks(libmcull::syscon::peripheralClocks0::UART0 | libmcull::syscon::peripheralClocks0::SWM |
                                            libmcull::syscon::peripheralClocks0::IOCON,
                                          0);
  swmPeriperhal.setup(testPin1, uartMainRxFunction);
  swmPeriperhal.setup(testPin2, uartMainTxFunction);
}

/**
 * @brief Tests UART HAL init functionality
 */
MINUNIT_ADD(LPC845M301SyncUartInit, LPC845M301SetupUart, LPC845M301Teardown) {
  std::uint32_t realBaudRate;
  realBaudRate = hal_usart_peripheral.Init<uart0ClockConfig>(115200);
  minUnitCheck(realBaudRate == 117187);
  minUnitCheck((dut_registers->CFG & CFG::kRESERVED_MASK) ==
               (CFG::kENABLE | CFG::kDATALEN8BIT | CFG::kPARITY_NONE | CFG::kSTOPBIT1));
  dut_registers->CFG = 0x00000000;
  realBaudRate = hal_usart_peripheral.Init<uart0ClockConfig>(9600, UartParities::kEven, UartStops::kStop2, UartLengths::kSize7);
  minUnitCheck(realBaudRate == 9615);
  minUnitCheck((dut_registers->CFG & CFG::kRESERVED_MASK) ==
               (CFG::kENABLE | CFG::kDATALEN7BIT | CFG::kPARITY_EVEN | CFG::kSTOPBIT2));
}