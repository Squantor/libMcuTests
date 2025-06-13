/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#include <nuclone_LPC812M101DH20_tests.hpp>

libmcull::iocon::Iocon<libmcuhw::kIoconAddress> iocon_peripheral;
libmcull::swm::Swm<libmcuhw::kSwmAddress> swm_peripheral;
libmcull::gpio::Gpio<libmcuhw::kGpioAddress> gpio_peripheral;
libmcull::spi::SpiPolled<libmcuhw::kSpi0Address, libmcull::spi::ChipEnables, std::uint16_t> spi_polled_peripheral;
libmcull::spi::SpiAsync<libmcuhw::kSpi0Address, libmcull::spi::ChipEnables, std::uint16_t> spiAsyncPeripheral16;
libmcull::spi::SpiAsync<libmcuhw::kSpi0Address, libmcull::spi::ChipEnables, std::uint8_t> spiAsyncPeripheral8;
libmcull::syscon::Syscon<libmcuhw::kSysconAddress> syscon_peripheral;
libmcull::usart::UsartPolled<libmcuhw::kUsart0Address, std::uint8_t> usart_polled_peripheral;
libmcull::usart::UsartAsync<libmcuhw::kUsart0Address, std::uint8_t> usartAsyncPeripheral;
libmcull::i2c::I2c<libmcuhw::kI2c0Address> i2c_peripheral;
libmcull::sct::Sct<libmcuhw::kSctAddress> sct_peripheral;
libmcull::acmp::Acmp<libmcuhw::kAcmpAddress> acmp_peripheral;
libmcull::fmc::Fmc<libmcuhw::kFmcAddress> fcmPeripheral;

void board_init(void) {
  // clock enables and resets
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::kClockSwm |
                                           libmcull::syscon::PeripheralClocks::kClockIocon |
                                           libmcull::syscon::PeripheralClocks::kClockGpio);
  // setup IOCON pins
  iocon_peripheral.Setup(xtal_in_pin, libmcull::iocon::PullModes::INACTIVE);
  iocon_peripheral.Setup(xtal_out_pin, libmcull::iocon::PullModes::INACTIVE);
  swm_peripheral.setup(xtal_in_pin, xtal_in_function);
  swm_peripheral.setup(xtal_out_pin, xtal_out_function);
  // setup system clocks
  syscon_peripheral.SetSysOscControl(libmcuhw::syscon::SYSOSCCTRL::kNO_BYPASS | libmcuhw::syscon::SYSOSCCTRL::kFREQ_1_20MHz);
  syscon_peripheral.PowerPeripherals(libmcull::syscon::PeripheralPowers::kPowerSysOsc);
  libmcu::Delay(6000);
  syscon_peripheral.SelectPllClockSource(libmcull::syscon::PllClockSources::kSysOsc);
  fcmPeripheral.setFlashWaitState(libmcull::fmc::WaitStates::WAIT_2_CLOCK);
  syscon_peripheral.DepowerPeripherals(libmcull::syscon::PeripheralPowers::kPowerSysPll);
  syscon_peripheral.SetSystemPllControl(4, libmcull::syscon::PllPostDividers::kDiv4);
  syscon_peripheral.PowerPeripherals(libmcull::syscon::PeripheralPowers::kPowerSysPll);
  while (!syscon_peripheral.GetSystemPllStatus())
    ;
  syscon_peripheral.SetSystemClockDivider(2);
  syscon_peripheral.SelectMainClockSource(libmcull::syscon::MainClockSources::kPllOut);
  // disable all unneeded clocks
  syscon_peripheral.DisablePeripheralClocks(libmcull::syscon::PeripheralClocks::kClockIocon);
}
