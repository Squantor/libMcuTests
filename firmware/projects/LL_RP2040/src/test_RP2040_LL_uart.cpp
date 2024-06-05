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

using namespace libMcuLL;
using namespace libMcuLL::sw::uart;

// peripheral register sets
static constexpr hwAddressType uart0Address = hw::uart0Address;
hw::uart::peripheral *const uart0Registers{reinterpret_cast<hw::uart::peripheral *>(uart0Address)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040LLSetupUart) {
  minUnitCheck(RP2040TeardownCorrect() == true);
  resetsPeripheral.reset(sw::resets::IO_BANK0 | sw::resets::PADS_BANK0 | sw::resets::UART0, 100000);
  // connect all GPIO's
  gpioBank0Peripheral.setup(uartRxPin);
  padsBank0Peripheral.setup(uartRxPin, sw::pads::driveModes::DRIVE_8MA, true, false, true, true);
  gpioBank0Peripheral.setup(uartTxPin);
  padsBank0Peripheral.setup(uartTxPin, sw::pads::driveModes::DRIVE_8MA, false, false, false, false);
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
