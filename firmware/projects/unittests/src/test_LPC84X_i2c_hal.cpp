/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * \file test_LPC84X_i2c_hal.cpp
 *
 * Test queing for LPC84X I2C HAL
 */

#include <minunit.h>
#include <cstring>
#include <libmcu/libmcull.hpp>
#include <libmcu/libmcuhal.hpp>
#include <nxp/LPC8XX_HW/LPC84X_syscon_hw.hpp>
#include <nxp/LPC8XX_CLOCK/LPC84X_clock_hw.hpp>
#include <nxp/LPC8XX_HAL/LPC84X_i2c_hal.hpp>

constexpr inline libmcu::I2cDeviceAddress test_address{0x42};

struct MockLlI2cInterrupt : libmcull::AsyncI2cBase {
  constexpr libmcu::Results Transmit(const libmcu::I2cDeviceAddress address, const std::span<const std::uint8_t> buffer,
                                     libmcu::NonBlocking *callback) {
    counter_transmit++;
    mock_address = address;
    transmit_buffer = buffer;
    mock_callback = callback;
    return libmcu::Results::NoError;
  }
  constexpr libmcu::Results StartMasterTransmit(const libmcu::I2cDeviceAddress address, const std::span<const std::uint8_t> buffer,
                                                libmcu::NonBlocking *callback) {
    counter_transmit++;
    mock_address = address;
    transmit_buffer = buffer;
    mock_callback = callback;
    return libmcu::Results::NoError;
  }
  constexpr libmcu::Results ContinueMasterTransmit(const std::span<const std::uint8_t> buffer, libmcu::NonBlocking *callback) {
    counter_transmit++;
    transmit_buffer = buffer;
    mock_callback = callback;
    return libmcu::Results::NoError;
  }
  constexpr libmcu::Results StopMasterTransmit(const std::span<const std::uint8_t> buffer, libmcu::NonBlocking *callback) {
    counter_transmit++;
    transmit_buffer = buffer;
    mock_callback = callback;
    return libmcu::Results::NoError;
  }
  constexpr libmcu::Results Receive(const libmcu::I2cDeviceAddress address, const std::span<std::uint8_t> buffer,
                                    libmcu::NonBlocking *callback) {
    counter_receive++;
    mock_address = address;
    receive_buffer = buffer;
    mock_callback = callback;
    return libmcu::Results::NoError;
  }
  constexpr libmcu::Results StartMasterReceive(const libmcu::I2cDeviceAddress address, const std::span<std::uint8_t> buffer,
                                               libmcu::NonBlocking *callback) {
    counter_receive++;
    mock_address = address;
    receive_buffer = buffer;
    mock_callback = callback;
    return libmcu::Results::NoError;
  }
  constexpr libmcu::Results ContinueMasterReceive(const std::span<std::uint8_t> buffer, libmcu::NonBlocking *callback) {
    counter_receive++;
    receive_buffer = buffer;
    mock_callback = callback;
    return libmcu::Results::NoError;
  }
  constexpr libmcu::Results StopMasterReceive(const std::span<std::uint8_t> buffer, libmcu::NonBlocking *callback) {
    counter_receive++;
    receive_buffer = buffer;
    mock_callback = callback;
    return libmcu::Results::NoError;
  }
  void Progress(void) {
    counter_progress++;
  }
  void Callback(void) {
    if (mock_callback != nullptr)
      mock_callback->Callback();
  }
  void Reset(void) {
    counter_transmit = 0;
    counter_receive = 0;
    counter_callback = 0;
    counter_progress = 0;
  }
  int counter_transmit;
  int counter_receive;
  int counter_callback;
  int counter_progress;
  libmcu::NonBlocking *mock_callback;
  std::span<std::uint8_t> receive_buffer;        /*!< Receive buffer */
  std::span<const std::uint8_t> transmit_buffer; /*!< transmit buffer */
  libmcu::I2cDeviceAddress mock_address;
};

struct DutCallback : public libmcu::NonBlocking {
  void Progress(void) {}
  void Callback(void) {
    callback_counter++;
  }
  int callback_counter;
};

MockLlI2cInterrupt mock_ll_i2c_peripheral_int;

DutCallback transmit_callback;
DutCallback receive_callback;
DutCallback stop_callback;

// todo instantiate HAL I2C
libmcuhal::i2c::I2c<mock_ll_i2c_peripheral_int, 3> dut_hal_i2c;

MINUNIT_SETUP(setup_LPC84X_i2c_hal) {
  mock_ll_i2c_peripheral_int.Reset();
  transmit_callback.callback_counter = 0;
  receive_callback.callback_counter = 0;
  stop_callback.callback_counter = 0;
  MINUNIT_PASS();
}

MINUNIT_TEARDOWN(teardown_LPC84X_i2c_hal) {
  MINUNIT_PASS();
}

MINUNIT_ADD(test_LPC84X_i2c_hal_initial, setup_LPC84X_i2c_hal, teardown_LPC84X_i2c_hal) {
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
}

MINUNIT_ADD(test_LPC84X_i2c_hal_single_transmit, setup_LPC84X_i2c_hal, teardown_LPC84X_i2c_hal) {
  std::array<std::uint8_t, 5> test_write_data{0x88, 0x11, 0xAA, 0x55, 0xC5};
  MINUNIT_ASSERT(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(dut_hal_i2c.Transmit(test_address, test_write_data, &transmit_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(transmit_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Busy);
  MINUNIT_CHECK(transmit_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.mock_callback != nullptr);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.transmit_buffer.size() == test_write_data.size());
  MINUNIT_CHECK(std::memcmp(mock_ll_i2c_peripheral_int.transmit_buffer.data(), test_write_data.data(), test_write_data.size()) ==
                0);
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(transmit_callback.callback_counter == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 1);
}

MINUNIT_ADD(test_LPC84X_i2c_hal_multi_single_write, setup_LPC84X_i2c_hal, teardown_LPC84X_i2c_hal) {
  std::array<std::uint8_t, 5> test_write_data_first{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 4> test_write_data_second{0x22, 0x98, 0xA4, 0x68};
  MINUNIT_ASSERT(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(dut_hal_i2c.Transmit(test_address, test_write_data_first, &transmit_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(transmit_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Busy);
  MINUNIT_CHECK(transmit_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.mock_callback != nullptr);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.transmit_buffer.size() == test_write_data_first.size());
  MINUNIT_CHECK(std::memcmp(mock_ll_i2c_peripheral_int.transmit_buffer.data(), test_write_data_first.data(),
                            test_write_data_first.size()) == 0);
  MINUNIT_CHECK(dut_hal_i2c.Transmit(test_address, test_write_data_second, &transmit_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(dut_hal_i2c.Transmit(test_address, test_write_data_first, &transmit_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Busy);
  MINUNIT_CHECK(transmit_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.mock_callback != nullptr);
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(transmit_callback.callback_counter == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 1);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 2);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.transmit_buffer.size() == test_write_data_second.size());
  MINUNIT_CHECK(std::memcmp(mock_ll_i2c_peripheral_int.transmit_buffer.data(), test_write_data_second.data(),
                            test_write_data_second.size()) == 0);
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(transmit_callback.callback_counter == 2);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 2);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 3);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.transmit_buffer.size() == test_write_data_first.size());
  MINUNIT_CHECK(std::memcmp(mock_ll_i2c_peripheral_int.transmit_buffer.data(), test_write_data_first.data(),
                            test_write_data_first.size()) == 0);
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(transmit_callback.callback_counter == 3);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 3);
}

MINUNIT_ADD(test_LPC84X_i2c_hal_single_read, setup_LPC84X_i2c_hal, teardown_LPC84X_i2c_hal) {
  std::array<std::uint8_t, 5> test_read_data{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 5> test_result_data;
  test_result_data.fill(0);
  MINUNIT_ASSERT(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(dut_hal_i2c.Receive(test_address, test_result_data, &receive_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(receive_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Busy);
  MINUNIT_CHECK(receive_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.mock_callback != nullptr);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.receive_buffer.size() == test_read_data.size());
  std::memcpy(mock_ll_i2c_peripheral_int.receive_buffer.data(), test_read_data.data(), test_read_data.size());
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(std::memcmp(test_result_data.data(), test_read_data.data(), test_read_data.size()) == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(receive_callback.callback_counter == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 1);
}

MINUNIT_ADD(test_LPC84X_i2c_hal_multi_single_read, setup_LPC84X_i2c_hal, teardown_LPC84X_i2c_hal) {
  std::array<std::uint8_t, 5> test_read_data_first{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 5> test_read_data_second{0x22, 0x98, 0xA4, 0x68, 0x12};
  std::array<std::uint8_t, 5> test_result_data;
  MINUNIT_ASSERT(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(dut_hal_i2c.Receive(test_address, test_result_data, &receive_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(dut_hal_i2c.Receive(test_address, test_result_data, &receive_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(dut_hal_i2c.Receive(test_address, test_result_data, &receive_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(dut_hal_i2c.Receive(test_address, test_result_data, &receive_callback) == libmcu::Results::Full);
  MINUNIT_CHECK(receive_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Busy);
  MINUNIT_CHECK(receive_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.mock_callback != nullptr);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Busy);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.receive_buffer.size() == test_read_data_first.size());
  std::memcpy(mock_ll_i2c_peripheral_int.receive_buffer.data(), test_read_data_first.data(), test_read_data_first.size());
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(std::memcmp(test_result_data.data(), test_read_data_first.data(), test_read_data_first.size()) == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(receive_callback.callback_counter == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 1);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 2);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.transmit_buffer.size() == test_read_data_second.size());
  std::memcpy(mock_ll_i2c_peripheral_int.receive_buffer.data(), test_read_data_second.data(), test_read_data_second.size());
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(std::memcmp(test_result_data.data(), test_read_data_second.data(), test_read_data_second.size()) == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(receive_callback.callback_counter == 2);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 2);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 3);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.transmit_buffer.size() == test_read_data_first.size());
  std::memcpy(mock_ll_i2c_peripheral_int.receive_buffer.data(), test_read_data_first.data(), test_read_data_first.size());
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(std::memcmp(test_result_data.data(), test_read_data_first.data(), test_read_data_first.size()) == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(receive_callback.callback_counter == 3);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 3);
}

MINUNIT_ADD(test_LPC84X_i2c_hal_multi_multi_write, setup_LPC84X_i2c_hal, teardown_LPC84X_i2c_hal) {
  std::array<std::uint8_t, 5> test_write_data_first{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 4> test_write_data_second{0x22, 0x98, 0xA4, 0x68};
  MINUNIT_ASSERT(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(dut_hal_i2c.StartMasterTransmit(test_address, test_write_data_first) == libmcu::Results::NoError);
  MINUNIT_CHECK(dut_hal_i2c.ContinueMasterTransmit(test_write_data_second) == libmcu::Results::NoError);
  MINUNIT_CHECK(dut_hal_i2c.StopMasterTransmit(test_write_data_first, &transmit_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(transmit_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Busy);
  MINUNIT_CHECK(transmit_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.mock_callback != nullptr);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.transmit_buffer.size() == test_write_data_first.size());
  MINUNIT_CHECK(std::memcmp(mock_ll_i2c_peripheral_int.transmit_buffer.data(), test_write_data_first.data(),
                            test_write_data_first.size()) == 0);

  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Busy);
  MINUNIT_CHECK(transmit_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.mock_callback != nullptr);
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(transmit_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 1);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 2);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.transmit_buffer.size() == test_write_data_second.size());
  MINUNIT_CHECK(std::memcmp(mock_ll_i2c_peripheral_int.transmit_buffer.data(), test_write_data_second.data(),
                            test_write_data_second.size()) == 0);
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(transmit_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 2);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 3);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.transmit_buffer.size() == test_write_data_first.size());
  MINUNIT_CHECK(std::memcmp(mock_ll_i2c_peripheral_int.transmit_buffer.data(), test_write_data_first.data(),
                            test_write_data_first.size()) == 0);
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(transmit_callback.callback_counter == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_transmit == 3);
}

MINUNIT_ADD(test_LPC84X_i2c_hal_multi_multi_read, setup_LPC84X_i2c_hal, teardown_LPC84X_i2c_hal) {
  std::array<std::uint8_t, 5> test_read_data_first{0x88, 0x11, 0xAA, 0x55, 0xC5};
  std::array<std::uint8_t, 5> test_read_data_second{0x22, 0x98, 0xA4, 0x68, 0x12};
  std::array<std::uint8_t, 5> test_result_data;
  MINUNIT_ASSERT(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(dut_hal_i2c.StartMasterReceive(test_address, test_result_data) == libmcu::Results::NoError);
  MINUNIT_CHECK(dut_hal_i2c.ContinueMasterReceive(test_result_data) == libmcu::Results::NoError);
  MINUNIT_CHECK(dut_hal_i2c.StopMasterReceive(test_result_data, &receive_callback) == libmcu::Results::NoError);
  MINUNIT_CHECK(dut_hal_i2c.StopMasterReceive(test_result_data) == libmcu::Results::Full);
  MINUNIT_CHECK(receive_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Busy);
  MINUNIT_CHECK(receive_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_progress == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.mock_callback != nullptr);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Busy);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.receive_buffer.size() == test_read_data_first.size());
  std::memcpy(mock_ll_i2c_peripheral_int.receive_buffer.data(), test_read_data_first.data(), test_read_data_first.size());
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(std::memcmp(test_result_data.data(), test_read_data_first.data(), test_read_data_first.size()) == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(receive_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 1);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 2);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.transmit_buffer.size() == test_read_data_second.size());
  std::memcpy(mock_ll_i2c_peripheral_int.receive_buffer.data(), test_read_data_second.data(), test_read_data_second.size());
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(std::memcmp(test_result_data.data(), test_read_data_second.data(), test_read_data_second.size()) == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(receive_callback.callback_counter == 0);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 2);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 3);
  MINUNIT_ASSERT(mock_ll_i2c_peripheral_int.transmit_buffer.size() == test_read_data_first.size());
  std::memcpy(mock_ll_i2c_peripheral_int.receive_buffer.data(), test_read_data_first.data(), test_read_data_first.size());
  mock_ll_i2c_peripheral_int.Callback();
  MINUNIT_CHECK(std::memcmp(test_result_data.data(), test_read_data_first.data(), test_read_data_first.size()) == 0);
  MINUNIT_CHECK(dut_hal_i2c.GetStatus() == libmcu::Results::Idle);
  MINUNIT_CHECK(receive_callback.callback_counter == 1);
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 3);
  dut_hal_i2c.Progress();
  MINUNIT_CHECK(mock_ll_i2c_peripheral_int.counter_receive == 3);
}