/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 I2C
 */
#include <nuclone_RP2040_LL.hpp>
#include <MinUnit.h>
#include <RP2040_LL_teardown.hpp>
#include <common.hpp>

constexpr inline libmcull::I2cDeviceAddress expanderAddress{0x21}; /**< PCF8574 I2C address */
constexpr inline libmcull::I2cDeviceAddress dummyAddress{0x53};    /**< I2C address that has no device*/

// peripheral register sets
static constexpr libmcu::HwAddressType i2c0Address = libmcuhw::kI2c0Address;
libmcuhw::i2c::I2c *const i2c0Registers{reinterpret_cast<libmcuhw::i2c::I2c *>(i2c0Address)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040LLSetupI2C) {
  minUnitCheck(Rp2040Teardown_correct() == true);
  resets_peripheral.Reset(libmcull::resets::kIoBank0 | libmcull::resets::kPadsBank0 | libmcull::resets::kI2c0, 100000);
  // connect all GPIO's
  gpio_bank0_peripheral.Setup(i2c_scl_pin);
  pads_bank0_peripheral.Setup(i2c_scl_pin, libmcull::pads::DriveModes::k8mA, libmcull::pads::PullModes::kPullUp, true, true);
  gpio_bank0_peripheral.Setup(i2cSdaPin);
  pads_bank0_peripheral.Setup(i2cSdaPin, libmcull::pads::DriveModes::k8mA, libmcull::pads::PullModes::kPullUp, true, false);
}

MINUNIT_ADD(RP2040LLI2cSetup, RP2040LLSetupI2C, Rp2040Teardown) {
  minUnitCheck(400000 == i2c_polled_peripheral.setup(libmcull::i2c::I2cModes::kFast, 400000));
  // 120MHz divided by 400KHz is 300, so our low/high times need to be 300 together
  minUnitCheck(i2c0Registers->IC_FS_SCL_HCNT + i2c0Registers->IC_FS_SCL_LCNT == 300);
  minUnitCheck(i2c0Registers->IC_SDA_HOLD == 37);
}

MINUNIT_ADD(RP2040LLI2cWrite, RP2040LLSetupI2C, Rp2040Teardown) {
  std::array<std::uint8_t, 3> transmitData{0x55, 0xAA, 0x12};
  minUnitCheck(400000 == i2c_polled_peripheral.setup(libmcull::i2c::I2cModes::kFast, 400000));
  minUnitCheck(i2c_polled_peripheral.write(dummyAddress, transmitData, 0x10000u) == libmcu::Results::InvalidAddress);
  minUnitCheck(i2c0Registers->IC_STATUS == 0x06);
  minUnitCheck(i2c_polled_peripheral.write(expanderAddress, transmitData, 0x10000u) == libmcu::Results::Done);
  minUnitCheck(i2c0Registers->IC_STATUS == 0x06);
}

MINUNIT_ADD(RP2040LLI2cRead, RP2040LLSetupI2C, Rp2040Teardown) {
  std::array<std::uint8_t, 1> transmitData;
  std::array<std::uint8_t, 3> receiveData;
  minUnitCheck(100000 == i2c_polled_peripheral.setup(libmcull::i2c::I2cModes::kFast, 100000));
  minUnitCheck(i2c_polled_peripheral.read(dummyAddress, receiveData, 0x10000u) == libmcu::Results::InvalidAddress);
  transmitData[0] = 0x55;
  minUnitCheck(i2c_polled_peripheral.write(expanderAddress, transmitData, 0x10000u) == libmcu::Results::Done);
  minUnitCheck(i2c_polled_peripheral.read(expanderAddress, receiveData, 0x10000u) == libmcu::Results::Done);
  minUnitCheck(i2c0Registers->IC_STATUS == 0x06);
  // expander bit 4 and 5 are connected and 6 and 7 are connected, so one low on the pair makes both low
  minUnitCheck(receiveData[0] = 0x05);
  minUnitCheck(receiveData[1] = 0x05);
  minUnitCheck(receiveData[2] = 0x05);
  // we now turn expander bits 4 and 5 high so they should read both as high
  transmitData[0] = 0x75;
  minUnitCheck(i2c_polled_peripheral.write(expanderAddress, transmitData, 0x10000u) == libmcu::Results::Done);
  minUnitCheck(i2c_polled_peripheral.read(expanderAddress, receiveData, 0x10000u) == libmcu::Results::Done);
  minUnitCheck(i2c0Registers->IC_STATUS == 0x06);
  minUnitCheck(receiveData[0] = 0x35);
  minUnitCheck(receiveData[1] = 0x35);
  minUnitCheck(receiveData[2] = 0x35);
}
