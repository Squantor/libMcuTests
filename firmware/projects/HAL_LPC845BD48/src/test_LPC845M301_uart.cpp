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
  minUnitCheck(LPC845M301TeardownCorrect() == true);
  sysconPeripheral.enablePeripheralClocks(libmcull::syscon::peripheralClocks0::UART0 | libmcull::syscon::peripheralClocks0::SWM |
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
  sysconPeripheral.peripheralClockSource(libmcull::syscon::ClockSourceSelects::UART0, libmcull::syscon::clockSources::MAIN);
  realBaudRate = hal_usart_peripheral.Init<uart0ClockConfig>(115200);
  minUnitCheck(realBaudRate == 117187);
  minUnitCheck((dut_registers->CFG & hardware::CFG::kRESERVED_MASK) ==
               (hardware::CFG::kENABLE | hardware::CFG::kDATALEN8BIT | hardware::CFG::kPARITY_NONE | hardware::CFG::kSTOPBIT1));
  dut_registers->CFG = 0x00000000;
  realBaudRate =
    hal_usart_peripheral.Init<uart0ClockConfig>(9600, hal::UartParities::kEven, hal::UartStops::kStop2, hal::UartLengths::kSize7);
  minUnitCheck(realBaudRate == 9615);
  minUnitCheck((dut_registers->CFG & hardware::CFG::kRESERVED_MASK) ==
               (hardware::CFG::kENABLE | hardware::CFG::kDATALEN7BIT | hardware::CFG::kPARITY_EVEN | hardware::CFG::kSTOPBIT2));
}

/**
 * @brief Tests UART HAL claim and unclaim functionality
 */
MINUNIT_ADD(LPC845M301SyncUartClaimUnclaim, LPC845M301SetupUart, LPC845M301Teardown) {
  libmcuhal::AsyncHandle handle_first = 42;
  libmcuhal::AsyncHandle handle_second = 44;

  minUnitCheck(hal_usart_peripheral.Claim(handle_first) == libmcu::Results::kClaimed);
  minUnitCheck(handle_first == 0);
  minUnitCheck(hal_usart_peripheral.Claim(handle_second) == libmcu::Results::kInUse);
  minUnitCheck(hal_usart_peripheral.Release(handle_first) == libmcu::Results::kUnclaimed);
  minUnitCheck(hal_usart_peripheral.Release(handle_first) == libmcu::Results::kNotClaimed);
  minUnitCheck(hal_usart_peripheral.Claim(handle_second) == libmcu::Results::kClaimed);
  minUnitCheck(handle_second == 1);
  minUnitCheck(hal_usart_peripheral.Release(handle_second) == libmcu::Results::kUnclaimed);
}

/**
 * @brief Tests UART HAL single character communication
 */
MINUNIT_ADD(LPC845M301SyncUartComms, LPC845M301SetupUart, LPC845M301Teardown) {
  sysconPeripheral.peripheralClockSource(libmcull::syscon::ClockSourceSelects::UART0, libmcull::syscon::clockSources::MAIN);
  hal_usart_peripheral.Init<uart0ClockConfig>(115200);

  std::size_t timeout;
  UartTransferType single_transfer = 0xA5;
  UartTransferType single_reception;
  hal::UartStateMasks status = hal_usart_peripheral.GetStatus();
  minUnitCheck((status & hal::UartStateMasks::kReceiverIdleMask) && (status & hal::UartStateMasks::kTransmitIdleMask) &&
               (status & hal::UartStateMasks::kTransmitDataMask));
  libmcuhal::AsyncHandle handle = 42;
  minUnitCheck(hal_usart_peripheral.Transmit(42, single_transfer) == libmcu::Results::kNotClaimed);
  minUnitCheck(hal_usart_peripheral.Claim(handle) == libmcu::Results::kClaimed);
  minUnitCheck(handle != 42);
  minUnitCheck(hal_usart_peripheral.Transmit(42, single_transfer) == libmcu::Results::kInvalidHandle);
  minUnitCheck(hal_usart_peripheral.Transmit(handle, single_transfer) == libmcu::Results::kNoError);
  // check for reception
  timeout = 0xFFFF;
  while (timeout > 0 && !(hal_usart_peripheral.GetStatus() & hal::UartStateMasks::kReceiverDataMask)) {
    timeout--;
  }
  minUnitCheck(timeout > 0);
  minUnitCheck(hal_usart_peripheral.GetStatus() & hal::UartStateMasks::kReceiverDataMask);
  // receive and check for timeout
  minUnitCheck(hal_usart_peripheral.Receive(handle, single_reception) == libmcu::Results::kNoError);
  minUnitCheck(single_reception == single_transfer);
  minUnitCheck(hal_usart_peripheral.Receive(handle, single_reception) == libmcu::Results::kTimeout);

  minUnitCheck(hal_usart_peripheral.Release(handle) == libmcu::Results::kUnclaimed);
  minUnitPass();
}