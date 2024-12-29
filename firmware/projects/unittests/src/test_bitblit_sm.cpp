/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file test_bitblit_sm.cpp
 * @brief bitblit tests for smaller source then destination data size
 */

#include <MinUnit.h>
#include <cstring>
#include <cstdint>
#include <array>
#include <libmcu/libmcu_functions.hpp>
#include <libmcu/bitmap/bitblit1d.hpp>