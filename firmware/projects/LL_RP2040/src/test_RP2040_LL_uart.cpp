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
}

MINUNIT_ADD(RP2040LLUartSetup, RP2040LLSetupUart, RP2040Teardown) {
  minUnitPass();
}
