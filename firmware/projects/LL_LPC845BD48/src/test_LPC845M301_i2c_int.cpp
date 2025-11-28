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
constexpr inline libmcu::I2cDeviceAddress testExpander{0x21};

struct DutCallback : public libmcu::AsyncInterface {
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
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
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
  minUnitCheck(ll_i2c_peripheral_int.InitMaster<I2c0ClockConfig>(100000, kI2cTimeout) == 100000);
  minUnitCheck(ll_i2c_peripheral_int.InitMaster<I2c0ClockConfig>(400000, kI2cTimeout) == 428571);
  minUnitCheck(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
}

/**
 * @brief Tests I2C transfers that are separate
 */
MINUNIT_ADD(LPC845M301DH20I2cIntrTxRxSingle, LPC845M301SetupI2cIntr, LPC845M301Teardown) {
  std::array<std::uint8_t, 5> testWriteData{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 3> testReadData{};
  std::array<std::uint8_t, 1> testI2cExpanderOutput{0x30};
  std::array<std::uint8_t, 1> testI2cExpanderInput{};
  std::uint32_t timeout = 0;
  testReadData.fill(0x00);
  minUnitCheck(ll_i2c_peripheral_int.InitMaster<I2c0ClockConfig>(100000, kI2cTimeout) == 100000);
  minUnitCheck(ll_i2c_peripheral_int.Transmit(testExpander, testWriteData, &transmit_callback) == libmcu::Results::NoError);
  minUnitCheck(transmit_callback.callback_counter == 0);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::Idle) && (timeout < kI2cTimeout); timeout++)
    ll_i2c_peripheral_int.Progress();
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
  minUnitCheck(transmit_callback.callback_counter == 1);

  minUnitCheck(ll_i2c_peripheral_int.Receive(testExpander, testReadData, &receive_callback) == libmcu::Results::NoError);
  minUnitCheck(receive_callback.callback_counter == 0);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::Idle) && (timeout < kI2cTimeout); timeout++)
    ll_i2c_peripheral_int.Progress();
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
  minUnitCheck(receive_callback.callback_counter == 1);

  minUnitCheck(testReadData[0] == 0xC5);
  minUnitCheck(testReadData[1] == 0xC5);
  minUnitCheck(testReadData[2] == 0xC5);
  minUnitCheck(ll_i2c_peripheral_int.Transmit(testExpander, testI2cExpanderOutput) == libmcu::Results::NoError);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::Idle) && (timeout < kI2cTimeout); timeout++)
    ll_i2c_peripheral_int.Progress();
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
  minUnitCheck(ll_i2c_peripheral_int.Receive(testExpander, testI2cExpanderInput) == libmcu::Results::NoError);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::Idle) && (timeout < kI2cTimeout); timeout++)
    ll_i2c_peripheral_int.Progress();
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
  minUnitCheck(testI2cExpanderInput[0] == 0x30);

  minUnitCheck(transmit_callback.callback_counter == 1);
  minUnitCheck(receive_callback.callback_counter == 1);
}

MINUNIT_ADD(LPC845M301DH20I2cIntrTxRxMulti, LPC845M301SetupI2cIntr, LPC845M301Teardown) {
  std::array<std::uint8_t, 5> testWriteData{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 3> testReadData{};
  std::uint32_t timeout = 0;
  testReadData.fill(0x00);

  minUnitCheck(ll_i2c_peripheral_int.InitMaster<I2c0ClockConfig>(100000, kI2cTimeout) == 100000);
  minUnitCheck(ll_i2c_peripheral_int.StartMasterTransmit(testExpander, std::span<const std::uint8_t>(testWriteData).subspan(0, 3),
                                                         &transmit_callback) == libmcu::Results::NoError);
  minUnitCheck(transmit_callback.callback_counter == 0);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::WaitForNextTransmit) && (timeout < kI2cTimeout);
       timeout++)
    ll_i2c_peripheral_int.Progress();
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::WaitForNextTransmit);
  minUnitCheck(transmit_callback.callback_counter == 1);

  minUnitCheck(ll_i2c_peripheral_int.ContinueMasterTransmit(std::span<const std::uint8_t>(testWriteData).subspan(3, 2),
                                                            &transmit_callback) == libmcu::Results::NoError);
  minUnitCheck(transmit_callback.callback_counter == 1);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::WaitForNextTransmit) && (timeout < kI2cTimeout);
       timeout++)
    ll_i2c_peripheral_int.Progress();
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::WaitForNextTransmit);
  minUnitCheck(transmit_callback.callback_counter == 2);

  minUnitCheck(ll_i2c_peripheral_int.StopMaster(&stop_callback) == libmcu::Results::NoError);
  minUnitCheck(transmit_callback.callback_counter == 2);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::Idle) && (timeout < kI2cTimeout); timeout++)
    ll_i2c_peripheral_int.Progress();
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
  minUnitCheck(stop_callback.callback_counter == 1);

  minUnitCheck(ll_i2c_peripheral_int.StartMasterReceive(testExpander, std::span<std::uint8_t>(testReadData).subspan(0, 2),
                                                        &receive_callback) == libmcu::Results::NoError);
  minUnitCheck(receive_callback.callback_counter == 0);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::WaitForNextReceive) && (timeout < kI2cTimeout);
       timeout++)
    ll_i2c_peripheral_int.Progress();
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::WaitForNextReceive);
  minUnitCheck(receive_callback.callback_counter == 1);

  minUnitCheck(ll_i2c_peripheral_int.ContinueMasterReceive(std::span<std::uint8_t>(testReadData).subspan(2, 1),
                                                           &receive_callback) == libmcu::Results::NoError);
  minUnitCheck(receive_callback.callback_counter == 1);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::WaitForNextReceive) && (timeout < kI2cTimeout);
       timeout++)
    ll_i2c_peripheral_int.Progress();
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::WaitForNextReceive);
  minUnitCheck(receive_callback.callback_counter == 2);

  minUnitCheck(ll_i2c_peripheral_int.StopMaster(&stop_callback) == libmcu::Results::NoError);
  minUnitCheck(stop_callback.callback_counter == 1);
  for (timeout = 0; (ll_i2c_peripheral_int.GetStatus() != libmcu::Results::Idle) && (timeout < kI2cTimeout); timeout++)
    ll_i2c_peripheral_int.Progress();
  minUnitCheck(timeout < kI2cTimeout);
  minUnitCheck(ll_i2c_peripheral_int.GetStatus() == libmcu::Results::Idle);
  minUnitCheck(stop_callback.callback_counter == 2);

  minUnitCheck(testReadData[0] == 0xC5);
  minUnitCheck(testReadData[1] == 0xC5);
  minUnitCheck(testReadData[2] == 0xC5);
}