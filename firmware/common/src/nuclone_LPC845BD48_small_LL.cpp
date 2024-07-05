/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief code for LPC845 small nuclone LL tester
 */
#include <nuclone_LPC845BD48_small_LL.hpp>

libMcu::ll::iocon::iocon<libMcu::hw::ioconAddress> ioconPeripheral;
libMcu::ll::swm::swm<libMcu::hw::swmAddress> swmPeriperhal;
libMcu::ll::gpio::gpio<libMcu::hw::gpioAddress> gpioPeripheral;
libMcu::ll::syscon::syscon<libMcu::hw::sysconAddress> sysconPeripheral;

void boardInit(void) {
  // clock enables and resets
  sysconPeripheral.enablePeripheralClocks(libMcu::ll::syscon::SWM_CLOCK | libMcu::ll::syscon::IOCON_CLOCK |
                                            libMcu::ll::syscon::GPIO0_CLOCK | libMcu::ll::syscon::GPIO1_CLOCK,
                                          0);
  // setup IOCON pins
  ioconPeripheral.setup(xtalInPin, libMcu::ll::iocon::pullModes::INACTIVE);
  ioconPeripheral.setup(xtalOutPin, libMcu::ll::iocon::pullModes::INACTIVE);
  swmPeriperhal.setup(xtalInPin, xtalInFunction);
  swmPeriperhal.setup(xtalOutPin, xtalOutFunction);
}
