/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file test_LPC845M301_uart_int.cpp
 * @brief tests for LPC845M301 interrupt driven HAL UART
 */
#include <nuclone_LPC845BD48_small_HAL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>

namespace hardware = libmcuhw::usart;
namespace lowlevel = libmcull::usart;
namespace hal = libmcuhal::usart;

constexpr std::uint32_t timeout_value = 1000;

static constexpr libmcuhw::HwAddressType usart0_address = libmcuhw::Usart0Address;
hardware::Usart *const dut_registers{reinterpret_cast<hardware::Usart *>(usart0_address)};

/**
 * @brief LPC845M301 HAL UART setup
 */
MINUNIT_SETUP(Lpc845m301SetupHalUartInt) {
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
 * @brief Tests interrupt driven HAL UART initialisation
 */
MINUNIT_ADD(Lpc845m301HalUartIntInit, Lpc845m301SetupHalUartInt, LPC845M301Teardown) {
  std::uint32_t realBaudRate;
  syscon_peripheral.PeripheralClockSource(libmcull::syscon::ClockSourceSelects::Uart0, libmcull::syscon::ClockSources::Main);
  realBaudRate = hal_usart_peripheral_int.Init<uart0_clock_config>(115200);
  minUnitCheck(realBaudRate == 117187);
  minUnitCheck((dut_registers->CFG & hardware::CFG::RESERVED_MASK) ==
               (hardware::CFG::ENABLE | hardware::CFG::DATALEN8BIT | hardware::CFG::PARITY_NONE | hardware::CFG::STOPBIT1));
  dut_registers->CFG = 0x00000000;
  realBaudRate = hal_usart_peripheral_int.Init<uart0_clock_config>(9600, hal::UartParities::Even, hal::UartStops::Stop2,
                                                                   hal::UartLengths::Size7);
  minUnitCheck(realBaudRate == 9615);
  minUnitCheck((dut_registers->CFG & hardware::CFG::RESERVED_MASK) ==
               (hardware::CFG::ENABLE | hardware::CFG::DATALEN7BIT | hardware::CFG::PARITY_EVEN | hardware::CFG::STOPBIT2));
}

/**
 * @brief Tests interrupt driven HAL UART transmit and receive
 */
MINUNIT_ADD(Lpc845m301HalUartIntTransmitReceive, Lpc845m301SetupHalUartInt, LPC845M301Teardown) {
  uint8_t single_char = 0x73;
  std::uint32_t timeout_counter;
  std::array<std::uint8_t, 5> test_write_data{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 5> test_read_data{};
  std::array<std::uint8_t, 5> test_result_data{0x73, 0x88, 0x11, 0xAA, 0x55};
  minUnitCheck(hal_usart_peripheral_int.Init<uart0_clock_config>(115200) == 117187);
  minUnitCheck(hal_usart_peripheral_int.Transmit(single_char) == libmcu::Results::NoError);
  minUnitCheck(hal_usart_peripheral_int.Transmit(test_write_data) == libmcu::Results::NoError);
  timeout_counter = 0;
  while ((timeout_counter < timeout_value) && (hal_usart_peripheral_int.GetReceiveLevel() < (test_read_data.size() + 1))) {
    timeout_counter++;
  }
  minUnitCheck(timeout_counter < timeout_value);
  minUnitCheck(hal_usart_peripheral_int.Receive(test_read_data) == libmcu::Results::NoError);
  minUnitCheck(test_read_data == test_result_data);
  minUnitCheck(hal_usart_peripheral_int.GetReceiveLevel() == 1);
  minUnitCheck(hal_usart_peripheral_int.Receive(single_char) == libmcu::Results::NoError);
  minUnitCheck(single_char == 0xC5);
}