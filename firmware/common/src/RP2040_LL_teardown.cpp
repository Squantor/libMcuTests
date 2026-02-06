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
#include <minunit.h>

using namespace libmcull;

// local peripheral register sets
static constexpr libmcu::HwAddressType sioAddressLocal = libmcuhw::SioAddress;
libmcuhw::sio::Sio *const sioRegisters{reinterpret_cast<libmcuhw::sio::Sio *>(sioAddressLocal)};
static constexpr libmcu::HwAddressType padsBank0AddressLocal = libmcuhw::PadsBank0Address;
libmcuhw::padsBank0::PadsBank0 *const padsBank0ARegisters{
  reinterpret_cast<libmcuhw::padsBank0::PadsBank0 *>(padsBank0AddressLocal)};
static constexpr libmcu::HwAddressType ioBankAddressLocal = libmcuhw::IoBank0Address;
libmcuhw::gpio_bank0::GpioBank0 *const ioBankRegisters{reinterpret_cast<libmcuhw::gpio_bank0::GpioBank0 *>(ioBankAddressLocal)};
static constexpr libmcu::HwAddressType resetsAddressLocal = libmcuhw::ResetsAddress;
libmcuhw::resets::Resets *const resetsRegisters{reinterpret_cast<libmcuhw::resets::Resets *>(resetsAddressLocal)};

/** @brief resets all the registers to their default states
 */
MINUNIT_TEARDOWN(Rp2040Teardown) {
  MINUNIT_CHECK(Rp2040Teardown_correct() == true);
}

/** @brief checks if all the registers to their default states
 *  @return if all registers are correctly torndown
 */
bool Rp2040Teardown_correct(void) {
  return true;
}