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
  pinsHal.Init();
  // setup test pins to gpio mode
  pinsHal.Setup(gpio0Pin, libmcuhal::pins::DriveModes::k4Ma, libmcuhal::pins::PullModes::kNone,
                libmcuhal::pins::speedModes::kMedium, true);
  pinsHal.Setup(gpio1Pin, libmcuhal::pins::DriveModes::k4Ma, libmcuhal::pins::PullModes::kNone,
                libmcuhal::pins::speedModes::kMedium, true);
  gpioHal.Init();
  minUnitPass();
}

MINUNIT_ADD(RP2040HalGpio, RP2040SetupHalgpio, RP2040Teardown) {
  gpioHal.SetInput(gpio0Pin);
  gpioHal.SetInput(gpio1Pin);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0000u);
  gpioHal.SetOutput(gpio1Pin);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0002u);
  gpioHal.SetHigh(gpio1Pin);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0002u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpioHal.GetPin(gpio1Pin) != 0);
  minUnitCheck(gpioHal.GetPin(gpio0Pin) != 0);
  gpioHal.SetLow(gpio1Pin);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpioHal.GetPin(gpio1Pin) == 0);
  minUnitCheck(gpioHal.GetPin(gpio0Pin) == 0);
  gpioHal.SetInput(gpio1Pin);
  gpioHal.SetOutput(gpio0Pin);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0001u);
  gpioHal.SetPin(gpio0Pin, 1);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0001u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0001u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpioHal.GetPin(gpio1Pin) != 0);
  minUnitCheck(gpioHal.GetPin(gpio0Pin) != 0);
  gpioHal.SetPin(gpio0Pin, 0);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpioHal.GetPin(gpio1Pin) == 0);
  minUnitCheck(gpioHal.GetPin(gpio0Pin) == 0);
  gpioHal.Toggle(gpio0Pin);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0001u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpioHal.GetPin(gpio1Pin) != 0);
  minUnitCheck(gpioHal.GetPin(gpio0Pin) != 0);
  gpioHal.Toggle(gpio0Pin);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpioHal.GetPin(gpio1Pin) == 0);
  minUnitCheck(gpioHal.GetPin(gpio0Pin) == 0);
  gpioHal.SetInput(gpio0Pin);
  gpioHal.SetPullMode(gpio0Pin, libmcuhal::gpio::PullModes::kNone);
  gpioHal.SetPullMode(gpio1Pin, libmcuhal::gpio::PullModes::kPullUp);
  minUnitCheck(padsBank0Registers->GPIO[0] == 0x0000'0052u);
  minUnitCheck(padsBank0Registers->GPIO[1] == 0x0000'005Au);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpioHal.GetPin(gpio1Pin) != 0);
  minUnitCheck(gpioHal.GetPin(gpio0Pin) != 0);
  gpioHal.SetPullMode(gpio1Pin, libmcuhal::gpio::PullModes::kPullDown);
  minUnitCheck(padsBank0Registers->GPIO[0] == 0x0000'0052u);
  minUnitCheck(padsBank0Registers->GPIO[1] == 0x0000'0056u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpioHal.GetPin(gpio1Pin) == 0);
  minUnitCheck(gpioHal.GetPin(gpio0Pin) == 0);
  gpioHal.SetPullMode(gpio1Pin, libmcuhal::gpio::PullModes::kNone);
  gpioHal.SetPullMode(gpio0Pin, libmcuhal::gpio::PullModes::kPullUp);
  minUnitCheck(padsBank0Registers->GPIO[0] == 0x0000'005Au);
  minUnitCheck(padsBank0Registers->GPIO[1] == 0x0000'0052u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpioHal.GetPin(gpio1Pin) != 0);
  minUnitCheck(gpioHal.GetPin(gpio0Pin) != 0);
  gpioHal.SetPullMode(gpio0Pin, libmcuhal::gpio::PullModes::kPullDown);
  minUnitCheck(padsBank0Registers->GPIO[0] == 0x0000'0056u);
  minUnitCheck(padsBank0Registers->GPIO[1] == 0x0000'0052u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpioHal.GetPin(gpio1Pin) == 0);
  minUnitCheck(gpioHal.GetPin(gpio0Pin) == 0);
}