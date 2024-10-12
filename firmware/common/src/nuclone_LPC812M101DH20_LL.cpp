/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#include <nuclone_LPC812M101DH20_tests.hpp>

libMcuLL::sw::iocon::iocon<libMcuHw::ioconAddress> ioconPeripheral;
libMcuLL::sw::swm::swm<libMcuHw::swmAddress> swmPeriperhal;
libMcuLL::sw::gpio::gpio<libMcuHw::gpioAddress> gpioPeripheral;
libMcuLL::sw::spi::spiSync<libMcuHw::spi0Address, libMcuLL::sw::spi::chipEnables, std::uint16_t> spiSyncPeripheral;
libMcuLL::sw::spi::spiAsync<libMcuHw::spi0Address, libMcuLL::sw::spi::chipEnables, std::uint16_t> spiAsyncPeripheral16;
libMcuLL::sw::spi::spiAsync<libMcuHw::spi0Address, libMcuLL::sw::spi::chipEnables, std::uint8_t> spiAsyncPeripheral8;
libMcuLL::sw::syscon::syscon<libMcuHw::sysconAddress> sysconPeripheral;
libMcuLL::sw::usart::usartSync<libMcuHw::usart0Address, std::uint8_t> usartSyncPeripheral;
libMcuLL::sw::usart::usartAsync<libMcuHw::usart0Address, std::uint8_t> usartAsyncPeripheral;
libMcuLL::sw::i2c::i2c<libMcuHw::i2c0Address> i2cPeripheral;
libMcuLL::sw::sct::sct<libMcuHw::sctAddress> sctPeripheral;
libMcuLL::sw::acmp::acmp<libMcuHw::acmpAddress> acmpPeripheral;
libMcuLL::sw::fmc::fmc<libMcuHw::fmcAddress> fcmPeripheral;

void boardInit(void) {
  // clock enables and resets
  sysconPeripheral.enablePeripheralClocks(libMcuLL::sw::syscon::peripheralClocks::SWM |
                                          libMcuLL::sw::syscon::peripheralClocks::IOCON |
                                          libMcuLL::sw::syscon::peripheralClocks::GPIO);
  // setup IOCON pins
  ioconPeripheral.setup(xtalInPin, libMcuLL::sw::iocon::pullModes::INACTIVE);
  ioconPeripheral.setup(xtalOutPin, libMcuLL::sw::iocon::pullModes::INACTIVE);
  swmPeriperhal.setup(xtalInPin, xtalInFunction);
  swmPeriperhal.setup(xtalOutPin, xtalOutFunction);
  // setup system clocks
  sysconPeripheral.setSysOscControl(libMcuLL::hw::syscon::SYSOSCCTRL::NO_BYPASS | libMcuLL::hw::syscon::SYSOSCCTRL::FREQ_1_20MHz);
  sysconPeripheral.powerPeripherals(libMcuLL::sw::syscon::peripheralPowers::SYSOSC);
  libMcuLL::delay(6000);
  sysconPeripheral.selectPllClock(libMcuLL::sw::syscon::pllClockSources::SYSOSC);
  fcmPeripheral.setFlashWaitState(libMcuLL::sw::fmc::waitstates::WAIT_2_CLOCK);
  sysconPeripheral.depowerPeripherals(libMcuLL::sw::syscon::peripheralPowers::SYSPLL);
  sysconPeripheral.setSystemPllControl(4, libMcuLL::sw::syscon::pllPostDivider::DIV_4);
  sysconPeripheral.powerPeripherals(libMcuLL::sw::syscon::peripheralPowers::SYSPLL);
  while (!sysconPeripheral.getSystemPllStatus())
    ;
  sysconPeripheral.setSystemClockDivider(2);
  sysconPeripheral.selectMainClock(libMcuLL::sw::syscon::mainClockSources::PLL_OUT);
  // disable all unneeded clocks
  sysconPeripheral.disablePeripheralClocks(libMcuLL::sw::syscon::peripheralClocks::IOCON);
}
