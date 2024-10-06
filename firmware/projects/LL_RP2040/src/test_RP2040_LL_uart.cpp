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

namespace hardware = libMcuHw::uart;

// peripheral register sets
static constexpr libMcu::hwAddressType uart0Address = libMcuHw::uart0Address;
libMcuHw::uart::uart *const uart0Registers{reinterpret_cast<libMcuHw::uart::uart *>(uart0Address)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040LLSetupUart) {
  minUnitCheck(RP2040TeardownCorrect() == true);
  resetsPeripheral.reset(libMcuLL::resets::IO_BANK0 | libMcuLL::resets::PADS_BANK0 | libMcuLL::resets::UART0, 100000);
  // connect all GPIO's
  gpioBank0Peripheral.setup(uartRxPin);
  padsBank0Peripheral.setup(uartRxPin, libMcuLL::pads::driveModes::DRIVE_8MA, true, false, true, true);
  gpioBank0Peripheral.setup(uartTxPin);
  padsBank0Peripheral.setup(uartTxPin, libMcuLL::pads::driveModes::DRIVE_8MA, false, false, false, false);
}

MINUNIT_ADD(RP2040LLUartSetup, RP2040LLSetupUart, RP2040Teardown) {
  // test baud rate edge conditions
  minUnitCheck(uartPeripheral.setup(7800000) == 7500000);
  minUnitCheck(uart0Registers->UARTIBRD == 1);
  minUnitCheck(uartPeripheral.setup(100) == 114);
  minUnitCheck(uart0Registers->UARTIBRD == 65535);
  // test all common baud rates and their divisors
  minUnitCheck(uartPeripheral.setup(300) == 300);
  // check control registers and divisors and such
  minUnitCheck(uartPeripheral.setup(1200) == 1200);
  // check control registers and divisors and such
  minUnitCheck(uartPeripheral.setup(4800) == 4800);
  // check control registers and divisors and such
  minUnitCheck(uartPeripheral.setup(9600) == 9600);
  // check control registers and divisors and such
  minUnitCheck(uartPeripheral.setup(38400) == 38400);
  // check control registers and divisors and such
  minUnitCheck(uartPeripheral.setup(115200) == 115190);
  // check control registers and divisors and such
  minUnitCheck(uartPeripheral.setup(1000000) == 1000000);
  // check control registers and divisors and such
}

MINUNIT_ADD(RP2040LLUartComms, RP2040LLSetupUart, RP2040Teardown) {
  std::array<std::uint8_t, 1> singleData{0x5A};
  std::array<std::uint8_t, 5> multiData{0x01, 0x23, 0x45, 0x67, 0xA5};
  std::array<std::uint8_t, 5> receiveDataMulti;
  std::array<std::uint8_t, 1> receiveDataSingle;
  minUnitCheck(uartPeripheral.setup(9600) == 9600);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::TXFE_FLAG);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::RXFE_FLAG);
  minUnitCheck(uartPeripheral.read(receiveDataSingle, 100) == libMcu::results::TIMEOUT);
  uartPeripheral.write(singleData);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::TXFE_FLAG);
  libMcuLL::delay(1000000);                                               // wait until data has been received
  minUnitCheck(!(uart0Registers->UARTFR & hardware::UARTFR::RXFE_FLAG));  // we looped TX and RX so RX FIFO should also not be empty
  minUnitCheck(uartPeripheral.read(receiveDataSingle, 100) == libMcu::results::NO_ERROR);
  minUnitCheck(receiveDataSingle[0] == 0x5A);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::TXFE_FLAG);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::RXFE_FLAG);
  uartPeripheral.write(multiData);
  minUnitCheck(uartPeripheral.read(receiveDataMulti, 10000) == libMcu::results::NO_ERROR);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::TXFE_FLAG);
  minUnitCheck(uart0Registers->UARTFR & hardware::UARTFR::RXFE_FLAG);
  minUnitCheck(receiveDataMulti[0] == 0x01);
  minUnitCheck(receiveDataMulti[1] == 0x23);
  minUnitCheck(receiveDataMulti[2] == 0x45);
  minUnitCheck(receiveDataMulti[3] == 0x67);
  minUnitCheck(receiveDataMulti[4] == 0xA5);
}
