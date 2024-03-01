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

using namespace libMcuLL::hw::i2c;
using namespace libMcuLL::sw::i2c;

constexpr inline libMcuLL::i2cDeviceAddress testExpander{0x21};
// peripheral register sets
static constexpr libMcuLL::hwAddressType i2cAddress = libMcuLL::hw::i2c0Address;
libMcuLL::hw::i2c::peripheral *const dutRegisters{reinterpret_cast<libMcuLL::hw::i2c::peripheral *>(i2cAddress)};

/**
 * @brief Gpio setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupI2cSync) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  sysconPeripheral.enablePeripheralClocks(libMcuLL::sw::syscon::peripheralClocks::I2C |
                                          libMcuLL::sw::syscon::peripheralClocks::SWM);
  swmPeriperhal.setup(i2cSclOutPin, i2cMainSclFunction);
  swmPeriperhal.setup(i2cSdaOutPin, i2cMainSdaFunction);
}

// testing inits
MINUNIT_ADD(LPC812M101DH20I2cSyncInits, LPC812M101CppSetupI2cSync, LPC812M101Teardown) {
  uint32_t actualRate = i2cPeripheral.initMaster(133630, 100);
  minUnitCheck(actualRate == 136363);
  minUnitCheck((dutRegisters->CFG & CFG::RESERVED_MASK) == CFG::MSTEN);
  minUnitCheck((dutRegisters->TIMEOUT == 1615));
}

// testing write and read
MINUNIT_ADD(LPC812M101DH20I2cSyncWriteRead, LPC812M101CppSetupI2cSync, LPC812M101Teardown) {
  std::array<std::uint8_t, 5> testWriteData{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 3> testReadData{};
  std::array<std::uint8_t, 1> testI2cExpanderOutput{0x30};
  std::array<std::uint8_t, 1> testI2cExpanderInput{};
  uint32_t actualRate = i2cPeripheral.initMaster(100000, 100);
  minUnitCheck(actualRate == 100000);
  i2cPeripheral.write(testExpander, testWriteData);
  i2cPeripheral.read(testExpander, testReadData);
  minUnitCheck(testReadData[0] == 0xC5);
  minUnitCheck(testReadData[1] == 0xC5);
  minUnitCheck(testReadData[2] == 0xC5);
  i2cPeripheral.write(testExpander, testI2cExpanderOutput);
  i2cPeripheral.read(testExpander, testI2cExpanderInput);
  minUnitCheck(testI2cExpanderInput[0] == 0x30);
}
