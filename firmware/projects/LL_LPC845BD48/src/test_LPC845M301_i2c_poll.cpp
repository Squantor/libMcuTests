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
#include <minunit.h>
#include <LPC845M301_teardown.hpp>
#include <common.hpp>

namespace hardware = libmcuhw::spi;
namespace lowlevel = libmcull::spi;

constexpr inline std::uint32_t kI2cTimeout{1000};
constexpr inline libmcu::I2cDeviceAddress testExpander{0x21};

// peripheral registers
static constexpr libmcu::HwAddressType i2c_address = libmcuhw::I2c0Address; /**< peripheral address */
libmcuhw::i2c::I2c *const i2c_registers{reinterpret_cast<libmcuhw::i2c::I2c *>(i2c_address)};

/**
 * @brief I2C setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupI2cPoll) {
  MINUNIT_CHECK(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::peripheral_clocks_0::I2c0 |
                                             libmcull::syscon::peripheral_clocks_0::Swm |
                                             libmcull::syscon::peripheral_clocks_0::Iocon,
                                           0);
  syscon_peripheral.PeripheralClockSource(libmcull::syscon::ClockSourceSelects::I2c0, libmcull::syscon::ClockSources::Main);
  swm_peripheral.Setup(i2c_scl_pin, i2c_main_scl_function);
  swm_peripheral.Setup(i2c_sda_pin, i2c_main_sda_function);
}

/**
 * @brief Tests I2C init functions
 */
MINUNIT_ADD(LPC845M301DH20I2cPollInit, LPC845M301SetupI2cPoll, LPC845M301Teardown) {
  MINUNIT_CHECK(i2c_polled_peripheral.InitMaster<I2c0ClockConfig>(100000, kI2cTimeout) == 100000);
  MINUNIT_CHECK(i2c_polled_peripheral.InitMaster<I2c0ClockConfig>(400000, kI2cTimeout) == 428571);
  // MINUNIT_CHECK(i2c_polled_peripheral.InitMaster<I2c0ClockConfig>(400001) == 400000);
}

/**
 * @brief Tests I2C transfers that are separate
 */
MINUNIT_ADD(LPC845M301DH20I2cTxRx, LPC845M301SetupI2cPoll, LPC845M301Teardown) {
  std::array<std::uint8_t, 5> test_write_data{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 3> test_read_data{};
  std::array<std::uint8_t, 1> test_i2c_expander_output{0x30};
  std::array<std::uint8_t, 1> test_i2c_expander_input{};
  MINUNIT_CHECK(i2c_polled_peripheral.InitMaster<I2c0ClockConfig>(100000, kI2cTimeout) == 100000);
  i2c_polled_peripheral.Transmit(testExpander, test_write_data);
  i2c_polled_peripheral.Receive(testExpander, test_read_data);
  MINUNIT_CHECK(test_read_data[0] == 0xC5);
  MINUNIT_CHECK(test_read_data[1] == 0xC5);
  MINUNIT_CHECK(test_read_data[2] == 0xC5);
  i2c_polled_peripheral.Transmit(testExpander, test_i2c_expander_output);
  i2c_polled_peripheral.Receive(testExpander, test_i2c_expander_input);
  MINUNIT_CHECK(test_i2c_expander_input[0] == 0x30);
}
/**
 * @brief Tests I2C transfers that are split into multiple transactions
 * @todo
 */
MINUNIT_ADD(LPC845M301DH20I2cMultiTxRx, LPC845M301SetupI2cPoll, LPC845M301Teardown) {
  MINUNIT_PASS();
}