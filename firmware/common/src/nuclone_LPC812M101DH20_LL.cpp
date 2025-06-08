/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#include <nuclone_LPC812M101DH20_tests.hpp>

libmcull::iocon::Iocon<libmcuhw::kIoconAddress> ioconPeripheral;
libmcull::swm::Swm<libmcuhw::kSwmAddress> swmPeriperhal;
libmcull::gpio::Gpio<libmcuhw::kGpioAddress> gpioPeripheral;
libmcull::spi::SpiPolled<libmcuhw::kSpi0Address, libmcull::spi::ChipEnables, std::uint16_t> spiSyncPeripheral;
libmcull::spi::SpiAsync<libmcuhw::kSpi0Address, libmcull::spi::ChipEnables, std::uint16_t> spiAsyncPeripheral16;
libmcull::spi::SpiAsync<libmcuhw::kSpi0Address, libmcull::spi::ChipEnables, std::uint8_t> spiAsyncPeripheral8;
libmcull::syscon::Syscon<libmcuhw::kSysconAddress> sysconPeripheral;
libmcull::usart::usartSync<libmcuhw::kUsart0Address, std::uint8_t> usartSyncPeripheral;
libmcull::usart::UsartAsync<libmcuhw::kUsart0Address, std::uint8_t> usartAsyncPeripheral;
libmcull::i2c::I2c<libmcuhw::kI2c0Address> i2cPeripheral;
libmcull::sct::Sct<libmcuhw::kSctAddress> sctPeripheral;
libmcull::acmp::Acmp<libmcuhw::kAcmpAddress> acmpPeripheral;
libmcull::fmc::Fmc<libmcuhw::kFmcAddress> fcmPeripheral;

void boardInit(void) {
  // clock enables and resets
  sysconPeripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::kClockSwm |
                                          libmcull::syscon::PeripheralClocks::kClockIocon |
                                          libmcull::syscon::PeripheralClocks::kClockGpio);
  // setup IOCON pins
  ioconPeripheral.Setup(xtalInPin, libmcull::iocon::PullModes::INACTIVE);
  ioconPeripheral.Setup(xtalOutPin, libmcull::iocon::PullModes::INACTIVE);
  swmPeriperhal.setup(xtalInPin, xtalInFunction);
  swmPeriperhal.setup(xtalOutPin, xtalOutFunction);
  // setup system clocks
  sysconPeripheral.SetSysOscControl(libmcuhw::syscon::SYSOSCCTRL::kNO_BYPASS | libmcuhw::syscon::SYSOSCCTRL::kFREQ_1_20MHz);
  sysconPeripheral.PowerPeripherals(libmcull::syscon::PeripheralPowers::kPowerSysOsc);
  libmcu::Delay(6000);
  sysconPeripheral.SelectPllClockSource(libmcull::syscon::PllClockSources::kSysOsc);
  fcmPeripheral.setFlashWaitState(libmcull::fmc::WaitStates::WAIT_2_CLOCK);
  sysconPeripheral.DepowerPeripherals(libmcull::syscon::PeripheralPowers::kPowerSysPll);
  sysconPeripheral.SetSystemPllControl(4, libmcull::syscon::PllPostDividers::kDiv4);
  sysconPeripheral.PowerPeripherals(libmcull::syscon::PeripheralPowers::kPowerSysPll);
  while (!sysconPeripheral.GetSystemPllStatus())
    ;
  sysconPeripheral.SetSystemClockDivider(2);
  sysconPeripheral.SelectMainClockSource(libmcull::syscon::MainClockSources::kPllOut);
  // disable all unneeded clocks
  sysconPeripheral.DisablePeripheralClocks(libmcull::syscon::PeripheralClocks::kClockIocon);
}
