/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * \file test_ringallocator.cpp
 *
 * All tests for the ring allocator
 */
#include <MinUnit.h>
#include <cstdint>
#include <cstring>
#include <concepts>
#include <array>
#include <span>
#include <libmcu/assertions.hpp>
#include <libmcu/ringallocator.hpp>

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

libmcu::RingAllocator<std::uint8_t, 10, TestAssert> ring_block_buffer_dut_u8;

void FillSpan(std::span<std::uint8_t> span, std::uint8_t value) {
  for (uint8_t& element : span) {
    element = value;
  }
}

MINUNIT_SETUP(RingAllocatorSetup) {
  AssertionReset();
  ring_block_buffer_dut_u8.Reset();
  minUnitPass();
}

MINUNIT_TEARDOWN(RingAllocatorTeardown) {
  minUnitPass();
}

MINUNIT_ADD(RingAllocatorEmpty, RingAllocatorSetup, RingAllocatorTeardown) {
  minUnitCheck(ring_block_buffer_dut_u8.IsEmpty() == true);
  minUnitCheck(ring_block_buffer_dut_u8.IsFull() == false);
  minUnitCheck(ring_block_buffer_dut_u8.GetLevel() == 0);
}

MINUNIT_ADD(RingAllocatorGetBlocks, RingAllocatorSetup, RingAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(5);
  minUnitCheck(block_one.size() == 5);
  FillSpan(block_one, block_one.size());
  minUnitCheck(block_one[0] == block_one.size());
  minUnitCheck(ring_block_buffer_dut_u8.IsEmpty() == false);
  minUnitCheck(ring_block_buffer_dut_u8.IsFull() == false);
  minUnitCheck(ring_block_buffer_dut_u8.GetLevel() == 5);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.Request(2);
  minUnitCheck(block_two.size() == 2);
  minUnitCheck(ring_block_buffer_dut_u8.IsEmpty() == false);
  minUnitCheck(ring_block_buffer_dut_u8.IsFull() == false);
  minUnitCheck(ring_block_buffer_dut_u8.GetLevel() == 7);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.Request(3);
  minUnitCheck(block_three.size() == 3);
  minUnitCheck(ring_block_buffer_dut_u8.IsEmpty() == false);
  minUnitCheck(ring_block_buffer_dut_u8.IsFull() == true);
  minUnitCheck(ring_block_buffer_dut_u8.GetLevel() == 10);
  minUnitCheck(assertion_counter == 0);
}

MINUNIT_ADD(RingAllocatorRequestAndRelease, RingAllocatorSetup, RingAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(3);
  minUnitCheck(block_one.size() == 3);
  minUnitCheck(ring_block_buffer_dut_u8.GetLevel() == 3);
  ring_block_buffer_dut_u8.Release(block_one);
  minUnitCheck(ring_block_buffer_dut_u8.IsEmpty() == true);
  minUnitCheck(ring_block_buffer_dut_u8.IsFull() == false);
  minUnitCheck(ring_block_buffer_dut_u8.GetLevel() == 0);
  minUnitCheck(assertion_counter == 0);
}

MINUNIT_ADD(RingAllocatorBarelyNoWrap, RingAllocatorSetup, RingAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(7);
  FillSpan(block_one, block_one.size());
  ring_block_buffer_dut_u8.Release(block_one);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.Request(4);
  FillSpan(block_two, block_two.size());
  minUnitCheck(block_two.size() == 4);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.Request(3);
  minUnitCheck(block_three.size() == 3);
  FillSpan(block_three, block_three.size());
  minUnitCheck(ring_block_buffer_dut_u8.IsEmpty() == false);
  minUnitCheck(ring_block_buffer_dut_u8.IsFull() == false);
  minUnitCheck(ring_block_buffer_dut_u8.GetLevel() == 7);
  minUnitCheck(assertion_counter == 0);
}

MINUNIT_ADD(RingAllocatorRequestAndReleaseWithWrapping, RingAllocatorSetup, RingAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(7);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.Request(2);
  ring_block_buffer_dut_u8.Release(block_one);
  minUnitCheck(ring_block_buffer_dut_u8.GetLevel() == 2);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.Request(5);
  // 5+2 is not 9 but due to wrapping loss we get 9
  minUnitCheck(ring_block_buffer_dut_u8.GetLevel() == 9);
  minUnitCheck(assertion_counter == 0);
  FillSpan(block_three, block_three.size());
  minUnitCheck(block_three[0] == block_three.size());
}

MINUNIT_ADD(RingAllocatorOutOfSpaceNoWrap, RingAllocatorSetup, RingAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(7);
  minUnitCheck(block_one.size() == 7);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.Request(3);
  minUnitCheck(block_two.size() == 3);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.Request(3);
  minUnitCheck(block_three.size() == 0);
  minUnitCheck(ring_block_buffer_dut_u8.IsEmpty() == false);
  minUnitCheck(ring_block_buffer_dut_u8.IsFull() == true);
  minUnitCheck(ring_block_buffer_dut_u8.GetLevel() == 10);
  minUnitCheck(assertion_counter == 1);
  minUnitAssert(assertion_cstring != nullptr);
  minUnitCheck(std::strcmp(assertion_cstring, "RingAllocator::Request: buffer is full") == 0);
}

MINUNIT_ADD(RingAllocatorOutOfSpaceWrapping, RingAllocatorSetup, RingAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(7);
  minUnitCheck(block_one.size() == 7);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.Request(2);
  minUnitCheck(block_two.size() == 2);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.Request(3);
  minUnitCheck(block_three.size() == 0);
  minUnitCheck(ring_block_buffer_dut_u8.IsEmpty() == false);
  minUnitCheck(ring_block_buffer_dut_u8.IsFull() == false);
  minUnitCheck(ring_block_buffer_dut_u8.GetLevel() == 9);
  minUnitCheck(assertion_counter == 1);
  minUnitAssert(assertion_cstring != nullptr);
  minUnitCheck(std::strcmp(assertion_cstring, "RingAllocator::Request: buffer is full") == 0);
}

MINUNIT_ADD(RingAllocatorIncorrectRelease, RingAllocatorSetup, RingAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(7);
  minUnitCheck(block_one.size() == 7);
  std::array<std::uint8_t, 3> block_alien;
  ring_block_buffer_dut_u8.Release(block_alien);
  minUnitCheck(assertion_counter == 1);
  minUnitAssert(assertion_cstring != nullptr);
  minUnitCheck(std::strcmp(assertion_cstring, "RingAllocator::Release: does not match back index") == 0);
}

MINUNIT_ADD(RingAllocatorOutOfOrderRelease, RingAllocatorSetup, RingAllocatorTeardown) {
  std::span<std::uint8_t> block_one = ring_block_buffer_dut_u8.Request(3);
  std::span<std::uint8_t> block_two = ring_block_buffer_dut_u8.Request(3);
  std::span<std::uint8_t> block_three = ring_block_buffer_dut_u8.Request(3);
  ring_block_buffer_dut_u8.Release(block_two);
  minUnitCheck(assertion_counter == 1);
  minUnitAssert(assertion_cstring != nullptr);
  minUnitCheck(std::strcmp(assertion_cstring, "RingAllocator::Release: does not match back index") == 0);
}
