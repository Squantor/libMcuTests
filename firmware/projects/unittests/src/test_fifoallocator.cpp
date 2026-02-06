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

std::uint32_t assertion_counter = 0;
const char* assertion_cstring = nullptr;

void AssertionFunction(const char* message) {
  assertion_cstring = message;
  assertion_counter++;
}

void AssertionReset(void) {
  assertion_counter = 0;
  assertion_cstring = nullptr;
}

/** @brief Assertion function used for tests
 */
struct TestAssert {
  /**
   * @brief Assertion function that loops forever
   * @param message Cause of the assertion
   */
  void operator()(const char* message) const noexcept {
    AssertionFunction(message);
  }
};

libmcu::FifoAllocator<std::uint8_t, 10, TestAssert> ring_block_buffer_dut_u8;

MINUNIT_SETUP(FifoAllocatorSetup) {
  AssertionReset();
  ring_block_buffer_dut_u8.Reset();
  MINUNIT_PASS();
}

MINUNIT_TEARDOWN(FifoAllocatorTeardown) {
  MINUNIT_PASS();
}

MINUNIT_ADD(FifoAllocatorEmpty, FifoAllocatorSetup, FifoAllocatorTeardown) {
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsEmpty() == true);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsFull() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.GetLevel() == 0);
}

MINUNIT_ADD(FifoAllocatorGetBlocks, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(5);
  MINUNIT_CHECK(block_one.size() == 5);
  FillSpan(block_one, block_one.size());
  MINUNIT_CHECK(block_one[0] == block_one.size());
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsEmpty() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsFull() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.GetLevel() == 5);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.Request(2);
  MINUNIT_CHECK(block_two.size() == 2);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsEmpty() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsFull() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.GetLevel() == 7);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.Request(3);
  MINUNIT_CHECK(block_three.size() == 3);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsEmpty() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsFull() == true);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.GetLevel() == 10);
  MINUNIT_CHECK(assertion_counter == 0);
}

MINUNIT_ADD(FifoAllocatorRequestAndRelease, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(3);
  MINUNIT_CHECK(block_one.size() == 3);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.GetLevel() == 3);
  ring_block_buffer_dut_u8.Release(block_one);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsEmpty() == true);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsFull() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.GetLevel() == 0);
  MINUNIT_CHECK(assertion_counter == 0);
}

MINUNIT_ADD(FifoAllocatorBarelyNoWrap, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(7);
  FillSpan(block_one, block_one.size());
  ring_block_buffer_dut_u8.Release(block_one);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.Request(4);
  FillSpan(block_two, block_two.size());
  MINUNIT_CHECK(block_two.size() == 4);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.Request(3);
  MINUNIT_CHECK(block_three.size() == 3);
  FillSpan(block_three, block_three.size());
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsEmpty() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsFull() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.GetLevel() == 7);
  MINUNIT_CHECK(assertion_counter == 0);
}

MINUNIT_ADD(FifoAllocatorRequestAndReleaseWithWrapping, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(7);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.Request(2);
  ring_block_buffer_dut_u8.Release(block_one);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.GetLevel() == 2);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.Request(5);
  // 5+2 is not 9 but due to wrapping loss we get 9
  MINUNIT_CHECK(ring_block_buffer_dut_u8.GetLevel() == 9);
  MINUNIT_CHECK(assertion_counter == 0);
  FillSpan(block_three, block_three.size());
  MINUNIT_CHECK(block_three[0] == block_three.size());
}

MINUNIT_ADD(FifoAllocatorOutOfSpaceNoWrap, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(7);
  MINUNIT_CHECK(block_one.size() == 7);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.Request(3);
  MINUNIT_CHECK(block_two.size() == 3);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.Request(3);
  MINUNIT_CHECK(block_three.size() == 0);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsEmpty() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsFull() == true);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.GetLevel() == 10);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "FifoAllocator::Request: buffer is full") == 0);
}

MINUNIT_ADD(FifoAllocatorOutOfSpaceWrapping, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(7);
  MINUNIT_CHECK(block_one.size() == 7);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.Request(2);
  MINUNIT_CHECK(block_two.size() == 2);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.Request(3);
  MINUNIT_CHECK(block_three.size() == 0);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsEmpty() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.IsFull() == false);
  MINUNIT_CHECK(ring_block_buffer_dut_u8.GetLevel() == 9);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "FifoAllocator::Request: buffer is full") == 0);
}

MINUNIT_ADD(FifoAllocatorIncorrectRelease, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(7);
  MINUNIT_CHECK(block_one.size() == 7);
  std::array<std::uint8_t, 3> block_alien;
  ring_block_buffer_dut_u8.Release(block_alien);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "FifoAllocator::Release: does not match back index") == 0);
}

MINUNIT_ADD(FifoAllocatorOutOfOrderRelease, FifoAllocatorSetup, FifoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(3);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.Request(3);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.Request(3);
  ring_block_buffer_dut_u8.Release(block_two);
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "FifoAllocator::Release: does not match back index") == 0);
}
