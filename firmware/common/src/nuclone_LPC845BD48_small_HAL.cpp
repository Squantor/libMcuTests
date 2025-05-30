/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief code for LPC845 small nuclone Hal tester
 */
#include <nuclone_LPC845BD48_small_HAL.hpp>

namespace clocks = libmcuhw::clock;

libmcull::iocon::Iocon<libmcuhw::ioconAddress> ioconPeripheral;
libmcull::swm::Swm<libmcuhw::swmAddress> swmPeriperhal;
libmcull::gpio::gpio<libmcuhw::gpioAddress> gpioPeripheral;
libmcull::syscon::Syscon<libmcuhw::sysconAddress> sysconPeripheral;
libmcull::usart::SyncUart<libmcuhw::kUsart0Address, UartTransferType> usart_peripheral;
libmcuhal::usart::SyncUart<usart_peripheral, UartTransferType> hal_usart_peripheral;

void boardInit(void) {
  // clock enables and resets
  sysconPeripheral.enablePeripheralClocks(libmcull::syscon::peripheralClocks0::SWM | libmcull::syscon::peripheralClocks0::IOCON |
                                            libmcull::syscon::peripheralClocks0::GPIO0 | libmcull::syscon::peripheralClocks0::GPIO1,
                                          0);
  // setup IOCON pins
  ioconPeripheral.setup(xtalInPin, libmcull::iocon::pullModes::INACTIVE);
  ioconPeripheral.setup(xtalOutPin, libmcull::iocon::pullModes::INACTIVE);
  swmPeriperhal.setup(xtalInPin, xtalInFunction);
  swmPeriperhal.setup(xtalOutPin, xtalOutFunction);
  // setup clock out test pin
  // swmPeriperhal.setup(testPin, clockOutFunction);
  // sysconPeripheral.setClockOutput(libmcull::syscon::clockOutSources::MAIN, 10u);
  sysconPeripheral.configureMcuClocks<nucloneClockConfig>();
  // switch mainclock
  // sysconPeripheral.selectMainClock(libmcull::syscon::mainClockSources::EXT); // for selecting crystal oscillator
}
