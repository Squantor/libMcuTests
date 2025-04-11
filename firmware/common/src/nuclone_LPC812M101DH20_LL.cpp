/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#include <nuclone_LPC812M101DH20_tests.hpp>

libmcull::sw::iocon::iocon<libmcuhw::ioconAddress> ioconPeripheral;
libmcull::sw::swm::swm<libmcuhw::swmAddress> swmPeriperhal;
libmcull::sw::gpio::gpio<libmcuhw::gpioAddress> gpioPeripheral;
libmcull::sw::spi::spiSync<libmcuhw::spi0Address, libmcull::sw::spi::ChipEnables, std::uint16_t> spiSyncPeripheral;
libmcull::sw::spi::spiAsync<libmcuhw::spi0Address, libmcull::sw::spi::ChipEnables, std::uint16_t> spiAsyncPeripheral16;
libmcull::sw::spi::spiAsync<libmcuhw::spi0Address, libmcull::sw::spi::ChipEnables, std::uint8_t> spiAsyncPeripheral8;
libmcull::sw::syscon::syscon<libmcuhw::sysconAddress> sysconPeripheral;
libmcull::sw::usart::usartSync<libmcuhw::usart0Address, std::uint8_t> usartSyncPeripheral;
libmcull::sw::usart::usartAsync<libmcuhw::usart0Address, std::uint8_t> usartAsyncPeripheral;
libmcull::sw::i2c::i2c<libmcuhw::i2c0Address> i2cPeripheral;
libmcull::sw::sct::sct<libmcuhw::sctAddress> sctPeripheral;
libmcull::sw::acmp::acmp<libmcuhw::acmpAddress> acmpPeripheral;
libmcull::sw::fmc::fmc<libmcuhw::fmcAddress> fcmPeripheral;

void boardInit(void) {
  // clock enables and resets
  sysconPeripheral.enablePeripheralClocks(libmcull::sw::syscon::peripheralClocks::SWM |
                                          libmcull::sw::syscon::peripheralClocks::IOCON |
                                          libmcull::sw::syscon::peripheralClocks::GPIO);
  // setup IOCON pins
  ioconPeripheral.setup(xtalInPin, libmcull::sw::iocon::pullModes::INACTIVE);
  ioconPeripheral.setup(xtalOutPin, libmcull::sw::iocon::pullModes::INACTIVE);
  swmPeriperhal.setup(xtalInPin, xtalInFunction);
  swmPeriperhal.setup(xtalOutPin, xtalOutFunction);
  // setup system clocks
  sysconPeripheral.setSysOscControl(libmcuhw::syscon::SYSOSCCTRL::NO_BYPASS | libmcuhw::syscon::SYSOSCCTRL::FREQ_1_20MHz);
  sysconPeripheral.powerPeripherals(libmcull::sw::syscon::peripheralPowers::SYSOSC);
  libmcu::Delay(6000);
  sysconPeripheral.selectPllClock(libmcull::sw::syscon::pllClockSources::SYSOSC);
  fcmPeripheral.setFlashWaitState(libmcull::sw::fmc::waitstates::WAIT_2_CLOCK);
  sysconPeripheral.depowerPeripherals(libmcull::sw::syscon::peripheralPowers::SYSPLL);
  sysconPeripheral.setSystemPllControl(4, libmcull::sw::syscon::pllPostDivider::DIV_4);
  sysconPeripheral.powerPeripherals(libmcull::sw::syscon::peripheralPowers::SYSPLL);
  while (!sysconPeripheral.getSystemPllStatus())
    ;
  sysconPeripheral.setSystemClockDivider(2);
  sysconPeripheral.selectMainClock(libmcull::sw::syscon::mainClockSources::PLL_OUT);
  // disable all unneeded clocks
  sysconPeripheral.disablePeripheralClocks(libmcull::sw::syscon::peripheralClocks::IOCON);
}
