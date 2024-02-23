/*
SPDX-License-Identifier: MIT

Copyright (c) 2024 Bart Bilos
For conditions of distribution and use, see LICENSE file
*/
#ifndef NUCLONE_CORTEXM0PLUS_TESTS_HPP
#define NUCLONE_CORTEXM0PLUS_TESTS_HPP

#define CLOCK_XTAL (12000000u)
#define CLOCK_CPU (12000000u)

#include <mcu_ll.hpp>

extern libMcuLL::sw::systick::systick<libMcuLL::hw::systickAddress> systickPeripheral;
extern libMcuLL::sw::nvic::nvic<libMcuLL::hw::nvicAddress, libMcuLL::hw::scbAddress> nvicPeripheral;
extern libMcuLL::sw::scb::scb<libMcuLL::hw::scbAddress> scbPeripheral;
extern volatile std::uint32_t systickIsrCount;

void boardInit(void);
void crudeDelay(uint32_t iterations);

#endif