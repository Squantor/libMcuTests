/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#include <nuclone_RP2040_LL.hpp>

libmcull::systick::systick<libmcuhw::systickAddress> systickPeripheral;
libmcull::nvic::nvic<libmcuhw::nvicAddress, libmcuhw::scbAddress> nvicPeripheral;

libmcull::pads::PadsBank0<libmcuhw::kPadsBank0Address> padsBank0Peripheral;
libmcull::gpioBank0::GpioBank0<libmcuhw::kIoBank0Address> gpioBank0Peripheral;
libmcull::resets::Resets<libmcuhw::kResetsAddress> resetsPeripheral;
libmcull::sioGpio::SioGpio<libmcuhw::kSioAddress> sioGpioPeripheral;
libmcull::clocks::Clocks<libmcuhw::kClocksAddress> clocksPeripheral;
libmcull::xosc::Xosc<libmcuhw::kXoscAddress> xoscPeripheral;
libmcull::pll::Pll<libmcuhw::kPllSysAddress> pllSysPeripheral;
libmcull::pll::Pll<libmcuhw::kPllUsbAddress> pllUsbPeripheral;
libmcull::spi::SpiPolled<libmcuhw::kSpi0Address> spiPeripheral;
libmcull::i2c::I2cPolled<libmcuhw::kI2c0Address> i2cPeripheral;
libmcull::uart::UartPolled<libmcuhw::kUart0Address> uartPeripheral;

extern "C" {
void SysTick_Handler(void) {
  systickPeripheral.isr();
}
}

auto blinkyLedLambda = []() {
  sioGpioPeripheral.Toggle(ledPin);
};

void boardInit(void) {
  std::uint32_t timeout;
  // reset all setup peripherals
  clocksPeripheral.Setup(libmcull::clocks::SystemSources::kRef, 1, 0);  // switch to safe clock for SYS
  timeout = resetsPeripheral.Reset(
    libmcull::resets::kIoBank0 | libmcull::resets::kPadsBank0 | libmcull::resets::kPllSys | libmcull::resets::kPllUsb, 100000);
  // setup crystal oscillator
  clocksPeripheral.ClearResusitator();
  // 47 ticks is around 1 ms @ 12 MHz
  xoscPeripheral.Start(47, 0x1000000);

  // Setup PLL's
  // atadarov config: 12MHz * 40 / 4 / 1 = 120MHz
  timeout = pllSysPeripheral.start(1, 40, 4, 1, 0x1000000);
  // SDK config: 12MHz * 125 / 6 / 2 = 125MHz
  // timeout = pllUsbPeripheral.start(1, 125, 6, 2, 0x1000000);
  // atadarov config: 12 MHz * 36 / 3 / 3 = 48MHz
  timeout = pllUsbPeripheral.start(1, 36, 3, 3, 0x1000000);
  // SDK config: 12MHz * 100 / 5 / 5 = 48MHz
  // timeout = pllUsbPeripheral.start(1, 100, 5, 5, 0x1000000);

  // Setup clocks
  clocksPeripheral.Setup(libmcull::clocks::SystemAuxSources::kPllSys, 1, 0, 125);  // we delay as we switch from 3MHz to 125MHz
  clocksPeripheral.Setup(libmcull::clocks::PeripheralSources::kSys, 32);           // minimal delay as we do not make big switch
  clocksPeripheral.Setup(libmcull::clocks::UsbSources::kPllUsb, 1, 32);            // minimal delay as we do not make big switch
  // 48MHz / 3 = 16MHz
  clocksPeripheral.Setup(libmcull::clocks::AdcSources::kPllUsb, 3, 32);  // minimal delay as we do not make big switch
  // 12MHz / 256 = 46875 Hz, no fractional divison needed
  clocksPeripheral.Setup(libmcull::clocks::RtcSources::kXosc, 256, 0, 32);  // minimal delay as we do not make big switch
  /*
  // Configure 1 us tick for watchdog and timer
  WATCHDOG->TICK = ((F_REF / F_TICK) << WATCHDOG_TICK_CYCLES_Pos) | WATCHDOG_TICK_ENABLE_Msk;
  */

  // setup LED pin
  // padsBank0Peripheral.setup(ledPin, libmcull::pads::driveModes::DRIVE_4MA, false, false, false, false);
  // gpioBank0Peripheral.setup(ledPin);
  // sioGpioPeripheral.output(ledPin);
  // setup clock out pin
  clocksPeripheral.Setup(libmcull::clocks::GpoutGenerators::kGpout0, libmcull::clocks::GpoutSources::kSys, 10, 0, 12);
  // padsBank0Peripheral.setup(clockOutPin, libmcull::pads::driveModes::DRIVE_12MA, false, false, false, false);
  // gpioBank0Peripheral.setup(clockOutPin);

  // setup systick
  // systickPeripheral.init(120000000 / 8);
  // systickPeripheral.start(blinkyLedLambda);
  // reset all relevant peripherals
  // resetsPeripheral.reset(libmcull::resets::PADS_BANK0 | libmcull::resets::IO_BANK0, 100000);
  (void)timeout;
}
