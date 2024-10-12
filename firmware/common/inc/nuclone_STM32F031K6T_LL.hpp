/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef BOARD_TESTS_HPP
#define BOARD_TESTS_HPP

#define CLOCK_XTAL (12000000u)
#define CLOCK_EXT_IN (0u)
#define CLOCK_CPU (12000000u)
#define CLOCK_AHB (12000000u)
#define CLOCK_MAIN (12000000u)

#include <st/libmcu_STM32F031K6T_ll.hpp>

void boardInit(void);

#endif