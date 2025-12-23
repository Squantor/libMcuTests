/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file utilities.hpp
 * @brief various utility function definitions
 */
#include "utilities.hpp"

void FillSpan(std::span<std::uint8_t> span, std::uint8_t value) {
  for (uint8_t& element : span) {
    element = value;
  }
}