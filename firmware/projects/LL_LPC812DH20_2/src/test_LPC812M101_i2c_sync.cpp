/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC812M101 UART peripheral polling mode
 */

#include <nuclone_LPC812M101DH20_tests.hpp>
#include <MinUnit.h>
#include <LPC812M101_teardown.hpp>
#include <common.hpp>

using namespace libmcuhw::i2c;
using namespace libmcull::i2c;

constexpr inline libmcull::I2cDeviceAddress testExpander{0x21};
// peripheral register sets
static constexpr libmcu::HwAddressType i2cAddress = libmcuhw::kI2c0Address;
libmcuhw::i2c::I2c *const dutRegisters{reinterpret_cast<libmcuhw::i2c::I2c *>(i2cAddress)};

/**
 * @brief Gpio setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupI2cSync) {
  minUnitCheck(Lpc812M101TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::kClockI2c |
                                           libmcull::syscon::PeripheralClocks::kClockSwm);
  swm_peripheral.setup(i2c_scl_out_pin, i2c_main_scl_function);
  swm_peripheral.setup(i2c_sda_out_pin, i2c_main_sda_function);
}

// testing inits
MINUNIT_ADD(LPC812M101DH20I2cSyncInits, LPC812M101CppSetupI2cSync, LPC812M101Teardown) {
  uint32_t actualRate = i2c_peripheral.InitMaster(133630, 100);
  minUnitCheck(actualRate == 136363);
  minUnitCheck((dutRegisters->CFG & CFG::RESERVED_MASK) == CFG::kMSTEN);
  minUnitCheck((dutRegisters->TIMEOUT == 1615));
}

// testing write and read
MINUNIT_ADD(LPC812M101DH20I2cSyncWriteRead, LPC812M101CppSetupI2cSync, LPC812M101Teardown) {
  std::array<std::uint8_t, 5> testWriteData{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 3> testReadData{};
  std::array<std::uint8_t, 1> testI2cExpanderOutput{0x30};
  std::array<std::uint8_t, 1> testI2cExpanderInput{};
  uint32_t actualRate = i2c_peripheral.InitMaster(100000, 100);
  minUnitCheck(actualRate == 100000);
  i2c_peripheral.Write(testExpander, testWriteData);
  i2c_peripheral.Read(testExpander, testReadData);
  minUnitCheck(testReadData[0] == 0xC5);
  minUnitCheck(testReadData[1] == 0xC5);
  minUnitCheck(testReadData[2] == 0xC5);
  i2c_peripheral.Write(testExpander, testI2cExpanderOutput);
  i2c_peripheral.Read(testExpander, testI2cExpanderInput);
  minUnitCheck(testI2cExpanderInput[0] == 0x30);
}
