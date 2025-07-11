/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 UART
 */
#include <nuclone_RP2040_LL.hpp>
#include <MinUnit.h>
#include <RP2040_LL_teardown.hpp>
#include <common.hpp>

namespace hardware = libmcuhw::uart;

// peripheral register sets
static constexpr libmcu::HwAddressType uart0Address = libmcuhw::kUart0Address;
libmcuhw::uart::Uart *const uart0Registers{reinterpret_cast<libmcuhw::uart::Uart *>(uart0Address)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040LLSetupUart) {
  minUnitCheck(Rp2040Teardown_correct() == true);
  resets_peripheral.Reset(libmcull::resets::kIoBank0 | libmcull::resets::kPadsBank0 | libmcull::resets::kUart0, 100000);
  // connect all GPIO's
  gpio_bank0_peripheral.Setup(uartRxPin);
  pads_bank0_peripheral.Setup(uartRxPin, libmcull::pads::DriveModes::k8mA, libmcull::pads::PullModes::kPullUp, true, true);
  gpio_bank0_peripheral.Setup(uartTxPin);
  pads_bank0_peripheral.Setup(uartTxPin, libmcull::pads::DriveModes::k8mA, libmcull::pads::PullModes::kNone, false, false);
}

MINUNIT_ADD(RP2040LLUartSetup, RP2040LLSetupUart, Rp2040Teardown) {
  // test baud rate edge conditions
  minUnitCheck(uart_polled_peripheral.Setup(7800000) == 7500000);
  minUnitCheck(uart0Registers->UARTIBRD == 1);
  minUnitCheck(uart_polled_peripheral.Setup(100) == 114);
  minUnitCheck(uart0Registers->UARTIBRD == 65535);
  // test all common baud rates and their divisors
  minUnitCheck(uart_polled_peripheral.Setup(300) == 300);
  // check control registers and divisors and such
  minUnitCheck(uart_polled_peripheral.Setup(1200) == 1200);
  // check control registers and divisors and such
  minUnitCheck(uart_polled_peripheral.Setup(4800) == 4800);
  // check control registers and divisors and such
  minUnitCheck(uart_polled_peripheral.Setup(9600) == 9600);
  // check control registers and divisors and such
  minUnitCheck(uart_polled_peripheral.Setup(38400) == 38400);
  // check control registers and divisors and such
  minUnitCheck(uart_polled_peripheral.Setup(115200) == 115190);
  // check control registers and divisors and such
  minUnitCheck(uart_polled_peripheral.Setup(1000000) == 1000000);
  // check control registers and divisors and such
}

MINUNIT_ADD(RP2040LLUartComms, RP2040LLSetupUart, Rp2040Teardown) {
  std::array<std::uint8_t, 1> singleData{0x5A};
  std::array<std::uint8_t, 5> multiData{0x01, 0x23, 0x45, 0x67, 0xA5};
  std::array<std::uint8_t, 5> receiveDataMulti;
  std::array<std::uint8_t, 1> receiveDataSingle;
  minUnitCheck(uart_polled_peripheral.Setup(9600) == 9600);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::TXFE_FLAG);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::RXFE_FLAG);
  minUnitCheck(uart_polled_peripheral.Receive(receiveDataSingle, 100) == libmcu::Results::Timeout);
  uart_polled_peripheral.Transmit(singleData);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::TXFE_FLAG);
  libmcu::Delay(1000000);                                                 // wait until data has been received
  minUnitCheck(!(uart0Registers->UARTFR & hardware::UARTFR::RXFE_FLAG));  // we looped TX and RX so RX FIFO should also not be empty
  minUnitCheck(uart_polled_peripheral.Receive(receiveDataSingle, 100) == libmcu::Results::NoError);
  minUnitCheck(receiveDataSingle[0] == 0x5A);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::TXFE_FLAG);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::RXFE_FLAG);
  uart_polled_peripheral.Transmit(multiData);
  minUnitCheck(uart_polled_peripheral.Receive(receiveDataMulti, 10000) == libmcu::Results::NoError);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::TXFE_FLAG);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::RXFE_FLAG);
  minUnitCheck(receiveDataMulti[0] == 0x01);
  minUnitCheck(receiveDataMulti[1] == 0x23);
  minUnitCheck(receiveDataMulti[2] == 0x45);
  minUnitCheck(receiveDataMulti[3] == 0x67);
  minUnitCheck(receiveDataMulti[4] == 0xA5);
}
