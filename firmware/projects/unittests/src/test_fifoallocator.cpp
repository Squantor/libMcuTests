/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * \file test_fifoallocator.cpp
 *
 * All tests for the FIFO allocator
 */
#include <minunit.h>
#include <cstdint>
#include <cstring>
#include <concepts>
#include <array>
#include <span>
#include "utilities.hpp"
#include <libmcu/assertions.hpp>
#include <libmcu/fifoallocator.hpp>

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

/** @brief Assertion function used for tests
 */
struct Assert_fifoallocator_policy {
  static constexpr bool enabled = true; /*!< Enable assertions */
  /**
   * @brief Assertion function that increments a counter
   * @param message Cause of the assertion
   */
  static void fail(const char* message) noexcept {
    assertion_function(message);
  }
};

libmcu::Fifo_allocator<std::uint8_t, 10, Assert_fifoallocator_policy> ring_block_buffer_dut_u8;

MINUNIT_SETUP(FifoAllocatorSetup) {
  assertion_reset();
  ring_block_buffer_dut_u8.reset();
  MINUNIT_PASS();
}

MINUNIT_TEARDOWN(FifoAllocatorTeardown) {
  MINUNIT_PASS();
}

MINUNIT_ADD(FifoAllocatorEmpty, FifoAllocatorSetup, FifoAllocatorTeardown) {
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_empty() == true);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_full() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.get_level() == 0);
}

MINUNIT_ADD(FifoAllocatorGetBlocks, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.request(5);
  MINUNIT_CHECK(block_one.size() == 5);
  fill_span(block_one, block_one.size());
  MINUNIT_CHECK(block_one[0] == block_one.size());
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_empty() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_full() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.get_level() == 5);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.request(2);
  MINUNIT_CHECK(block_two.size() == 2);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_empty() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_full() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.get_level() == 7);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.request(3);
  MINUNIT_CHECK(block_three.size() == 3);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_empty() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_full() == true);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.get_level() == 10);
  MINUNIT_CHECK(assertion_counter == 0);
}

MINUNIT_ADD(FifoAllocatorRequestAndRelease, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.request(3);
  MINUNIT_CHECK(block_one.size() == 3);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.get_level() == 3);
  ring_block_buffer_dut_u8.release(block_one);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_empty() == true);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_full() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.get_level() == 0);
  MINUNIT_CHECK(assertion_counter == 0);
}

MINUNIT_ADD(FifoAllocatorBarelyNoWrap, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.request(7);
  fill_span(block_one, block_one.size());
  ring_block_buffer_dut_u8.release(block_one);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.request(4);
  fill_span(block_two, block_two.size());
  MINUNIT_CHECK(block_two.size() == 4);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.request(3);
  MINUNIT_CHECK(block_three.size() == 3);
  fill_span(block_three, block_three.size());
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_empty() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_full() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.get_level() == 7);
  MINUNIT_CHECK(assertion_counter == 0);
}

MINUNIT_ADD(FifoAllocatorRequestAndReleaseWithWrapping, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.request(7);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.request(2);
  ring_block_buffer_dut_u8.release(block_one);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.get_level() == 2);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.request(5);
  // 5+2 is not 9 but due to wrapping loss we get 9
  MINUNIT_CHECK(ring_block_buffer_dut_u8.get_level() == 9);
  MINUNIT_CHECK(assertion_counter == 0);
  fill_span(block_three, block_three.size());
  MINUNIT_CHECK(block_three[0] == block_three.size());
}

MINUNIT_ADD(FifoAllocatorOutOfSpaceNoWrap, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.request(7);
  MINUNIT_CHECK(block_one.size() == 7);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.request(3);
  MINUNIT_CHECK(block_two.size() == 3);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.request(3);
  MINUNIT_CHECK(block_three.size() == 0);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_empty() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_full() == true);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.get_level() == 10);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Fifo_allocator.request: buffer is full") == 0);
}

MINUNIT_ADD(FifoAllocatorOutOfSpaceWrapping, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.request(7);
  MINUNIT_CHECK(block_one.size() == 7);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.request(2);
  MINUNIT_CHECK(block_two.size() == 2);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.request(3);
  MINUNIT_CHECK(block_three.size() == 0);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_empty() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.is_full() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.get_level() == 9);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Fifo_allocator.request: buffer is full") == 0);
}

MINUNIT_ADD(FifoAllocatorIncorrectRelease, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.request(7);
  MINUNIT_CHECK(block_one.size() == 7);
  std::array<std::uint8_t, 3> block_alien;
  ring_block_buffer_dut_u8.release(block_alien);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Fifo_allocator.release: does not match back index") == 0);
}

MINUNIT_ADD(FifoAllocatorOutOfOrderRelease, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.request(3);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.request(3);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.request(3);
  ring_block_buffer_dut_u8.release(block_two);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "Fifo_allocator.release: does not match back index") == 0);
}
