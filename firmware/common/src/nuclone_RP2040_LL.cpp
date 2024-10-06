/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#include <nuclone_RP2040_LL.hpp>

libMcuLL::systick::systick<libMcuHw::systickAddress> systickPeripheral;
libMcuLL::nvic::nvic<libMcuHw::nvicAddress, libMcuHw::scbAddress> nvicPeripheral;

libMcuLL::sw::padsBank0::padsBank0<libMcuHw::padsBank0Address> padsBank0Peripheral;
libMcuLL::sw::gpioBank0::gpioBank0<libMcuHw::ioBank0Address> gpioBank0Peripheral;
libMcuLL::sw::resets::resets<libMcuHw::resetsAddress> resetsPeripheral;
libMcuLL::sw::sioGpio::sioGpio<libMcuHw::sioAddress> sioGpioPeripheral;
libMcuLL::sw::clocks::clocks<libMcuHw::clocksAddress> clocksPeripheral;
libMcuLL::sw::xosc::xosc<libMcuHw::xoscAddress> xoscPeripheral;
libMcuLL::sw::pll::pll<libMcuHw::pllSysAddress> pllSysPeripheral;
libMcuLL::sw::pll::pll<libMcuHw::pllUsbAddress> pllUsbPeripheral;
libMcuLL::sw::spi::spi<libMcuHw::spi0Address> spiPeripheral;
libMcuLL::sw::i2c::i2c<libMcuHw::i2c0Address> i2cPeripheral;
libMcuLL::sw::uart::uart<libMcuHw::uart0Address> uartPeripheral;

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
  clocksPeripheral.setup(libMcuLL::sw::clocks::systemSources::REF, 1, 0);  // switch to safe clock for SYS
  timeout = resetsPeripheral.reset(libMcuLL::sw::resets::IO_BANK0 | libMcuLL::sw::resets::PADS_BANK0 |
                                     libMcuLL::sw::resets::PLL_SYS | libMcuLL::sw::resets::PLL_USB,
                                   100000);
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
  clocksPeripheral.setup(libMcuLL::sw::clocks::systemAuxSources::PLL_SYS, 1, 0, 125);  // we delay as we switch from 3MHz to 125MHz
  clocksPeripheral.setup(libMcuLL::sw::clocks::peripheralSources::SYS, 32);            // minimal delay as we do not make big switch
  clocksPeripheral.setup(libMcuLL::sw::clocks::usbSources::PLL_USB, 1, 32);            // minimal delay as we do not make big switch
  // 48MHz / 3 = 16MHz
  clocksPeripheral.setup(libMcuLL::sw::clocks::adcSources::PLL_USB, 3, 32);  // minimal delay as we do not make big switch
  // 12MHz / 256 = 46875 Hz, no fractional divison needed
  clocksPeripheral.setup(libMcuLL::sw::clocks::rtcSources::XOSC, 256, 0, 32);  // minimal delay as we do not make big switch
  /*
  // Configure 1 us tick for watchdog and timer
  WATCHDOG->TICK = ((F_REF / F_TICK) << WATCHDOG_TICK_CYCLES_Pos) | WATCHDOG_TICK_ENABLE_Msk;
  */

  // setup LED pin
  // padsBank0Peripheral.setup(ledPin, libMcuLL::sw::pads::driveModes::DRIVE_4MA, false, false, false, false);
  // gpioBank0Peripheral.setup(ledPin);
  // sioGpioPeripheral.output(ledPin);
  // setup clock out pin
  clocksPeripheral.setup(libMcuLL::sw::clocks::gpoutGenerators::GPOUT0, libMcuLL::sw::clocks::gpoutSources::SYS, 10, 0, 12);
  // padsBank0Peripheral.setup(clockOutPin, libMcuLL::sw::pads::driveModes::DRIVE_12MA, false, false, false, false);
  // gpioBank0Peripheral.setup(clockOutPin);

  // setup systick
  // systickPeripheral.init(120000000 / 8);
  // systickPeripheral.start(blinkyLedLambda);
  // reset all relevant peripherals
  // resetsPeripheral.reset(libMcuLL::sw::resets::PADS_BANK0 | libMcuLL::sw::resets::IO_BANK0, 100000);
  (void)timeout;
}
