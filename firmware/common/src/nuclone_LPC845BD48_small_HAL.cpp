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

libmcull::iocon::Iocon<libmcuhw::kIoconAddress> iocon_peripheral;
libmcull::swm::Swm<libmcuhw::kSwmAddress> swm_peripheral;
libmcull::gpio::Gpio<libmcuhw::kGpioAddress> gpio_peripheral;
libmcull::syscon::Syscon<libmcuhw::kSysconAddress> syscon_peripheral;
libmcull::usart::UartPolled<libmcuhw::kUsart0Address, UartTransferType> usart_peripheral;
libmcuhal::usart::SyncUart<usart_peripheral, UartTransferType> hal_usart_peripheral;

void board_init(void) {
  // clock enables and resets
  syscon_peripheral.EnablePeripheralClocks(
    libmcull::syscon::peripheral_clocks_0::kSwm | libmcull::syscon::peripheral_clocks_0::kIocon |
      libmcull::syscon::peripheral_clocks_0::kGpio0 | libmcull::syscon::peripheral_clocks_0::kGpio1,
    0);
  // setup IOCON pins
  iocon_peripheral.Setup(xtal_in_pin, libmcull::iocon::PullModes::INACTIVE);
  iocon_peripheral.Setup(xtal_out_pin, libmcull::iocon::PullModes::INACTIVE);
  swm_peripheral.Setup(xtal_in_pin, xtal_in_function);
  swm_peripheral.Setup(xtal_out_pin, xtal_out_function);
  // setup clock out test pin
  // swmPeriperhal.setup(test_pin, clock_out_function);
  // sysconPeripheral.SetClockOutput(libmcull::syscon::clockOutSources::MAIN, 10u);
  syscon_peripheral.ConfigureMcuClocks<nuclone_clock_config>();
  // switch mainclock
  // sysconPeripheral.SelectMainClock(libmcull::syscon::mainClockSources::EXT); // for selecting crystal oscillator
}
