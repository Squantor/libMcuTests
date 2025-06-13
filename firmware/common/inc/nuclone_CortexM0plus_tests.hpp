/*
SPDX-License-Identifier: MIT

Copyright (c) 2024 Bart Bilos
For conditions of distribution and use, see LICENSE file
*/
#ifndef NUCLONE_CORTEXM0PLUS_TESTS_HPP
#define NUCLONE_CORTEXM0PLUS_TESTS_HPP

#define CLOCK_XTAL (12000000u)
#define CLOCK_CPU (12000000u)

#include <arm/libmcu_CortexM0plus_ll.hpp>

extern libmcull::systick::Systick<libmcuhw::kSystickAddress> systick_peripheral;
extern libmcull::nvic::Nvic<libmcuhw::kNvicAddress, libmcuhw::kScbAddress> nvic_peripheral;
extern libmcull::scb::Scb<libmcuhw::kScbAddress> scb_peripheral;
extern volatile std::uint32_t systick_count;

void board_init(void);
void CrudeDelay(uint32_t iterations);

#endif