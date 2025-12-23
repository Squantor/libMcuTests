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
#include <MinUnit.h>
#include <cstdint>
#include <cstring>
#include <concepts>
#include <array>
#include <span>
#include "utilities.hpp"
#include <libmcu/assertions.hpp>
#include <libmcu/finoallocator.hpp>

libmcu::FinoAllocator<std::uint8_t, 10> fino_allocator_dut_u8;

MINUNIT_SETUP(FinoAllocatorSetup) {
  minUnitPass();
}

MINUNIT_TEARDOWN(FinoAllocatorTeardown) {
  minUnitPass();
}

MINUNIT_ADD(FinoAllocatorRequest, FinoAllocatorSetup, FinoAllocatorTeardown) {
  std::span<std::uint8_t> block_one = fino_allocator_dut_u8.Request(5);
  minUnitCheck(block_one.size() == 5);
  FillSpan(block_one, block_one.size());
  minUnitCheck(block_one[0] == block_one.size());
  std::span<std::uint8_t> block_two = fino_allocator_dut_u8.Request(4);
  minUnitCheck(block_two.size() == 4);
  FillSpan(block_two, block_two.size());
  minUnitCheck(block_two[0] == block_two.size());
  std::span<std::uint8_t> block_three = fino_allocator_dut_u8.Request(4);
  minUnitCheck(block_three.size() == 4);
  FillSpan(block_three, block_three.size());
  minUnitCheck(block_three[0] == block_three.size());
}