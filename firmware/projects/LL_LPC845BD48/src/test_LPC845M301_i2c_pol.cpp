/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the low level LPC845M301 polled i2c peripheral
 */
#include <nuclone_LPC845BD48_small_LL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>
#include <common.hpp>

namespace hardware = libmcuhw::spi;
namespace lowlevel = libmcull::spi;

constexpr inline std::uint32_t kI2cTimeout{1000};
constexpr inline libmcull::I2cDeviceAddress testExpander{0x21};

// peripheral registers
static constexpr libmcu::HwAddressType i2c_address = libmcuhw::kI2c0Address; /**< peripheral address */
libmcuhw::i2c::I2c *const i2c_registers{reinterpret_cast<libmcuhw::i2c::I2c *>(i2c_address)};

/**
 * @brief I2C setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupI2cPoll) {
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::peripheral_clocks_0::kI2c0 |
                                             libmcull::syscon::peripheral_clocks_0::kSwm |
                                             libmcull::syscon::peripheral_clocks_0::kIocon,
                                           0);
  syscon_peripheral.PeripheralClockSource(libmcull::syscon::ClockSourceSelects::kI2c0, libmcull::syscon::ClockSources::kMain);
  swm_peripheral.Setup(i2c_scl_pin, i2c_main_scl_function);
  swm_peripheral.Setup(i2c_sda_pin, i2c_main_sda_function);
}

/**
 * @brief Tests I2C init functions
 */
MINUNIT_ADD(LPC845M301DH20I2cPollInit, LPC845M301SetupI2cPoll, LPC845M301Teardown) {
  minUnitCheck(i2c_polled_peripheral.InitMaster<kI2c0ClockConfig>(100000, kI2cTimeout) == 100000);
  minUnitCheck(i2c_polled_peripheral.InitMaster<kI2c0ClockConfig>(400000, kI2cTimeout) == 428571);
  // minUnitCheck(i2c_polled_peripheral.InitMaster<kI2c0ClockConfig>(400001) == 400000);
}

/**
 * @brief Tests I2C transfers that are separate
 */
MINUNIT_ADD(LPC845M301DH20I2cTxRx, LPC845M301SetupI2cPoll, LPC845M301Teardown) {
  std::array<std::uint8_t, 5> testWriteData{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 3> testReadData{};
  std::array<std::uint8_t, 1> testI2cExpanderOutput{0x30};
  std::array<std::uint8_t, 1> testI2cExpanderInput{};
  minUnitCheck(i2c_polled_peripheral.InitMaster<kI2c0ClockConfig>(100000, kI2cTimeout) == 100000);
  i2c_polled_peripheral.Transmit(testExpander, testWriteData);
  i2c_polled_peripheral.Receive(testExpander, testReadData);
  minUnitCheck(testReadData[0] == 0xC5);
  minUnitCheck(testReadData[1] == 0xC5);
  minUnitCheck(testReadData[2] == 0xC5);
  i2c_polled_peripheral.Transmit(testExpander, testI2cExpanderOutput);
  i2c_polled_peripheral.Receive(testExpander, testI2cExpanderInput);
  minUnitCheck(testI2cExpanderInput[0] == 0x30);
}
/**
 * @brief Tests I2C transfers that are split into multiple transactions
 * @todo
 */
MINUNIT_ADD(LPC845M301DH20I2cMultiTxRx, LPC845M301SetupI2cPoll, LPC845M301Teardown) {
  minUnitPass();
}