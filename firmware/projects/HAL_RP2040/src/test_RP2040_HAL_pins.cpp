/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 gpio
 */
#include <nuclone_RP2040_HAL.hpp>
#include <minunit.h>
#include <RP2040_HAL_teardown.hpp>
#include <common.hpp>

// peripheral register sets
static constexpr libmcu::HwAddressType pads_bank0_location = libmcuhw::kPadsBank0Address;
libmcuhw::padsBank0::PadsBank0 *const pads_bank0_registers{reinterpret_cast<libmcuhw::padsBank0::PadsBank0 *>(pads_bank0_location)};
static constexpr libmcu::HwAddressType io_bank0_location = libmcuhw::kIoBank0Address;
libmcuhw::gpio_bank0::GpioBank0 *const io_bank0_registers{reinterpret_cast<libmcuhw::gpio_bank0::GpioBank0 *>(io_bank0_location)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupHalPins) {
  pins_hal.Init();
  MINUNIT_PASS();
}

MINUNIT_ADD(RP2040HalPins, RP2040SetupHalPins, Rp2040Teardown) {
  pins_hal.Setup(gpio0_pin, libmcuhal::pins::DriveModes::k8Ma, libmcuhal::pins::PullModes::kPullUp,
                 libmcuhal::pins::speedModes::KSlow, true);
  MINUNIT_CHECK(pads_bank0_registers->GPIO[0] == (libmcuhw::pads::GPIO::RESERVED_MASK & 0x0000'006Au));
  MINUNIT_CHECK(io_bank0_registers->GPIO[0].CTRL == (libmcuhw::gpio_bank0::CTRL::RESERVED_MASK & 0x0000'0005u));
  pins_hal.Reset(gpio0_pin);
  MINUNIT_CHECK(pads_bank0_registers->GPIO[0] == (libmcuhw::pads::GPIO::RESERVED_MASK & 0x0000'0056u));
  MINUNIT_CHECK(io_bank0_registers->GPIO[0].CTRL == (libmcuhw::gpio_bank0::CTRL::RESERVED_MASK & 0x0000'001Fu));
  pins_hal.Setup(spi_sck_pin, libmcuhal::pins::DriveModes::k4Ma, libmcuhal::pins::PullModes::kNone,
                 libmcuhal::pins::speedModes::kFast, false);
  MINUNIT_CHECK(pads_bank0_registers->GPIO[2] == (libmcuhw::pads::GPIO::RESERVED_MASK & 0x0000'0051u));
  MINUNIT_CHECK(io_bank0_registers->GPIO[2].CTRL == (libmcuhw::gpio_bank0::CTRL::RESERVED_MASK & 0x0000'0001u));
  pins_hal.Reset(spi_sck_pin);
  MINUNIT_CHECK(pads_bank0_registers->GPIO[2] == (libmcuhw::pads::GPIO::RESERVED_MASK & 0x0000'0056u));
  MINUNIT_CHECK(io_bank0_registers->GPIO[2].CTRL == (libmcuhw::gpio_bank0::CTRL::RESERVED_MASK & 0x0000'001Fu));
  MINUNIT_PASS();
}