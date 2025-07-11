/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file test_LPC845M301_uart.cpp
 * @brief tests for LPC845M301 HAL UART
 * @todo add tests for multiple element transfers
 */

#include <nuclone_LPC845BD48_small_HAL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>

namespace hardware = libmcuhw::usart;
namespace lowlevel = libmcull::usart;
namespace hal = libmcuhal::usart;

static constexpr libmcuhw::HwAddressType usart0_address = libmcuhw::kUsart0Address;
hardware::Usart *const dut_registers{reinterpret_cast<hardware::Usart *>(usart0_address)};

/**
 * @brief LPC845M301 HAL UART setup
 */
MINUNIT_SETUP(LPC845M301SetupUart) {
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::peripheral_clocks_0::Uart0 |
                                             libmcull::syscon::peripheral_clocks_0::Swm |
                                             libmcull::syscon::peripheral_clocks_0::Iocon,
                                           0);
  swm_peripheral.Setup(test_pin_1, uart_main_rx_function);
  swm_peripheral.Setup(test_pin_2, uart_main_tx_function);
}

/**
 * @brief Tests UART HAL init functionality
 */
MINUNIT_ADD(LPC845M301SyncUartInit, LPC845M301SetupUart, LPC845M301Teardown) {
  std::uint32_t realBaudRate;
  syscon_peripheral.PeripheralClockSource(libmcull::syscon::ClockSourceSelects::Uart0, libmcull::syscon::ClockSources::Main);
  realBaudRate = hal_usart_peripheral.Init<uart_0_clock_config>(115200);
  minUnitCheck(realBaudRate == 117187);
  minUnitCheck((dut_registers->CFG & hardware::CFG::RESERVED_MASK) ==
               (hardware::CFG::ENABLE | hardware::CFG::DATALEN8BIT | hardware::CFG::PARITY_NONE | hardware::CFG::STOPBIT1));
  dut_registers->CFG = 0x00000000;
  realBaudRate = hal_usart_peripheral.Init<uart_0_clock_config>(9600, hal::UartParities::kEven, hal::UartStops::kStop2,
                                                                hal::UartLengths::kSize7);
  minUnitCheck(realBaudRate == 9615);
  minUnitCheck((dut_registers->CFG & hardware::CFG::RESERVED_MASK) ==
               (hardware::CFG::ENABLE | hardware::CFG::DATALEN7BIT | hardware::CFG::PARITY_EVEN | hardware::CFG::STOPBIT2));
}

/**
 * @brief Tests UART HAL claim and unclaim functionality
 */
MINUNIT_ADD(LPC845M301SyncUartClaimUnclaim, LPC845M301SetupUart, LPC845M301Teardown) {
  libmcuhal::AsyncHandle handle_first = 42;
  libmcuhal::AsyncHandle handle_second = 44;

  minUnitCheck(hal_usart_peripheral.Claim(handle_first) == libmcu::Results::Claimed);
  minUnitCheck(handle_first == 0);
  minUnitCheck(hal_usart_peripheral.Claim(handle_second) == libmcu::Results::InUse);
  minUnitCheck(hal_usart_peripheral.Release(handle_first) == libmcu::Results::Unclaimed);
  minUnitCheck(hal_usart_peripheral.Release(handle_first) == libmcu::Results::NotClaimed);
  minUnitCheck(hal_usart_peripheral.Claim(handle_second) == libmcu::Results::Claimed);
  minUnitCheck(handle_second == 1);
  minUnitCheck(hal_usart_peripheral.Release(handle_second) == libmcu::Results::Unclaimed);
}

/**
 * @brief Tests UART HAL single character communication
 */
MINUNIT_ADD(LPC845M301SyncUartComms, LPC845M301SetupUart, LPC845M301Teardown) {
  syscon_peripheral.PeripheralClockSource(libmcull::syscon::ClockSourceSelects::Uart0, libmcull::syscon::ClockSources::Main);
  hal_usart_peripheral.Init<uart_0_clock_config>(115200);

  std::size_t timeout;
  UartTransferType single_transfer = 0xA5;
  UartTransferType single_reception;
  hal::UartStateMasks status = hal_usart_peripheral.GetStatus();
  minUnitCheck((status & hal::UartStateMasks::kReceiverIdleMask) && (status & hal::UartStateMasks::kTransmitIdleMask) &&
               (status & hal::UartStateMasks::kTransmitDataMask));
  libmcuhal::AsyncHandle handle = 42;
  minUnitCheck(hal_usart_peripheral.Transmit(42, single_transfer) == libmcu::Results::NotClaimed);
  minUnitCheck(hal_usart_peripheral.Claim(handle) == libmcu::Results::Claimed);
  minUnitCheck(handle != 42);
  minUnitCheck(hal_usart_peripheral.Transmit(42, single_transfer) == libmcu::Results::InvalidHandle);
  minUnitCheck(hal_usart_peripheral.Transmit(handle, single_transfer) == libmcu::Results::NoError);
  // check for reception
  timeout = 0xFFFF;
  while (timeout > 0 && !(hal_usart_peripheral.GetStatus() & hal::UartStateMasks::kReceiverDataMask)) {
    timeout--;
  }
  minUnitCheck(timeout > 0);
  minUnitCheck(hal_usart_peripheral.GetStatus() & hal::UartStateMasks::kReceiverDataMask);
  // receive and check for timeout
  minUnitCheck(hal_usart_peripheral.Receive(handle, single_reception) == libmcu::Results::NoError);
  minUnitCheck(single_reception == single_transfer);
  minUnitCheck(hal_usart_peripheral.Receive(handle, single_reception) == libmcu::Results::Timeout);

  minUnitCheck(hal_usart_peripheral.Release(handle) == libmcu::Results::Unclaimed);
  minUnitPass();
}