/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#include <nuclone_RP2040_LL.hpp>

libmcull::systick::systick<libmcuhw::systickAddress> systickPeripheral;
libmcull::nvic::nvic<libmcuhw::nvicAddress, libmcuhw::scbAddress> nvicPeripheral;

libmcull::padsBank0::padsBank0<libmcuhw::padsBank0Address> padsBank0Peripheral;
libmcull::gpioBank0::gpioBank0<libmcuhw::ioBank0Address> gpioBank0Peripheral;
libmcull::resets::resets<libmcuhw::resetsAddress> resetsPeripheral;
libmcull::sioGpio::sioGpio<libmcuhw::sioAddress> sioGpioPeripheral;
libmcull::clocks::clocks<libmcuhw::clocksAddress> clocksPeripheral;
libmcull::xosc::xosc<libmcuhw::xoscAddress> xoscPeripheral;
libmcull::pll::pll<libmcuhw::pllSysAddress> pllSysPeripheral;
libmcull::pll::pll<libmcuhw::pllUsbAddress> pllUsbPeripheral;
libmcull::spi::spi<libmcuhw::spi0Address> spiPeripheral;
libmcull::i2c::i2c<libmcuhw::i2c0Address> i2cPeripheral;
libmcull::uart::uart<libmcuhw::uart0Address> uartPeripheral;

extern "C" {
void SysTick_Handler(void) {
  systickPeripheral.isr();
}
}

auto blinkyLedLambda = []() {
  sioGpioPeripheral.toggle(ledPin);
};

void boardInit(void) {
  std::uint32_t timeout;
  // reset all setup peripherals
  clocksPeripheral.setup(libmcull::clocks::systemSources::REF, 1, 0);  // switch to safe clock for SYS
  timeout = resetsPeripheral.reset(
    libmcull::resets::IO_BANK0 | libmcull::resets::PADS_BANK0 | libmcull::resets::PLL_SYS | libmcull::resets::PLL_USB, 100000);
  // setup crystal oscillator
  clocksPeripheral.clearResusitator();
  // 47 ticks is around 1 ms @ 12 MHz
  xoscPeripheral.start(47, 0x1000000);

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
  clocksPeripheral.setup(libmcull::clocks::systemAuxSources::PLL_SYS, 1, 0, 125);  // we delay as we switch from 3MHz to 125MHz
  clocksPeripheral.setup(libmcull::clocks::peripheralSources::SYS, 32);            // minimal delay as we do not make big switch
  clocksPeripheral.setup(libmcull::clocks::usbSources::PLL_USB, 1, 32);            // minimal delay as we do not make big switch
  // 48MHz / 3 = 16MHz
  clocksPeripheral.setup(libmcull::clocks::adcSources::PLL_USB, 3, 32);  // minimal delay as we do not make big switch
  // 12MHz / 256 = 46875 Hz, no fractional divison needed
  clocksPeripheral.setup(libmcull::clocks::rtcSources::XOSC, 256, 0, 32);  // minimal delay as we do not make big switch
  /*
  // Configure 1 us tick for watchdog and timer
  WATCHDOG->TICK = ((F_REF / F_TICK) << WATCHDOG_TICK_CYCLES_Pos) | WATCHDOG_TICK_ENABLE_Msk;
  */

  // setup LED pin
  // padsBank0Peripheral.setup(ledPin, libmcull::pads::driveModes::DRIVE_4MA, false, false, false, false);
  // gpioBank0Peripheral.setup(ledPin);
  // sioGpioPeripheral.output(ledPin);
  // setup clock out pin
  clocksPeripheral.setup(libmcull::clocks::gpoutGenerators::GPOUT0, libmcull::clocks::gpoutSources::SYS, 10, 0, 12);
  // padsBank0Peripheral.setup(clockOutPin, libmcull::pads::driveModes::DRIVE_12MA, false, false, false, false);
  // gpioBank0Peripheral.setup(clockOutPin);

  // setup systick
  // systickPeripheral.init(120000000 / 8);
  // systickPeripheral.start(blinkyLedLambda);
  // reset all relevant peripherals
  // resetsPeripheral.reset(libmcull::resets::PADS_BANK0 | libmcull::resets::IO_BANK0, 100000);
  (void)timeout;
}
