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

constexpr std::uint32_t timeout_value = 1000;

static constexpr libmcu::HwAddressType usart0_address = libmcuhw::Usart0Address;
libmcuhw::usart::Usart *const dut_registers{reinterpret_cast<libmcuhw::usart::Usart *>(usart0_address)};

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
  nvic_peripheral.Enable(libmcuhw::Interrupts::Uart0);
}
/**
 * @brief Tests interrupt driven USART initialisation
 */
MINUNIT_ADD(LPC845M301UsartIntInit, LPC845M301SetupUsartInt, LPC845M301Teardown) {
  std::uint32_t read_baud_rate;
  read_baud_rate = usart_interrupt_peripheral.Init<uart_0_clock_config>(115200);
  minUnitCheck(read_baud_rate == 117187);
  minUnitCheck((dut_registers->CFG & CFG::RESERVED_MASK) == (CFG::ENABLE | CFG::DATALEN8BIT | CFG::PARITY_NONE | CFG::STOPBIT1));
  dut_registers->CFG = 0x00000000;
  read_baud_rate =
    usart_interrupt_peripheral.Init<uart_0_clock_config>(9600, UartParities::Even, UartStops::Stop2, UartLengths::Size7);
  minUnitCheck(read_baud_rate == 9615);
  minUnitCheck((dut_registers->CFG & CFG::RESERVED_MASK) == (CFG::ENABLE | CFG::DATALEN7BIT | CFG::PARITY_EVEN | CFG::STOPBIT2));
}
/**
 * @brief Tests interrupt driven USART asynchronous locks
 */
MINUNIT_ADD(LPC845M301UsartIntAsyncLock, LPC845M301SetupUsartInt, LPC845M301Teardown) {
  minUnitCheck(usart_interrupt_peripheral.Init<uart_0_clock_config>(115200) == 117187);
  minUnitCheck(usart_interrupt_peripheral.GetStatus() == libmcu::Results::Idle);
  minUnitCheck(usart_interrupt_peripheral.Claim() == libmcu::Results::Claimed);
  minUnitCheck(usart_interrupt_peripheral.Claim() == libmcu::Results::InUse);
  minUnitCheck(usart_interrupt_peripheral.GetStatus() == libmcu::Results::Claimed);
  minUnitCheck(usart_interrupt_peripheral.Unclaim() == libmcu::Results::Unclaimed);
  minUnitCheck(usart_interrupt_peripheral.GetStatus() == libmcu::Results::Idle);
}
/**
 * @brief Tests interrupt driven USART transmission and reception
 */
MINUNIT_ADD(LPC845M301UsartIntTxRx, LPC845M301SetupUsartInt, LPC845M301Teardown) {
  uint8_t single_char = 0x73;
  std::uint32_t timeout_counter;
  std::array<std::uint8_t, 5> test_write_data{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 5> test_read_data{};
  std::array<std::uint8_t, 5> test_result_data{0x73, 0x88, 0x11, 0xAA, 0x55};
  minUnitCheck(usart_interrupt_peripheral.Init<uart_0_clock_config>(115200) == 117187);
  minUnitCheck(usart_interrupt_peripheral.Claim() == libmcu::Results::Claimed);
  minUnitCheck(usart_interrupt_peripheral.Transmit(single_char) == libmcu::Results::NoError);
  minUnitCheck(usart_interrupt_peripheral.Transmit(test_write_data) == libmcu::Results::NoError);
  timeout_counter = 0;
  while ((timeout_counter < timeout_value) && (usart_interrupt_peripheral.GetReceiveLevel() < (test_read_data.size() + 1))) {
    timeout_counter++;
  }
  minUnitCheck(timeout_counter < timeout_value);
  minUnitCheck(usart_interrupt_peripheral.Receive(test_read_data) == libmcu::Results::NoError);
  minUnitCheck(test_read_data == test_result_data);
  minUnitCheck(usart_interrupt_peripheral.GetReceiveLevel() == 1);
  minUnitCheck(usart_interrupt_peripheral.Receive(single_char) == libmcu::Results::NoError);
  minUnitCheck(single_char == 0xC5);
  minUnitCheck(usart_interrupt_peripheral.Unclaim() == libmcu::Results::Unclaimed);
}