/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#include <nuclone_CortexM0plus_tests.hpp>

libmcull::systick::Systick<libmcuhw::SystickAddress> systick_peripheral;
libmcull::nvic::Nvic<libmcuhw::NvicAddress, libmcuhw::ScbAddress> nvic_peripheral;
libmcull::scb::Scb<libmcuhw::ScbAddress> scb_peripheral;

void board_init(void) {
  systick_peripheral.GetPeripheral()->CSR = 0UL;
  systick_peripheral.GetPeripheral()->RVR = 0UL;
  systick_peripheral.GetPeripheral()->CVR = 0UL;
  nvic_peripheral.GetPeripheral()->ICER[0] = 0xFFFFFFFFUL;
  nvic_peripheral.GetPeripheral()->ICPR[0] = 0xFFFFFFFFUL;
}
