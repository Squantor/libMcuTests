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

namespace clocks = libmcuhw::clock;

libmcull::iocon::Iocon<libmcuhw::ioconAddress> iocon_peripheral;
libmcull::swm::Swm<libmcuhw::swmAddress> swmPeriperhal;
libmcull::gpio::gpio<libmcuhw::gpioAddress> gpioPeripheral;
libmcull::syscon::Syscon<libmcuhw::sysconAddress> sysconPeripheral;
libmcull::usart::SyncUart<libmcuhw::kUsart0Address, std::uint8_t> usartPeripheral;
libmcull::dma::Dma<libmcuhw::dmaAddress> dmaPeripheral;
libmcull::inmux::InMux<libmcuhw::inmuxAddress> inmux_peripheral;
libmcull::spi::SpiPolled<libmcuhw::spi0Address, libmcull::spi::SpiChipEnables, std::uint8_t> g_spi_polled_peripheral;

void boardInit(void) {
  // clock enables and resets
  sysconPeripheral.enablePeripheralClocks(libmcull::syscon::peripheralClocks0::SWM | libmcull::syscon::peripheralClocks0::IOCON |
                                            libmcull::syscon::peripheralClocks0::GPIO0 | libmcull::syscon::peripheralClocks0::GPIO1,
                                          0);
  // setup IOCON pins
  iocon_peripheral.setup(xtalInPin, libmcull::iocon::pullModes::INACTIVE);
  iocon_peripheral.setup(xtalOutPin, libmcull::iocon::pullModes::INACTIVE);
  swmPeriperhal.setup(xtalInPin, xtalInFunction);
  swmPeriperhal.setup(xtalOutPin, xtalOutFunction);
  // setup clock out test pin
  // swmPeriperhal.setup(testPin, clockOutFunction);
  // sysconPeripheral.setClockOutput(libmcull::syscon::clockOutSources::MAIN, 10u);
  sysconPeripheral.configureMcuClocks<nucloneClockConfig>();
  // switch mainclock
  // sysconPeripheral.selectMainClock(libmcull::syscon::mainClockSources::EXT); // for selecting crystal oscillator
}
