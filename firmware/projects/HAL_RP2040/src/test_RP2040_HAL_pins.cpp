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
#include <MinUnit.h>
#include <RP2040_HAL_teardown.hpp>
#include <common.hpp>

// peripheral register sets
static constexpr libmcu::HwAddressType padsBank0Location = libmcuhw::kPadsBank0Address;
libmcuhw::padsBank0::PadsBank0 *const padsBank0Registers{reinterpret_cast<libmcuhw::padsBank0::PadsBank0 *>(padsBank0Location)};
static constexpr libmcu::HwAddressType ioBank0Location = libmcuhw::kIoBank0Address;
libmcuhw::gpio_bank0::GpioBank0 *const ioBank0Registers{reinterpret_cast<libmcuhw::gpio_bank0::GpioBank0 *>(ioBank0Location)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupHalPins) {
  pinsHal.Init();
  minUnitPass();
}

MINUNIT_ADD(RP2040HalPins, RP2040SetupHalPins, RP2040Teardown) {
  pinsHal.Setup(gpio0Pin, libmcuhal::pins::DriveModes::k8Ma, libmcuhal::pins::PullModes::kPullUp,
                libmcuhal::pins::speedModes::KSlow, true);
  minUnitCheck(padsBank0Registers->GPIO[0] == (libmcuhw::pads::GPIO::RESERVED_MASK & 0x0000'006Au));
  minUnitCheck(ioBank0Registers->GPIO[0].CTRL == (libmcuhw::gpio_bank0::CTRL::RESERVED_MASK & 0x0000'0005u));
  pinsHal.Reset(gpio0Pin);
  minUnitCheck(padsBank0Registers->GPIO[0] == (libmcuhw::pads::GPIO::RESERVED_MASK & 0x0000'0056u));
  minUnitCheck(ioBank0Registers->GPIO[0].CTRL == (libmcuhw::gpio_bank0::CTRL::RESERVED_MASK & 0x0000'001Fu));
  pinsHal.Setup(spiSckPin, libmcuhal::pins::DriveModes::k4Ma, libmcuhal::pins::PullModes::kNone, libmcuhal::pins::speedModes::kFast,
                false);
  minUnitCheck(padsBank0Registers->GPIO[2] == (libmcuhw::pads::GPIO::RESERVED_MASK & 0x0000'0051u));
  minUnitCheck(ioBank0Registers->GPIO[2].CTRL == (libmcuhw::gpio_bank0::CTRL::RESERVED_MASK & 0x0000'0001u));
  pinsHal.Reset(spiSckPin);
  minUnitCheck(padsBank0Registers->GPIO[2] == (libmcuhw::pads::GPIO::RESERVED_MASK & 0x0000'0056u));
  minUnitCheck(ioBank0Registers->GPIO[2].CTRL == (libmcuhw::gpio_bank0::CTRL::RESERVED_MASK & 0x0000'001Fu));
  minUnitPass();
}