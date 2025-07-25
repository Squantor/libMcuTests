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

libmcull::systick::Systick<libmcuhw::SystickAddress> systick_peripheral;
libmcull::nvic::Nvic<libmcuhw::NvicAddress, libmcuhw::ScbAddress> nvic_peripheral;
libmcull::iocon::Iocon<libmcuhw::IoconAddress> iocon_peripheral;
libmcull::swm::Swm<libmcuhw::SwmAddress> swm_peripheral;
libmcull::gpio::Gpio<libmcuhw::GpioAddress> gpio_peripheral;
libmcull::syscon::Syscon<libmcuhw::SysconAddress> syscon_peripheral;
libmcull::usart::UartPolled<libmcuhw::Usart0Address, UartTransferType> usart_peripheral;
libmcull::i2c::I2cInterrupt<libmcuhw::I2c0Address> i2c_interrupt_peripheral;
libmcuhal::usart::UartPolled<usart_peripheral, UartTransferType> hal_usart_peripheral;
libmcuhal::i2c::I2cInterrupt<i2c_interrupt_peripheral> hal_i2c_interrupt_peripheral;

extern "C" {

void I2C0_IRQHandler(void) {
  i2c_interrupt_peripheral.InterruptHandler();
}
}

void board_init(void) {
  // clock enables and resets
  syscon_peripheral.EnablePeripheralClocks(
    libmcull::syscon::peripheral_clocks_0::Swm | libmcull::syscon::peripheral_clocks_0::Iocon |
      libmcull::syscon::peripheral_clocks_0::Gpio0 | libmcull::syscon::peripheral_clocks_0::Gpio1,
    0);
  // setup IOCON pins
  iocon_peripheral.Setup(xtal_in_pin, libmcull::iocon::PullModes::Inactive);
  iocon_peripheral.Setup(xtal_out_pin, libmcull::iocon::PullModes::Inactive);
  swm_peripheral.Setup(xtal_in_pin, xtal_in_function);
  swm_peripheral.Setup(xtal_out_pin, xtal_out_function);
  // setup clock out test pin
  // swmPeriperhal.setup(test_pin, clock_out_function);
  // sysconPeripheral.SetClockOutput(libmcull::syscon::clockOutSources::MAIN, 10u);
  syscon_peripheral.ConfigureMcuClocks<nuclone_clock_config>();
  // switch mainclock
  // sysconPeripheral.SelectMainClock(libmcull::syscon::mainClockSources::EXT); // for selecting crystal oscillator
}
