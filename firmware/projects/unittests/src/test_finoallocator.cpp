/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * \file test_finoallocator.cpp
 *
 * All tests for the FINO allocator
 */
#include <minunit.h>
#include <cstdint>
#include <cstring>
#include <concepts>
#include <array>
#include <span>
#include "utilities.hpp"
#include <libmcu/assertions.hpp>
#include <libmcu/finoallocator.hpp>

libmcu::Fino_allocator<std::uint8_t, 10> fino_allocator_dut_u8;

MINUNIT_SETUP(FinoAllocatorSetup) {
  MINUNIT_PASS();
}

MINUNIT_TEARDOWN(FinoAllocatorTeardown) {
  MINUNIT_PASS();
}

MINUNIT_ADD(FinoAllocatorRequest, FinoAllocatorSetup, FinoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = fino_allocator_dut_u8.request(5);
  MINUNIT_CHECK(block_one.size() == 5);
  fill_span(block_one, block_one.size());
  MINUNIT_CHECK(block_one[0] == block_one.size());
  std::span<std::uint8_t> block_two = fino_allocator_dut_u8.request(4);
  MINUNIT_CHECK(block_two.size() == 4);
  fill_span(block_two, block_two.size());
  MINUNIT_CHECK(block_two[0] == block_two.size());
  std::span<std::uint8_t> block_three = fino_allocator_dut_u8.request(4);
  MINUNIT_CHECK(block_three.size() == 4);
  fill_span(block_three, block_three.size());
  MINUNIT_CHECK(block_three[0] == block_three.size());
}