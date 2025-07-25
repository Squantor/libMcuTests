/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file test_LPC845M301_async_i2c.cpp
 * @brief tests for LPC845M301 HAL I2C interrupt mode
 */

#include <nuclone_LPC845BD48_small_HAL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>

using namespace libmcuhw::i2c;
using namespace libmcuhal::i2c;

static constexpr libmcuhw::HwAddressType i2c0_address = libmcuhw::I2c0Address;
libmcuhw::i2c::I2c *const dut_registers{reinterpret_cast<libmcuhw::i2c::I2c *>(i2c0_address)};

constexpr inline std::uint32_t kI2cTimeout{1000};
constexpr inline libmcull::I2cDeviceAddress testExpander{0x21};

/**
 * @brief LPC845M301 HAL I2C setup
 */
MINUNIT_SETUP(Lpc845m301SetupI2C) {
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::peripheral_clocks_0::I2c0 |
                                             libmcull::syscon::peripheral_clocks_0::Swm |
                                             libmcull::syscon::peripheral_clocks_0::Iocon,
                                           0);
  swm_peripheral.Setup(i2c_scl_pin, i2c_main_scl_function);
  swm_peripheral.Setup(i2c_sda_pin, i2c_main_sda_function);
  nvic_peripheral.Enable(libmcuhw::Interrupts::I2c0);
}

/**
 * @brief Tests I2C HAL init functionality
 */
MINUNIT_ADD(Lpc845m301I2cIntInit, Lpc845m301SetupI2C, LPC845M301Teardown) {
  std::uint32_t realBaudRate;
  syscon_peripheral.PeripheralClockSource(libmcull::syscon::ClockSourceSelects::I2c0, libmcull::syscon::ClockSources::Main);
  realBaudRate = hal_i2c_interrupt_peripheral.Init<i2c0_clock_config>(400000, kI2cTimeout);
  minUnitCheck(realBaudRate == 428571);
  /*
  @todo check registers
  minUnitCheck((dut_registers->CFG & hardware::CFG::RESERVED_MASK) ==
               (hardware::CFG::ENABLE | hardware::CFG::DATALEN8BIT | hardware::CFG::PARITY_NONE | hardware::CFG::STOPBIT1));
  dut_registers->CFG = 0x00000000;
  */
}

/**
 * @brief Tests I2C HAL claim and unclaim functionality
 */
MINUNIT_ADD(Lpc845m301I2cIntClaimUnclaim, Lpc845m301SetupI2C, LPC845M301Teardown) {
  libmcu::AsyncHandle handle_first = 42;
  libmcu::AsyncHandle handle_second = 44;

  minUnitCheck(hal_i2c_interrupt_peripheral.Claim(handle_first) == libmcu::Results::Claimed);
  minUnitCheck(handle_first == 0);
  minUnitCheck(hal_i2c_interrupt_peripheral.Claim(handle_second) == libmcu::Results::InUse);
  minUnitCheck(hal_i2c_interrupt_peripheral.Unclaim(handle_first) == libmcu::Results::Unclaimed);
  minUnitCheck(hal_i2c_interrupt_peripheral.Unclaim(handle_first) == libmcu::Results::InUse);
  minUnitCheck(hal_i2c_interrupt_peripheral.Claim(handle_second) == libmcu::Results::Claimed);
  minUnitCheck(handle_second == 1);
  minUnitCheck(hal_i2c_interrupt_peripheral.Unclaim(handle_second) == libmcu::Results::Unclaimed);
}

/**
 * @brief Tests I2C transfers that are separate
 */
MINUNIT_ADD(Lpc845m301I2cIntTxRx, Lpc845m301SetupI2C, LPC845M301Teardown) {
  std::array<std::uint8_t, 5> testWriteData{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 3> testReadData{};
  std::array<std::uint8_t, 1> testI2cExpanderOutput{0x30};
  std::array<std::uint8_t, 1> testI2cExpanderInput{};
  std::uint32_t timeout;
  libmcu::AsyncHandle handle = 42;
  minUnitCheck(hal_i2c_interrupt_peripheral.Init<i2c0_clock_config>(100000, kI2cTimeout) == 100000);
  minUnitCheck(hal_i2c_interrupt_peripheral.Claim(handle) == libmcu::Results::Claimed);
  minUnitCheck(handle == 0);
  minUnitCheck(hal_i2c_interrupt_peripheral.Transmit(handle, testExpander, testWriteData) == libmcu::Results::BusyTransmit);
  timeout = 0;
  for (timeout = 0; (hal_i2c_interrupt_peripheral.GetStatus(handle) == libmcu::Results::BusyTransmit) && (timeout < kI2cTimeout);
       timeout++)
    ;
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(hal_i2c_interrupt_peripheral.GetStatus(handle) == libmcu::Results::Claimed);

  minUnitCheck(hal_i2c_interrupt_peripheral.Receive(handle, testExpander, testReadData) == libmcu::Results::BusyReceive);
  for (timeout = 0; (hal_i2c_interrupt_peripheral.GetStatus(handle) == libmcu::Results::BusyReceive) && (timeout < kI2cTimeout);
       timeout++)
    ;
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(hal_i2c_interrupt_peripheral.GetStatus(handle) == libmcu::Results::Claimed);

  minUnitCheck(testReadData[0] == 0xC5);
  minUnitCheck(testReadData[1] == 0xC5);
  minUnitCheck(testReadData[2] == 0xC5);
  minUnitCheck(hal_i2c_interrupt_peripheral.Transmit(handle, testExpander, testI2cExpanderOutput) == libmcu::Results::BusyTransmit);
  for (timeout = 0; (hal_i2c_interrupt_peripheral.GetStatus(handle) == libmcu::Results::BusyTransmit) && (timeout < kI2cTimeout);
       timeout++)
    ;
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(hal_i2c_interrupt_peripheral.GetStatus(handle) == libmcu::Results::Claimed);
  minUnitCheck(hal_i2c_interrupt_peripheral.Receive(handle, testExpander, testI2cExpanderInput) == libmcu::Results::BusyReceive);
  for (timeout = 0; (hal_i2c_interrupt_peripheral.GetStatus(handle) == libmcu::Results::BusyReceive) && (timeout < kI2cTimeout);
       timeout++)
    ;
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(hal_i2c_interrupt_peripheral.GetStatus(handle) == libmcu::Results::Claimed);
  minUnitCheck(testI2cExpanderInput[0] == 0x30);
  minUnitCheck(hal_i2c_interrupt_peripheral.Unclaim(handle) == libmcu::Results::Unclaimed);
}