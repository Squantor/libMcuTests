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

libMcuLL::iocon::iocon<libMcuHw::ioconAddress> ioconPeripheral;
libMcuLL::swm::swm<libMcuHw::swmAddress> swmPeriperhal;
libMcuLL::gpio::gpio<libMcuHw::gpioAddress> gpioPeripheral;
libMcuLL::syscon::syscon<libMcuHw::sysconAddress> sysconPeripheral;
libMcuLL::usart::usart<libMcuHw::usart0Address, std::uint8_t> usartPeripheral;
libMcuHw::clock::clockConfig<libMcuHw::clock::clockInputSources::XTAL, 12'000'000, 30'000'000> nucloneClockConfig;

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
  swmPeriperhal.setup(testPin, clockOutFunction);
  // setup clock output
  sysconPeripheral.setClockOutput(libMcuLL::syscon::clockOutSources::MAIN, 10u);
  libMcuHw::clock::configureClocks<sysconPeripheral, nucloneClockConfig>();
  // switch mainclock
  // sysconPeripheral.selectMainClock(libMcuLL::syscon::mainClockSources::EXT); // for selecting crystal oscillator
}
