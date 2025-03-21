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

namespace clocks = libMcuHw::clock;

libMcuLL::iocon::iocon<libMcuHw::ioconAddress> ioconPeripheral;
libMcuLL::swm::swm<libMcuHw::swmAddress> swmPeriperhal;
libMcuLL::gpio::gpio<libMcuHw::gpioAddress> gpioPeripheral;
libMcuLL::syscon::syscon<libMcuHw::sysconAddress> sysconPeripheral;
libmcull::usart::SyncUart<libmcuhw::kUsart0Address, std::uint8_t> usart_peripheral;
libmcuhal::usart::SyncUart<usart_peripheral> hal_usart_peripheral;

void boardInit(void) {
  // clock enables and resets
  sysconPeripheral.enablePeripheralClocks(libMcuLL::syscon::peripheralClocks0::SWM | libMcuLL::syscon::peripheralClocks0::IOCON |
                                            libMcuLL::syscon::peripheralClocks0::GPIO0 | libMcuLL::syscon::peripheralClocks0::GPIO1,
                                          0);
  // setup IOCON pins
  ioconPeripheral.setup(xtalInPin, libMcuLL::iocon::pullModes::INACTIVE);
  ioconPeripheral.setup(xtalOutPin, libMcuLL::iocon::pullModes::INACTIVE);
  swmPeriperhal.setup(xtalInPin, xtalInFunction);
  swmPeriperhal.setup(xtalOutPin, xtalOutFunction);
  // setup clock out test pin
  // swmPeriperhal.setup(testPin, clockOutFunction);
  // sysconPeripheral.setClockOutput(libMcuLL::syscon::clockOutSources::MAIN, 10u);
  sysconPeripheral.configureMcuClocks<nucloneClockConfig>();
  // switch mainclock
  // sysconPeripheral.selectMainClock(libMcuLL::syscon::mainClockSources::EXT); // for selecting crystal oscillator
}
