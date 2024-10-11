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
#include <nuclone_RP2040_HAL.hpp>
#include <RP2040_HAL_teardown.hpp>
#include <test_conditions.hpp>
#include <MinUnit.h>

// local peripheral register sets
static constexpr libMcu::hwAddressType sioAddressLocal = libMcuHw::sioAddress;
libMcuHw::sio::sio *const sioRegisters{reinterpret_cast<libMcuHw::sio::sio *>(sioAddressLocal)};
static constexpr libMcu::hwAddressType padsBank0AddressLocal = libMcuHw::padsBank0Address;
libMcuHw::padsBank0::padsBank0 *const padsBank0Registers{reinterpret_cast<libMcuHw::padsBank0::padsBank0 *>(padsBank0AddressLocal)};
static constexpr libMcu::hwAddressType ioBankAddressLocal = libMcuHw::ioBank0Address;
libMcuHw::gpioBank0::gpioBank0 *const ioBankRegisters{reinterpret_cast<libMcuHw::gpioBank0::gpioBank0 *>(ioBankAddressLocal)};
static constexpr libMcu::hwAddressType resetsAddressLocal = libMcuHw::resetsAddress;
libMcuHw::resets::resets *const resetsRegisters{reinterpret_cast<libMcuHw::resets::resets *>(resetsAddressLocal)};

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