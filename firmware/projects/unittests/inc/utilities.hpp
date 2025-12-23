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
#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <cstdint>
#include <span>
#include <array>

void FillSpan(std::span<std::uint8_t> span, std::uint8_t value);

#endif