/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 gpio
 */
#include <nuclone_RP2040.hpp>
#include <MinUnit.h>
#include <RP2040_teardown.hpp>
#include <common.hpp>

using namespace libMcuLL::hw::sio;
// using namespace libMcuLL::sw::sio;

// peripheral register sets
static constexpr libMcuLL::hwAddressType dutAddress = libMcuLL::hw::sioAddress;
libMcuLL::hw::sio::peripheral *const dutRegisters{reinterpret_cast<libMcuLL::hw::sio::peripheral *>(dutAddress)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupSio) {
  // minUnitCheck(LPC812M101TeardownCorrect() == true);
  minUnitPass();
}

MINUNIT_ADD(RP2040gpio, RP2040SetupSio, NULL) {
  minUnitPass();
}