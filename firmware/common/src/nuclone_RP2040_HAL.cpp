/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#include <nuclone_RP2040_HAL.hpp>

libmcull::systick::systick<libmcuhw::systickAddress> systickPeripheral;
libmcull::nvic::nvic<libmcuhw::nvicAddress, libmcuhw::scbAddress> nvicPeripheral;
libmcull::resets::resets<libmcuhw::resetsAddress> resetsPeripheral;

libmcuhal::pins::pins<libmcuhw::padsBank0Address, libmcuhw::ioBank0Address> pinsHal;
libmcuhal::gpio::gpio<libmcuhw::padsBank0Address, libmcuhw::ioBank0Address, libmcuhw::sioAddress> gpioHal;

extern "C" {
void SysTick_Handler(void) {
  systickPeripheral.isr();
}
}

void boardInit(void) {
  std::uint32_t timeout;
  // reset all setup peripherals
  timeout = resetsPeripheral.reset(
    libmcull::resets::IO_BANK0 | libmcull::resets::PADS_BANK0 | libmcull::resets::PLL_SYS | libmcull::resets::PLL_USB, 100000);
  // clear resusitator status
  // CLOCKS_SET->CLK_SYS_RESUS_CTRL = CLOCKS_SYS_RESUS_CTRL_CLEAR;
  // 47 ticks is around 1 ms @ 12 MHz
  // xoscStart(47, 0x1000000);

  // Setup PLL's
  // atadarov config: 12MHz * 40 / 4 / 1 = 120MHz
  // timeout = pllStart(PLL_SYS, 1, 40, 4, 1, 0x1000000);
  // SDK config: 12MHz * 125 / 6 / 2 = 125MHz
  // pllStart(PLL_SYS, 1, 125, 6, 2, 0x1000000);
  // atadarov config: 12 MHz * 36 / 3 / 3 = 48MHz
  // pllStart(PLL_USB, 1, 36, 3, 3, 0x1000000);
  // SDK config: 12MHz * 100 / 5 / 5 = 48MHz
  // pllStart(PLL_USB, 1, 100, 5, 5, 0x1000000);

  // Setup clocks
  // clocksSwitchGlitchlessSrc(CLK_REF, REF_SRC_XOSC, 0x1000000);
  // clocksSwitchGlitchlessAux(CLK_SYS, SYS_AUXSRC_PLL_SYS, 0x1000000);
  // clockSwitchBasicAux(CLK_PERI, PERI_AUXSRC_CLK_SYS);
  // clockSwitchBasicAux(CLK_USB, USB_AUXSRC_PLL_USB);
  // clockSwitchBasicAux(CLK_ADC, ADC_AUXSRC_PLL_USB);
  // clocksSetDivider(CLK_RTC, 256, 0);  // 12MHz / 256 = 46875 Hz
  // clockSwitchBasicAux(CLK_RTC, RTC_AUXSRC_XOSC);
  // output clock network to GPOUT0, very useful for clock debugging
  // clocksSetDivider(CLK_GPOUT0, 10, 0);  // divide by 10 to make math easier
  // clockSwitchBasicAux(CLK_GPOUT0, GPOUT0_AUXSRC_CLK_USB);
  // iobank0GpioCtrl(IO_BANK0, CLOCK_PIN, BANK0_GPIO21_FUNC_CLOCK_GPOUT0, 0);
  // setup clock output pin
  // padsBank0Peripheral.setup(clockOutPin, sw::pads::driveModes::DRIVE_8MA, false, false, false, true);
  // gpioBank0Peripheral.setup(clockOutPin);
  /*
  // Configure 1 us tick for watchdog and timer
  WATCHDOG->TICK = ((F_REF / F_TICK) << WATCHDOG_TICK_CYCLES_Pos) | WATCHDOG_TICK_ENABLE_Msk;
  */

  // setup LED pin
  // padsBank0Peripheral.setup(ledPin, sw::pads::driveModes::DRIVE_4MA, false, false, false, false);
  // gpioBank0Peripheral.setup(ledPin);
  // sioGpioPeripheral.output(ledPin);

  //  setup systick
  // SysTick_Config(FREQ_CPU / TICKS_PER_S);
  systickPeripheral.init(12000000 / TICKS_PER_S);
  // reset all relevant peripherals
  (void)timeout;
  pinsHal.initialize();
  pinsHal.setup(ledPin, libmcuhal::pins::driveModes::DRIVE_12MA, libmcuhal::pins::pullModes::NONE,
                libmcuhal::pins::speedModes::SLEW_SLOW, false);
}
