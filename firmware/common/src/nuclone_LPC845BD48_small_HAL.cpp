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

libmcull::iocon::Iocon<libmcuhw::kIoconAddress> ioconPeripheral;
libmcull::swm::Swm<libmcuhw::kSwmAddress> swmPeriperhal;
libmcull::gpio::Gpio<libmcuhw::kGpioAddress> gpioPeripheral;
libmcull::syscon::Syscon<libmcuhw::kSysconAddress> sysconPeripheral;
libmcull::usart::UartPolled<libmcuhw::kUsart0Address, UartTransferType> usart_peripheral;
libmcuhal::usart::SyncUart<usart_peripheral, UartTransferType> hal_usart_peripheral;

void boardInit(void) {
  // clock enables and resets
  sysconPeripheral.EnablePeripheralClocks(
    libmcull::syscon::peripheral_clocks_0::kSwm | libmcull::syscon::peripheral_clocks_0::kIocon |
      libmcull::syscon::peripheral_clocks_0::kGpio0 | libmcull::syscon::peripheral_clocks_0::kGpio1,
    0);
  // setup IOCON pins
  ioconPeripheral.Setup(xtalInPin, libmcull::iocon::PullModes::INACTIVE);
  ioconPeripheral.Setup(xtalOutPin, libmcull::iocon::PullModes::INACTIVE);
  swmPeriperhal.Setup(xtalInPin, xtalInFunction);
  swmPeriperhal.Setup(xtalOutPin, xtalOutFunction);
  // setup clock out test pin
  // swmPeriperhal.setup(testPin, clockOutFunction);
  // sysconPeripheral.SetClockOutput(libmcull::syscon::clockOutSources::MAIN, 10u);
  sysconPeripheral.ConfigureMcuClocks<nucloneClockConfig>();
  // switch mainclock
  // sysconPeripheral.SelectMainClock(libmcull::syscon::mainClockSources::EXT); // for selecting crystal oscillator
}
