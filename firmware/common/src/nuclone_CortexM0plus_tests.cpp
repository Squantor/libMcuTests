/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#include <nuclone_CortexM0plus_tests.hpp>

libmcull::systick::systick<libmcuhw::systickAddress> systickPeripheral;
libmcull::nvic::nvic<libmcuhw::nvicAddress, libmcuhw::scbAddress> nvicPeripheral;
libmcull::scb::scb<libmcuhw::scbAddress> scbPeripheral;

void boardInit(void) {
  systickPeripheral.systickPeripheral()->CSR = 0UL;
  systickPeripheral.systickPeripheral()->RVR = 0UL;
  systickPeripheral.systickPeripheral()->CVR = 0UL;
  nvicPeripheral.nvicPeripheral()->ICER[0] = 0xFFFFFFFFUL;
  nvicPeripheral.nvicPeripheral()->ICPR[0] = 0xFFFFFFFFUL;
}
