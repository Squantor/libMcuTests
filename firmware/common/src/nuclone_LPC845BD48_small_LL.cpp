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
  // setup crystal oscillator
  sysconPeripheral.setSysOscControl(libMcuHw::syscon::SYSOSCCTRL::NO_BYPASS | libMcuHw::syscon::SYSOSCCTRL::FREQ_1_20MHz);
  sysconPeripheral.powerPeripherals(libMcuLL::syscon::powerOptions::SYSOSC);
  libMcuLL::delay(3000);
  // setup PLL
  sysconPeripheral.selectPllClock(libMcuLL::syscon::pllClockSources::EXT);
  sysconPeripheral.depowerPeripherals(libMcuLL::syscon::powerOptions::SYSPLL);
  sysconPeripheral.setSystemPllControl(4, libMcuLL::syscon::pllPostDivider::DIV_4);
  sysconPeripheral.powerPeripherals(libMcuLL::syscon::powerOptions::SYSPLL);
  while (sysconPeripheral.getSystemPllStatus() == 0)
    ;
  sysconPeripheral.setSystemClockDivider(2);
  // switch mainclock
  // sysconPeripheral.selectMainClock(libMcuLL::syscon::mainClockSources::EXT); // for selecting crystal oscillator
  sysconPeripheral.selectMainPllClock(libMcuLL::syscon::mainClockPllSources::SYSPLL);
  // setup clock out test pin
  swmPeriperhal.setup(testPin, clockOutFunction);
  // setup clock output
  sysconPeripheral.setClockOutput(libMcuLL::syscon::clockOutSources::MAIN, 10u);
}
