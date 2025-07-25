/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC845M301 UART peripheral interrupt mode
 */
#include <nuclone_LPC845BD48_small_LL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>
#include <common.hpp>

using namespace libmcuhw::usart;
using namespace libmcull::usart;

static constexpr libmcu::HwAddressType usart0Address = libmcuhw::Usart0Address;
libmcuhw::usart::Usart *const dut_registers{reinterpret_cast<libmcuhw::usart::Usart *>(usart0Address)};

/**
 * @brief USART setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupUsartInt) {
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::peripheral_clocks_0::Uart0 |
                                             libmcull::syscon::peripheral_clocks_0::Swm |
                                             libmcull::syscon::peripheral_clocks_0::Iocon,
                                           0);
  syscon_peripheral.PeripheralClockSource(libmcull::syscon::ClockSourceSelects::Uart0, libmcull::syscon::ClockSources::Main);
  swm_peripheral.Setup(test_pin_1, uart_main_rx_function);
  swm_peripheral.Setup(test_pin_2, uart_main_tx_function);
}