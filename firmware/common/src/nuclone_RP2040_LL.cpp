/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#include <nuclone_RP2040_LL.hpp>

using namespace libMcuLL;

sw::systick::systick<hw::systickAddress> systickPeripheral;
sw::nvic::nvic<hw::nvicAddress, hw::scbAddress> nvicPeripheral;

libMcuLL::padsBank0PeripheralType padsBank0Peripheral;
libMcuLL::gpioBank0PeripheralType gpioBank0Peripheral;
libMcuLL::resetsPeripheralType resetsPeripheral;
libMcuLL::sioGpioPeripheralType sioGpioPeripheral;
libMcuLL::clocksPeripheralType clocksPeripheral;
libMcuLL::xoscPeripheralType xoscPeripheral;
libMcuLL::pllSysPeripheralType pllSysPeripheral;
libMcuLL::pllUsbPeripheralType pllUsbPeripheral;
libMcuLL::spi0PeripheralType spiPeripheral;

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
  clocksPeripheral.setup(sw::clocks::systemSources::REF, 1, 0);  // switch to safe clock for SYS
  timeout =
    resetsPeripheral.reset(sw::resets::IO_BANK0 | sw::resets::PADS_BANK0 | sw::resets::PLL_SYS | sw::resets::PLL_USB, 100000);
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
  clocksPeripheral.setup(sw::clocks::systemAuxSources::PLL_SYS, 1, 0, 125);  // we delay as we switch from 3MHz to 125MHz
  clocksPeripheral.setup(sw::clocks::peripheralSources::SYS, 32);            // minimal delay as we do not make big switch
  clocksPeripheral.setup(sw::clocks::usbSources::PLL_USB, 1, 32);            // minimal delay as we do not make big switch
  // 48MHz / 3 = 16MHz
  clocksPeripheral.setup(sw::clocks::adcSources::PLL_USB, 3, 32);  // minimal delay as we do not make big switch
  // 12MHz / 256 = 46875 Hz, no fractional divison needed
  clocksPeripheral.setup(sw::clocks::rtcSources::XOSC, 256, 0, 32);  // minimal delay as we do not make big switch
  /*
  // Configure 1 us tick for watchdog and timer
  WATCHDOG->TICK = ((F_REF / F_TICK) << WATCHDOG_TICK_CYCLES_Pos) | WATCHDOG_TICK_ENABLE_Msk;
  */

  // setup LED pin
  // padsBank0Peripheral.setup(ledPin, sw::pads::driveModes::DRIVE_4MA, false, false, false, false);
  // gpioBank0Peripheral.setup(ledPin);
  // sioGpioPeripheral.output(ledPin);
  // setup clock out pin
  clocksPeripheral.setup(sw::clocks::gpoutGenerators::GPOUT0, sw::clocks::gpoutSources::SYS, 10, 0, 12);
  // padsBank0Peripheral.setup(clockOutPin, sw::pads::driveModes::DRIVE_12MA, false, false, false, false);
  // gpioBank0Peripheral.setup(clockOutPin);

  // setup systick
  // systickPeripheral.init(120000000 / 8);
  // systickPeripheral.start(blinkyLedLambda);
  // reset all relevant peripherals
  // resetsPeripheral.reset(sw::resets::PADS_BANK0 | sw::resets::IO_BANK0, 100000);
  (void)timeout;
}
