/**
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 *
 * @file test_fidfoallocator.cpp
 * @brief Tests for the FIDFO allocator
 * @todo Create a test so the front falls on the end of the buffer (is that possible?)
 * @todo Create test so you can check wrapping
 * @todo we could use a fuzzing test here
 */
#include <minunit.h>
#include <cstdint>
#include <cstring>
#include <concepts>
#include <array>
#include <span>
#include "utilities.hpp"
#include <libmcu/assertions.hpp>
#include <libmcu/fidfoallocator.hpp>

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
struct Assert_fidfoallocator_policy {
  static constexpr bool enabled = true; /*!< Enable assertions */
  /**
   * @brief Assertion function that increments a counter
   * @param message Cause of the assertion
   */
  static void fail(const char* message) noexcept {
    assertion_function(message);
  }
};

libmcu::Fidfo_allocator<std::uint8_t, 10, Assert_fidfoallocator_policy> fidfo_allocator_dut_u8;

MINUNIT_SETUP(fidfo_allocator_setup) {
  assertion_reset();
  fidfo_allocator_dut_u8.reset();
  MINUNIT_PASS();
}

MINUNIT_TEARDOWN(fidfo_allocator_teardown) {
  MINUNIT_PASS();
}

MINUNIT_ADD(FifoAllocatorEmpty, fidfo_allocator_setup, fidfo_allocator_teardown) {
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_empty() == true);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_full() == false);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 0);
}

MINUNIT_ADD(FifoAllocatorGetBlocks, fidfo_allocator_setup, fidfo_allocator_teardown) {
  std::span<std::uint8_t> block_one = fidfo_allocator_dut_u8.request(5);
  MINUNIT_CHECK(block_one.size() == 5);
  fill_span(block_one, block_one.size());
  MINUNIT_CHECK(block_one[0] == block_one.size());
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_empty() == false);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_full() == false);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 5);
  std::span<std::uint8_t> block_two = fidfo_allocator_dut_u8.request(2);
  MINUNIT_CHECK(block_two.size() == 2);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_empty() == false);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_full() == false);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 7);
  std::span<std::uint8_t> block_three = fidfo_allocator_dut_u8.request(3);
  MINUNIT_CHECK(block_three.size() == 3);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_empty() == false);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_full() == true);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 10);
  MINUNIT_CHECK(assertion_counter == 0);
}

MINUNIT_ADD(FifoAllocatorRequestAndRelease, fidfo_allocator_setup, fidfo_allocator_teardown) {
  std::span<std::uint8_t> block_one = fidfo_allocator_dut_u8.request(3);
  MINUNIT_CHECK(block_one.size() == 3);
  fill_span(block_one, block_one.size());
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 3);
  fidfo_allocator_dut_u8.release(block_one);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_empty() == true);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_full() == false);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 0);
  MINUNIT_CHECK(assertion_counter == 0);
}

MINUNIT_ADD(FifoAllocatorBarelyNoWrap, fidfo_allocator_setup, fidfo_allocator_teardown) {
  std::span<std::uint8_t> block_one = fidfo_allocator_dut_u8.request(7);
  fill_span(block_one, block_one.size());
  fidfo_allocator_dut_u8.release(block_one);
  std::span<std::uint8_t> block_two = fidfo_allocator_dut_u8.request(4);
  fill_span(block_two, block_two.size());
  MINUNIT_CHECK(block_two.size() == 4);
  std::span<std::uint8_t> block_three = fidfo_allocator_dut_u8.request(3);
  MINUNIT_CHECK(block_three.size() == 3);
  fill_span(block_three, block_three.size());
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_empty() == false);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_full() == false);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 7);
  MINUNIT_CHECK(assertion_counter == 0);
}

MINUNIT_ADD(FifoAllocatorRequestAndReleaseWithWrapping, fidfo_allocator_setup, fidfo_allocator_teardown) {
  std::span<std::uint8_t> block_one = fidfo_allocator_dut_u8.request(7);
  std::span<std::uint8_t> block_two = fidfo_allocator_dut_u8.request(2);
  fidfo_allocator_dut_u8.release(block_one);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 2);
  std::span<std::uint8_t> block_three = fidfo_allocator_dut_u8.request(5);
  // 5+2 is not 9 but due to wrapping loss we get 9
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 9);
  MINUNIT_CHECK(assertion_counter == 0);
  fill_span(block_three, block_three.size());
  MINUNIT_CHECK(block_three[0] == block_three.size());
}

MINUNIT_ADD(FifoAllocatorOutOfSpaceNoWrap, fidfo_allocator_setup, fidfo_allocator_teardown) {
  std::span<std::uint8_t> block_one = fidfo_allocator_dut_u8.request(7);
  MINUNIT_CHECK(block_one.size() == 7);
  std::span<std::uint8_t> block_two = fidfo_allocator_dut_u8.request(3);
  MINUNIT_CHECK(block_two.size() == 3);
  std::span<std::uint8_t> block_three = fidfo_allocator_dut_u8.request(3);
  MINUNIT_CHECK(block_three.size() == 0);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_empty() == false);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_full() == true);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 10);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Fidfo_allocator.request: buffer is full") == 0);
}

MINUNIT_ADD(FifoAllocatorOutOfSpaceWrapping, fidfo_allocator_setup, fidfo_allocator_teardown) {
  std::span<std::uint8_t> block_one = fidfo_allocator_dut_u8.request(7);
  MINUNIT_CHECK(block_one.size() == 7);
  std::span<std::uint8_t> block_two = fidfo_allocator_dut_u8.request(2);
  MINUNIT_CHECK(block_two.size() == 2);
  std::span<std::uint8_t> block_three = fidfo_allocator_dut_u8.request(3);
  MINUNIT_CHECK(block_three.size() == 0);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_empty() == false);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.is_full() == false);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 9);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Fidfo_allocator.request: buffer is full") == 0);
}

MINUNIT_ADD(FifoAllocatorIncorrectRelease, fidfo_allocator_setup, fidfo_allocator_teardown) {
  std::span<std::uint8_t> block_one = fidfo_allocator_dut_u8.request(7);
  MINUNIT_CHECK(block_one.size() == 7);
  std::array<std::uint8_t, 3> block_alien;
  fidfo_allocator_dut_u8.release(block_alien);
  MINUNIT_CHECK(assertion_counter == 2);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Fidfo_allocator.release: block does not belong to used space") == 0);
}

MINUNIT_ADD(FifoAllocatorOutOfOrderRelease, fidfo_allocator_setup, fidfo_allocator_teardown) {
  std::span<std::uint8_t> block_one = fidfo_allocator_dut_u8.request(1);
  std::span<std::uint8_t> block_two = fidfo_allocator_dut_u8.request(2);
  std::span<std::uint8_t> block_three = fidfo_allocator_dut_u8.request(3);
  fill_span(block_one, block_one.size());
  fill_span(block_two, block_two.size());
  fill_span(block_three, block_three.size());
  fidfo_allocator_dut_u8.release(block_two);
  MINUNIT_CHECK(assertion_counter == 0);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 3);
}

MINUNIT_ADD(FifoAllocatorOutOfOrderReleaseWithWrapping, fidfo_allocator_setup, fidfo_allocator_teardown) {
  std::span<std::uint8_t> block_one = fidfo_allocator_dut_u8.request(4);
  fill_span(block_one, 1);
  fidfo_allocator_dut_u8.release(block_one);
  std::span<std::uint8_t> block_two = fidfo_allocator_dut_u8.request(3);
  std::span<std::uint8_t> block_three = fidfo_allocator_dut_u8.request(3);
  std::span<std::uint8_t> block_four = fidfo_allocator_dut_u8.request(3);
  fill_span(block_two, 2);
  fill_span(block_three, 3);
  fill_span(block_four, 4);
  fidfo_allocator_dut_u8.release(block_three);
  // this should be 3 but is 4 as the indices have no notion of blocks, not a problem as the back pointer is used
  // as a fill level guard and nothing more
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 4);
  fidfo_allocator_dut_u8.release(block_four);
  MINUNIT_CHECK(fidfo_allocator_dut_u8.get_level() == 0);
  MINUNIT_CHECK(assertion_counter == 0);
}
