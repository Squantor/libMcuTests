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
  /*
  @todo set pin modes
  // setup test pins to gpio mode
  pins_hal.Setup(gpio0_pin, libmcuhal::pins::DriveModes::k4Ma, libmcuhal::pins::PullModes::kNone,
                 libmcuhal::pins::speedModes::kMedium, true);
  pins_hal.Setup(gpio1_pin, libmcuhal::pins::DriveModes::k4Ma, libmcuhal::pins::PullModes::kNone,
                 libmcuhal::pins::speedModes::kMedium, true);

  gpio_hal.Init();
  */
  minUnitPass();
}
MINUNIT_ADD(RP2040HalGpioPullModes, RP2040SetupHalgpio, Rp2040Teardown) {
  gpio_bank0_hal.Setup(gpio0_pin, libmcuhal::gpio::PullModes::kPullUp);
  gpio_bank0_hal.Setup(gpio2_pin, libmcuhal::gpio::PullModes::kPullDown);
  minUnitCheck(gpio_bank0_hal.Get(gpio0_pin) == true);
  minUnitCheck(gpio_bank0_hal.Get(gpio2_pin) == false);
  gpio_bank0_hal.Setup(gpio0_pin, libmcuhal::gpio::PullModes::kPullDown);
  gpio_bank0_hal.Setup(gpio2_pin, libmcuhal::gpio::PullModes::kPullUp);
  minUnitCheck(gpio_bank0_hal.Get(gpio0_pin) == false);
  minUnitCheck(gpio_bank0_hal.Get(gpio2_pin) == true);
}

/*
MINUNIT_ADD(RP2040HalGpio, RP2040SetupHalgpio, Rp2040Teardown) {
  gpio_hal.SetInput(gpio0_pin);
  gpio_hal.SetInput(gpio1_pin);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0000u);
  gpio_hal.SetOutput(gpio1_pin);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0002u);
  gpio_hal.SetHigh(gpio1_pin);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0002u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpio_hal.GetPin(gpio1_pin) != 0);
  minUnitCheck(gpio_hal.GetPin(gpio0_pin) != 0);
  gpio_hal.SetLow(gpio1_pin);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpio_hal.GetPin(gpio1_pin) == 0);
  minUnitCheck(gpio_hal.GetPin(gpio0_pin) == 0);
  gpio_hal.SetInput(gpio1_pin);
  gpio_hal.SetOutput(gpio0_pin);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0001u);
  gpio_hal.SetPin(gpio0_pin, 1);
  minUnitCheck(sioRegisters->GPIO_OE == 0x0000'0001u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0001u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpio_hal.GetPin(gpio1_pin) != 0);
  minUnitCheck(gpio_hal.GetPin(gpio0_pin) != 0);
  gpio_hal.SetPin(gpio0_pin, 0);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpio_hal.GetPin(gpio1_pin) == 0);
  minUnitCheck(gpio_hal.GetPin(gpio0_pin) == 0);
  gpio_hal.Toggle(gpio0_pin);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0001u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpio_hal.GetPin(gpio1_pin) != 0);
  minUnitCheck(gpio_hal.GetPin(gpio0_pin) != 0);
  gpio_hal.Toggle(gpio0_pin);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpio_hal.GetPin(gpio1_pin) == 0);
  minUnitCheck(gpio_hal.GetPin(gpio0_pin) == 0);
  gpio_hal.SetInput(gpio0_pin);
  gpio_hal.SetPullMode(gpio0_pin, libmcuhal::gpio::PullModes::kNone);
  gpio_hal.SetPullMode(gpio1_pin, libmcuhal::gpio::PullModes::kPullUp);
  minUnitCheck(padsBank0Registers->GPIO[0] == 0x0000'0052u);
  minUnitCheck(padsBank0Registers->GPIO[1] == 0x0000'005Au);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpio_hal.GetPin(gpio1_pin) != 0);
  minUnitCheck(gpio_hal.GetPin(gpio0_pin) != 0);
  gpio_hal.SetPullMode(gpio1_pin, libmcuhal::gpio::PullModes::kPullDown);
  minUnitCheck(padsBank0Registers->GPIO[0] == 0x0000'0052u);
  minUnitCheck(padsBank0Registers->GPIO[1] == 0x0000'0056u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpio_hal.GetPin(gpio1_pin) == 0);
  minUnitCheck(gpio_hal.GetPin(gpio0_pin) == 0);
  gpio_hal.SetPullMode(gpio1_pin, libmcuhal::gpio::PullModes::kNone);
  gpio_hal.SetPullMode(gpio0_pin, libmcuhal::gpio::PullModes::kPullUp);
  minUnitCheck(padsBank0Registers->GPIO[0] == 0x0000'005Au);
  minUnitCheck(padsBank0Registers->GPIO[1] == 0x0000'0052u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0002u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0001u);
  minUnitCheck(gpio_hal.GetPin(gpio1_pin) != 0);
  minUnitCheck(gpio_hal.GetPin(gpio0_pin) != 0);
  gpio_hal.SetPullMode(gpio0_pin, libmcuhal::gpio::PullModes::kPullDown);
  minUnitCheck(padsBank0Registers->GPIO[0] == 0x0000'0056u);
  minUnitCheck(padsBank0Registers->GPIO[1] == 0x0000'0052u);
  minUnitCheck(sioRegisters->GPIO_OUT == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0002u) == 0x0000'0000u);
  minUnitCheck((sioRegisters->GPIO_IN & 0x0000'0001u) == 0x0000'0000u);
  minUnitCheck(gpio_hal.GetPin(gpio1_pin) == 0);
  minUnitCheck(gpio_hal.GetPin(gpio0_pin) == 0);
}*/