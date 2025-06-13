/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#include <nuclone_RP2040_LL.hpp>

libmcull::systick::Systick<libmcuhw::kSystickAddress> systick_peripheral;
libmcull::nvic::Nvic<libmcuhw::kNvicAddress, libmcuhw::kScbAddress> nvic_peripheral;
libmcull::pads::PadsBank0<libmcuhw::kPadsBank0Address> pads_bank0_peripheral;
libmcull::gpioBank0::GpioBank0<libmcuhw::kIoBank0Address> gpio_bank0_peripheral;
libmcull::resets::Resets<libmcuhw::kResetsAddress> resets_peripheral;
libmcull::sio_gpio::SioGpio<libmcuhw::kSioAddress> sio_gpio_peripheral;
libmcull::clocks::Clocks<libmcuhw::kClocksAddress> clocks_peripheral;
libmcull::xosc::Xosc<libmcuhw::kXoscAddress> xosc_peripheral;
libmcull::pll::Pll<libmcuhw::kPllSysAddress> pll_sys_peripheral;
libmcull::pll::Pll<libmcuhw::kPllUsbAddress> pll_usb_peripheral;
libmcull::spi::SpiPolled<libmcuhw::kSpi0Address> spi_polled_peripheral;
libmcull::i2c::I2cPolled<libmcuhw::kI2c0Address> i2c_polled_peripheral;
libmcull::uart::UartPolled<libmcuhw::kUart0Address> uart_polled_peripheral;

extern "C" {
void SysTick_Handler(void) {
  systick_peripheral.isr();
}
}

auto blinkyLedLambda = []() {
  sio_gpio_peripheral.Toggle(ledPin);
};

void board_init(void) {
  std::uint32_t timeout;
  // reset all setup peripherals
  clocks_peripheral.Setup(libmcull::clocks::SystemSources::kRef, 1, 0);  // switch to safe clock for SYS
  timeout = resets_peripheral.Reset(
    libmcull::resets::kIoBank0 | libmcull::resets::kPadsBank0 | libmcull::resets::kPllSys | libmcull::resets::kPllUsb, 100000);
  // setup crystal oscillator
  clocks_peripheral.ClearResusitator();
  // 47 ticks is around 1 ms @ 12 MHz
  xosc_peripheral.Start(47, 0x1000000);

  // Setup PLL's
  // atadarov config: 12MHz * 40 / 4 / 1 = 120MHz
  timeout = pll_sys_peripheral.start(1, 40, 4, 1, 0x1000000);
  // SDK config: 12MHz * 125 / 6 / 2 = 125MHz
  // timeout = pll_usb_peripheral.start(1, 125, 6, 2, 0x1000000);
  // atadarov config: 12 MHz * 36 / 3 / 3 = 48MHz
  timeout = pll_usb_peripheral.start(1, 36, 3, 3, 0x1000000);
  // SDK config: 12MHz * 100 / 5 / 5 = 48MHz
  // timeout = pll_usb_peripheral.start(1, 100, 5, 5, 0x1000000);

  // Setup clocks
  clocks_peripheral.Setup(libmcull::clocks::SystemAuxSources::kPllSys, 1, 0, 125);  // we delay as we switch from 3MHz to 125MHz
  clocks_peripheral.Setup(libmcull::clocks::PeripheralSources::kSys, 32);           // minimal delay as we do not make big switch
  clocks_peripheral.Setup(libmcull::clocks::UsbSources::kPllUsb, 1, 32);            // minimal delay as we do not make big switch
  // 48MHz / 3 = 16MHz
  clocks_peripheral.Setup(libmcull::clocks::AdcSources::kPllUsb, 3, 32);  // minimal delay as we do not make big switch
  // 12MHz / 256 = 46875 Hz, no fractional divison needed
  clocks_peripheral.Setup(libmcull::clocks::RtcSources::kXosc, 256, 0, 32);  // minimal delay as we do not make big switch
  /*
  // Configure 1 us tick for watchdog and timer
  WATCHDOG->TICK = ((F_REF / F_TICK) << WATCHDOG_TICK_CYCLES_Pos) | WATCHDOG_TICK_ENABLE_Msk;
  */

  // setup LED pin
  // pads_bank0_peripheral.setup(ledPin, libmcull::pads::driveModes::DRIVE_4MA, false, false, false, false);
  // gpio_bank0_peripheral.setup(ledPin);
  // sio_gpio_peripheral.output(ledPin);
  // setup clock out pin
  clocks_peripheral.Setup(libmcull::clocks::GpoutGenerators::kGpout0, libmcull::clocks::GpoutSources::kSys, 10, 0, 12);
  // pads_bank0_peripheral.setup(clockOutPin, libmcull::pads::driveModes::DRIVE_12MA, false, false, false, false);
  // gpio_bank0_peripheral.setup(clockOutPin);

  // setup systick
  // systick_peripheral.init(120000000 / 8);
  // systick_peripheral.start(blinkyLedLambda);
  // reset all relevant peripherals
  // resets_peripheral.reset(libmcull::resets::PADS_BANK0 | libmcull::resets::IO_BANK0, 100000);
  (void)timeout;
}
