/*
SPDX-License-Identifier: MIT

Copyright (c) 2024 Bart Bilos
For conditions of distribution and use, see LICENSE file
*/
#ifndef NUCLONE_CORTEXM0PLUS_TESTS_HPP
#define NUCLONE_CORTEXM0PLUS_TESTS_HPP

#define CLOCK_XTAL (12000000u)
#define CLOCK_CPU (12000000u)

#include <generic/libmcu_CortexM0plus_ll.hpp>

extern libMcuLL::systick::systick<libMcuHw::systickAddress> systickPeripheral;
extern libMcuLL::nvic::nvic<libMcuHw::nvicAddress, libMcuHw::scbAddress> nvicPeripheral;
extern libMcuLL::scb::scb<libMcuHw::scbAddress> scbPeripheral;
extern volatile std::uint32_t systickIsrCount;

void boardInit(void);
void crudeDelay(uint32_t iterations);

#endif