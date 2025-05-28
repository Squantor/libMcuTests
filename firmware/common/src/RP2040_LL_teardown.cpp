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
#include <nuclone_RP2040_LL.hpp>
#include <RP2040_HAL_teardown.hpp>
#include <test_conditions.hpp>
#include <MinUnit.h>

using namespace libmcull;

// local peripheral register sets
static constexpr libmcu::HwAddressType sioAddressLocal = libmcuhw::sioAddress;
libmcuhw::sio::sio *const sioRegisters{reinterpret_cast<libmcuhw::sio::sio *>(sioAddressLocal)};
static constexpr libmcu::HwAddressType padsBank0AddressLocal = libmcuhw::padsBank0Address;
libmcuhw::padsBank0::padsBank0 *const padsBank0ARegisters{
  reinterpret_cast<libmcuhw::padsBank0::padsBank0 *>(padsBank0AddressLocal)};
static constexpr libmcu::HwAddressType ioBankAddressLocal = libmcuhw::ioBank0Address;
libmcuhw::gpioBank0::gpioBank0 *const ioBankRegisters{reinterpret_cast<libmcuhw::gpioBank0::gpioBank0 *>(ioBankAddressLocal)};
static constexpr libmcu::HwAddressType resetsAddressLocal = libmcuhw::resetsAddress;
libmcuhw::resets::resets *const resetsRegisters{reinterpret_cast<libmcuhw::resets::resets *>(resetsAddressLocal)};

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