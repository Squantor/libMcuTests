/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 gpio
 */
#include <nuclone_RP2040_LL.hpp>
#include <minunit.h>
#include <RP2040_LL_teardown.hpp>
#include <common.hpp>

// peripheral register sets
static constexpr libmcu::HwAddressType dutAddress = libmcuhw::IoBank0Address;
libmcuhw::gpio_bank0::GpioBank0 *const dutRegisters{reinterpret_cast<libmcuhw::gpio_bank0::GpioBank0 *>(dutAddress)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupGpio) {
  MINUNIT_CHECK(Rp2040Teardown_correct() == true);
  resets_peripheral.Reset(libmcull::resets::IoBank0 | libmcull::resets::PadsBank0, 100000);
  // connect all GPIO's
  gpio_bank0_peripheral.Setup(gpio0Pin);
  gpio_bank0_peripheral.Setup(gpio1Pin);
  gpio_bank0_peripheral.Setup(gpio2Pin);
  gpio_bank0_peripheral.Setup(gpio3Pin);
  gpio_bank0_peripheral.Setup(gpio4Pin);
  gpio_bank0_peripheral.Setup(gpio5Pin);
}

MINUNIT_ADD(RP2040gpio, RP2040SetupGpio, Rp2040Teardown) {
  MINUNIT_CHECK(dutRegisters->GPIO[0].CTRL == 0x0000'0005u);
  MINUNIT_CHECK(dutRegisters->GPIO[1].CTRL == 0x0000'0005u);
  MINUNIT_CHECK(dutRegisters->GPIO[2].CTRL == 0x0000'0005u);
  MINUNIT_CHECK(dutRegisters->GPIO[3].CTRL == 0x0000'0005u);
  MINUNIT_CHECK(dutRegisters->GPIO[4].CTRL == 0x0000'0005u);
  MINUNIT_CHECK(dutRegisters->GPIO[5].CTRL == 0x0000'0005u);
  MINUNIT_PASS();
}