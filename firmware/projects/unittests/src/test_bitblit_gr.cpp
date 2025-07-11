/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file test_bitblit_gr.cpp
 * @brief bitblit tests for greater source then destination data size
 */

#include <MinUnit.h>
#include <cstring>
#include <cstdint>
#include <array>
#include <libmcu/general_functions.hpp>
#include <libmcu/bitmap/bitblit1d.hpp>