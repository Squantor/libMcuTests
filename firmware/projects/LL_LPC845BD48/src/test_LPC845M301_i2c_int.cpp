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

struct DutCallback : public libmcu::NonBlocking {
  void Progress(void) {}
  void Callback(void) {
    callback_counter++;
  }
  int callback_counter;
};

DutCallback transmit_callback;
DutCallback receive_callback;
DutCallback stop_callback;

// peripheral registers
static constexpr libmcu::HwAddressType i2c_address = libmcuhw::I2c0Address; /**< peripheral address */
libmcuhw::i2c::I2c *const i2c_registers{reinterpret_cast<libmcuhw::i2c::I2c *>(i2c_address)};

/**
 * @brief I2C setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupI2cIntr) {
  MINUNIT_CHECK(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::peripheral_clocks_0::I2c0 |
                                             libmcull::syscon::peripheral_clocks_0::Swm |
                                             libmcull::syscon::peripheral_clocks_0::Iocon,
                                           0);
  syscon_peripheral.PeripheralClockSource(libmcull::syscon::ClockSourceSelects::I2c0, libmcull::syscon::ClockSources::Main);
  swm_peripheral.Setup(i2c_scl_pin, i2c_main_scl_function);
  swm_peripheral.Setup(i2c_sda_pin, i2c_main_sda_function);
  nvic_peripheral.Enable(libmcuhw::Interrupts::I2c0);
  transmit_callback.callback_counter = 0;
  receive_callback.callback_counter = 0;
}
/**
 * @brief Tests I2C init methods
 */
MINUNIT_ADD(LPC845M301DH20I2cIntrInit, LPC845M301SetupI2cIntr, LPC845M301Teardown) {
  MINUNIT_CHECK(ll_i2c_peripheral_int.InitMaster<I2c0ClockConfig>(100000, kI2cTimeout) == 100000);
  MINUNIT_CHECK(ll_i2c_peripheral_int.InitMaster<I2c0ClockConfig>(400000, kI2cTimeout) == 428571);
  MINUNIT_CHECK(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
}

/**
 * @brief Tests I2C transfers that are separate
 */
MINUNIT_ADD(LPC845M301DH20I2cIntrTxRxSingle, LPC845M301SetupI2cIntr, LPC845M301Teardown) {
  std::array<std::uint8_t, 5> test_write_data{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 3> test_read_data{};
  std::array<std::uint8_t, 1> test_i2c_expander_output{0x30};
  std::array<std::uint8_t, 1> test_i2c_expander_input{};
  std::uint32_t timeout = 0;
  test_read_data.fill(0x00);
  MINUNIT_CHECK(ll_i2c_peripheral_int.InitMaster<I2c0ClockConfig>(100000, kI2cTimeout) == 100000);
  MINUNIT_CHECK(ll_i2c_peripheral_int.Transmit(testExpander, test_write_data, &transmit_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(transmit_callback.callback_counter == 0);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::Idle) && (timeout < kI2cTimeout); timeout++)
    ll_i2c_peripheral_int.Progress();
  MINUNIT_CHECK(timeout < kI2cTimeout);
  MINUNIT_CHECK(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(transmit_callback.callback_counter == 1);

  MINUNIT_CHECK(ll_i2c_peripheral_int.Receive(testExpander, test_read_data, &receive_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(receive_callback.callback_counter == 0);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::Idle) && (timeout < kI2cTimeout); timeout++)
    ll_i2c_peripheral_int.Progress();
  MINUNIT_CHECK(timeout < kI2cTimeout);
  MINUNIT_CHECK(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(receive_callback.callback_counter == 1);

  MINUNIT_CHECK(test_read_data[0] == 0xC5);
  MINUNIT_CHECK(test_read_data[1] == 0xC5);
  MINUNIT_CHECK(test_read_data[2] == 0xC5);
  MINUNIT_CHECK(ll_i2c_peripheral_int.Transmit(testExpander, test_i2c_expander_output) == libmcu::Results::NoError);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::Idle) && (timeout < kI2cTimeout); timeout++)
    ll_i2c_peripheral_int.Progress();
  MINUNIT_CHECK(timeout < kI2cTimeout);
  MINUNIT_CHECK(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(ll_i2c_peripheral_int.Receive(testExpander, test_i2c_expander_input) == libmcu::Results::NoError);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::Idle) && (timeout < kI2cTimeout); timeout++)
    ll_i2c_peripheral_int.Progress();
  MINUNIT_CHECK(timeout < kI2cTimeout);
  MINUNIT_CHECK(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(test_i2c_expander_input[0] == 0x30);

  MINUNIT_CHECK(transmit_callback.callback_counter == 1);
  MINUNIT_CHECK(receive_callback.callback_counter == 1);
}

MINUNIT_ADD(LPC845M301DH20I2cIntrTxRxMulti, LPC845M301SetupI2cIntr, LPC845M301Teardown) {
  std::array<std::uint8_t, 5> test_write_data{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 3> test_read_data{};
  std::uint32_t timeout = 0;
  test_read_data.fill(0x00);

  MINUNIT_CHECK(ll_i2c_peripheral_int.InitMaster<I2c0ClockConfig>(100000, kI2cTimeout) == 100000);
  MINUNIT_CHECK(ll_i2c_peripheral_int.StartMasterTransmit(testExpander,
                                                          std::span<const std::uint8_t>(test_write_data).subspan(0, 2),
                                                          &transmit_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(transmit_callback.callback_counter == 0);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::WaitForNextTransmit) && (timeout < kI2cTimeout);
       timeout++)
    ll_i2c_peripheral_int.Progress();
  MINUNIT_CHECK(timeout < kI2cTimeout);
  MINUNIT_CHECK(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::WaitForNextTransmit);
  MINUNIT_CHECK(transmit_callback.callback_counter == 1);

  MINUNIT_CHECK(ll_i2c_peripheral_int.ContinueMasterTransmit(std::span<const std::uint8_t>(test_write_data).subspan(2, 2),
                                                             &transmit_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(transmit_callback.callback_counter == 1);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::WaitForNextTransmit) && (timeout < kI2cTimeout);
       timeout++)
    ll_i2c_peripheral_int.Progress();
  MINUNIT_CHECK(timeout < kI2cTimeout);
  MINUNIT_CHECK(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::WaitForNextTransmit);
  MINUNIT_CHECK(transmit_callback.callback_counter == 2);

  MINUNIT_CHECK(ll_i2c_peripheral_int.StopMasterTransmit(std::span<const std::uint8_t>(test_write_data).subspan(0, 5),
                                                         &stop_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(transmit_callback.callback_counter == 2);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::Idle) && (timeout < kI2cTimeout); timeout++)
    ll_i2c_peripheral_int.Progress();
  MINUNIT_CHECK(timeout < kI2cTimeout);
  MINUNIT_CHECK(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(stop_callback.callback_counter == 1);

  MINUNIT_CHECK(ll_i2c_peripheral_int.StartMasterReceive(testExpander, std::span<std::uint8_t>(test_read_data).subspan(0, 1),
                                                         &receive_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(receive_callback.callback_counter == 0);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::WaitForNextReceive) && (timeout < kI2cTimeout);
       timeout++)
    ll_i2c_peripheral_int.Progress();
  MINUNIT_CHECK(timeout < kI2cTimeout);
  MINUNIT_CHECK(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::WaitForNextReceive);
  MINUNIT_CHECK(receive_callback.callback_counter == 1);

  MINUNIT_CHECK(ll_i2c_peripheral_int.ContinueMasterReceive(std::span<std::uint8_t>(test_read_data).subspan(1, 1),
                                                            &receive_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(receive_callback.callback_counter == 1);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::WaitForNextReceive) && (timeout < kI2cTimeout);
       timeout++)
    ll_i2c_peripheral_int.Progress();
  MINUNIT_CHECK(timeout < kI2cTimeout);
  MINUNIT_CHECK(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::WaitForNextReceive);
  MINUNIT_CHECK(receive_callback.callback_counter == 2);

  MINUNIT_CHECK(ll_i2c_peripheral_int.StopMasterReceive(std::span<std::uint8_t>(test_read_data).subspan(2, 1), &stop_callback) ==
                libmcu::Results::NoError);
  MINUNIT_CHECK(stop_callback.callback_counter == 1);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::Idle) && (timeout < kI2cTimeout); timeout++)
    ll_i2c_peripheral_int.Progress();
  MINUNIT_CHECK(timeout < kI2cTimeout);
  MINUNIT_CHECK(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(stop_callback.callback_counter == 2);

  MINUNIT_CHECK(test_read_data[0] == 0xC5);
  MINUNIT_CHECK(test_read_data[1] == 0xC5);
  MINUNIT_CHECK(test_read_data[2] == 0xC5);
}