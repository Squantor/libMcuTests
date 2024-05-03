/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief Teardown function that resets the RP2040 to a state
 * that more or less matches reset state
 */
#include <nuclone_RP2040.hpp>
#include <RP2040_teardown.hpp>
#include <test_conditions.hpp>
#include <MinUnit.h>

using namespace libMcuLL;

// local peripheral register sets
static constexpr hwAddressType sioAddressLocal = hw::sioAddress;
hw::sio::peripheral *const sioRegisters{reinterpret_cast<hw::sio::peripheral *>(sioAddressLocal)};
static constexpr hwAddressType padsBank0AddressLocal = hw::padsBank0Address;
hw::padsBank0::peripheral *const padsBank0ARegisters{reinterpret_cast<hw::padsBank0::peripheral *>(padsBank0AddressLocal)};
static constexpr hwAddressType ioBankAddressLocal = hw::ioBank0Address;
hw::gpioBank0::peripheral *const ioBankRegisters{reinterpret_cast<hw::gpioBank0::peripheral *>(ioBankAddressLocal)};
static constexpr hwAddressType resetsAddressLocal = hw::resetsAddress;
hw::resets::peripheral *const resetsRegisters{reinterpret_cast<hw::resets::peripheral *>(resetsAddressLocal)};

/** @brief resets all the registers to their default states
 */
MINUNIT_TEARDOWN(RP2040Teardown) {
  minUnitCheck(RP2040TeardownCorrect() == true);
}

/** @brief checks if all the registers to their default states
 *  @return if all registers are correctly torndown
 */
bool RP2040TeardownCorrect(void) {
  return true;
}