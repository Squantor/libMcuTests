/**
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 *
 * @file test_ringbuf.cpp
 * @brief Ring buffer tests
 */
#include <minunit.h>
#include <cstdint>
#include <cstring>
#include <stdint.h>
#include <array>
#include <libmcu/libmcu.hpp>
#include <libmcu/data_struct/ringbuffer.hpp>

static std::uint32_t assertion_counter = 0;
static const char* assertion_cstring = nullptr;

static void assertion_function(const char* message) {
  assertion_cstring = message;
  assertion_counter++;
}

static void assertion_reset(void) {
  assertion_counter = 0;
  assertion_cstring = nullptr;
}

/** @brief Assertion policy used for tests
 */
struct Assert_ringbuffer_policy {
  static constexpr bool enabled = true; /*!< Enable assertions */
  /**
   * @brief Assertion function that increments a counter
   * @param message Cause of the assertion
   */
  static void fail(const char* message) noexcept {
    assertion_function(message);
  }
};

libmcu::Ring_buffer<uint16_t, 10, Assert_ringbuffer_policy> RingBufferDutU16;

namespace {

MINUNIT_SETUP(ring_buffer_setup) {
  RingBufferDutU16.reset();
  assertion_reset();
  MINUNIT_PASS();
}

MINUNIT_ADD(ring_buffer_is_empty, ring_buffer_setup, NULL) {
  MINUNIT_CHECK(RingBufferDutU16.is_empty() == true);
  MINUNIT_CHECK(RingBufferDutU16.is_full() == false);
  MINUNIT_CHECK(assertion_counter == 0);
}

MINUNIT_ADD(ring_buffer_push_back, ring_buffer_setup, NULL) {
  uint16_t number = 42;
  MINUNIT_CHECK(RingBufferDutU16.is_empty() == true);
  MINUNIT_CHECK(RingBufferDutU16.push_back(number) == true);
  MINUNIT_CHECK(RingBufferDutU16.is_empty() == false);
  MINUNIT_CHECK(assertion_counter == 0);
}

MINUNIT_ADD(ring_buffer_push_back_full, ring_buffer_setup, NULL) {
  uint16_t number = 42;
  MINUNIT_CHECK(RingBufferDutU16.is_empty() == true);
  for (uint16_t i = 0; i < 10; i++) {
    MINUNIT_CHECK(RingBufferDutU16.push_back(i) == true);
  }
  MINUNIT_CHECK(RingBufferDutU16.push_back(number) == false);
  MINUNIT_CHECK(RingBufferDutU16.is_full() == true);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Ring_buffer.push_back: buffer is full") == 0);
}

MINUNIT_ADD(ring_buffer_push_front, ring_buffer_setup, NULL) {
  uint16_t number = 42;
  MINUNIT_CHECK(RingBufferDutU16.is_empty() == true);
  MINUNIT_CHECK(RingBufferDutU16.push_front(number) == true);
  MINUNIT_CHECK(RingBufferDutU16.is_empty() == false);
  MINUNIT_CHECK(assertion_counter == 0);
}

MINUNIT_ADD(ring_buffer_push_front_is_full, ring_buffer_setup, NULL) {
  uint16_t number = 42;

  MINUNIT_CHECK(RingBufferDutU16.is_empty() == true);
  for (uint16_t i = 0; i < 10; i++) {
    MINUNIT_CHECK(RingBufferDutU16.push_front(i) == true);
  }
  MINUNIT_CHECK(RingBufferDutU16.push_front(number) == false);
  MINUNIT_CHECK(RingBufferDutU16.is_full() == true);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Ring_buffer.push_front: buffer is full") == 0);
}

MINUNIT_ADD(ring_buffer_pop_back, ring_buffer_setup, NULL) {
  uint16_t number = 42;
  uint16_t output = 0;

  MINUNIT_CHECK(RingBufferDutU16.pop_back(output) == false);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Ring_buffer.pop_back: buffer is empty") == 0);
  MINUNIT_CHECK(RingBufferDutU16.push_front(number) == true);
  MINUNIT_CHECK(RingBufferDutU16.pop_back(output) == true);
  MINUNIT_CHECK(output == 42);
  MINUNIT_CHECK(RingBufferDutU16.pop_back(output) == false);
  MINUNIT_CHECK(assertion_counter == 2);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Ring_buffer.pop_back: buffer is empty") == 0);
  for (uint16_t i = 0; i < 10; i++) {
    RingBufferDutU16.push_front(i);
  }
  for (uint16_t i = 0; i < 10; i++) {
    MINUNIT_CHECK(RingBufferDutU16.pop_back(output) == true);
    MINUNIT_CHECK(output == i);
  }
  MINUNIT_CHECK(RingBufferDutU16.pop_back(output) == false);
  MINUNIT_CHECK(assertion_counter == 3);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Ring_buffer.pop_back: buffer is empty") == 0);
  for (uint16_t i = 0; i < 10; i++) {
    RingBufferDutU16.push_back(i);
  }
  for (uint16_t i = 0; i < 10; i++) {
    MINUNIT_CHECK(RingBufferDutU16.pop_back(output) == true);
    MINUNIT_CHECK(output == 9 - i);
  }
  MINUNIT_CHECK(RingBufferDutU16.pop_back(output) == false);
  MINUNIT_CHECK(assertion_counter == 4);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Ring_buffer.pop_back: buffer is empty") == 0);
}

MINUNIT_ADD(ring_buffer_pop_front, ring_buffer_setup, NULL) {
  uint16_t number = 42;
  uint16_t output = 0;
  MINUNIT_CHECK(RingBufferDutU16.pop_front(output) == false);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Ring_buffer.pop_front: buffer is empty") == 0);
  MINUNIT_CHECK(RingBufferDutU16.push_front(number) == true);
  MINUNIT_CHECK(RingBufferDutU16.pop_front(output) == true);
  MINUNIT_CHECK(output == 42);
  MINUNIT_CHECK(RingBufferDutU16.pop_front(output) == false);
  MINUNIT_CHECK(assertion_counter == 2);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Ring_buffer.pop_front: buffer is empty") == 0);
  for (uint16_t i = 0; i < 10; i++) {
    RingBufferDutU16.push_front(i);
  }
  for (int i = 0; i < 10; i++) {
    MINUNIT_CHECK(RingBufferDutU16.pop_front(output) == true);
    MINUNIT_CHECK(output == 9 - i);
  }
  MINUNIT_CHECK(RingBufferDutU16.pop_front(output) == false);
  MINUNIT_CHECK(assertion_counter == 3);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Ring_buffer.pop_front: buffer is empty") == 0);
  for (uint16_t i = 0; i < 10; i++) {
    RingBufferDutU16.push_back(i);
  }
  for (uint16_t i = 0; i < 10; i++) {
    MINUNIT_CHECK(RingBufferDutU16.pop_front(output) == true);
    MINUNIT_CHECK(output == i);
  }
  MINUNIT_CHECK(RingBufferDutU16.pop_front(output) == false);
  MINUNIT_CHECK(assertion_counter == 4);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Ring_buffer.pop_front: buffer is empty") == 0);
}

MINUNIT_ADD(ring_buffer_edge_check, ring_buffer_setup, NULL) {
  uint16_t number = 42;
  uint16_t output = 0;
  for (uint16_t i = 0; i < 10; i++) {
    RingBufferDutU16.push_front(i);
    RingBufferDutU16.pop_back(output);
  }
  RingBufferDutU16.push_front(number);
  MINUNIT_CHECK(RingBufferDutU16.get_level() == 1);
}

}  // namespace