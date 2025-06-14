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
static constexpr libmcu::HwAddressType sioLocation = libmcuhw::kSioAddress;
libmcuhw::sio::Sio *const sioRegisters{reinterpret_cast<libmcuhw::sio::Sio *>(sioLocation)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupHalgpio) {
  gpio_bank0_hal.Init();
  gpio_bank0_peripheral.Setup(gpio0_pin);
  pads_bank0_peripheral.Setup(gpio0_pin, libmcull::pads::DriveModes::k4mA, libmcull::pads::PullModes::kNone, false, false);
  sio_gpio_peripheral.SetInput(gpio0_pin);
  gpio_bank0_peripheral.Setup(gpio1_pin);
  pads_bank0_peripheral.Setup(gpio1_pin, libmcull::pads::DriveModes::k4mA, libmcull::pads::PullModes::kNone, false, false);
  sio_gpio_peripheral.SetInput(gpio1_pin);
  gpio_bank0_peripheral.Setup(gpio2_pin);
  pads_bank0_peripheral.Setup(gpio2_pin, libmcull::pads::DriveModes::k4mA, libmcull::pads::PullModes::kNone, false, false);
  sio_gpio_peripheral.SetInput(gpio2_pin);
  gpio_bank0_peripheral.Setup(gpio3_pin);
  pads_bank0_peripheral.Setup(gpio3_pin, libmcull::pads::DriveModes::k4mA, libmcull::pads::PullModes::kNone, false, false);
  sio_gpio_peripheral.SetInput(gpio3_pin);
  minUnitPass();
}

MINUNIT_ADD(RP2040HalGpioPullModes, RP2040SetupHalgpio, Rp2040Teardown) {
  gpio_bank0_hal.SetupInput(gpio0_pin, libmcuhal::gpio::PullModes::kPullUp);
  gpio_bank0_hal.SetupInput(gpio1_pin, libmcuhal::gpio::PullModes::kNone);
  gpio_bank0_hal.SetupInput(gpio2_pin, libmcuhal::gpio::PullModes::kPullDown);
  gpio_bank0_hal.SetupInput(gpio3_pin, libmcuhal::gpio::PullModes::kNone);
  minUnitCheck(gpio_bank0_hal.GetLevel(gpio0_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(gpio2_pin) == 0);
  gpio_bank0_hal.SetupInput(gpio0_pin, libmcuhal::gpio::PullModes::kPullDown);
  gpio_bank0_hal.SetupInput(gpio2_pin, libmcuhal::gpio::PullModes::kPullUp);
  minUnitCheck(gpio_bank0_hal.GetLevel(gpio0_pin) == 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(gpio2_pin) != 0);
}

MINUNIT_ADD(RP2040HalGpioOutputTests, RP2040SetupHalgpio, Rp2040Teardown) {
  gpio_bank0_hal.SetupInput(gpio0_pin, libmcuhal::gpio::PullModes::kNone);
  gpio_bank0_hal.SetupInput(gpio1_pin, libmcuhal::gpio::PullModes::kNone);
  gpio_bank0_hal.SetupInput(gpio2_pin, libmcuhal::gpio::PullModes::kNone);
  gpio_bank0_hal.SetupInput(gpio3_pin, libmcuhal::gpio::PullModes::kNone);
  gpio_bank0_hal.SetupOutput(gpio0_pin, 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(gpio0_pin) == 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(gpio1_pin) == 0);
  gpio_bank0_hal.SetLevel(gpio0_pin, 1);
  minUnitCheck(gpio_bank0_hal.GetLevel(gpio0_pin) != 0);
  minUnitCheck(gpio_bank0_hal.GetLevel(gpio1_pin) != 0);
}
