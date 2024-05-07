/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 gpio
 */
#include <nuclone_RP2040_HAL.hpp>
#include <MinUnit.h>
#include <RP2040_HAL_teardown.hpp>
#include <common.hpp>

using namespace libMcuLL;

// peripheral register sets
static constexpr hwAddressType padsBank0Location = hw::padsBank0Address;
hw::padsBank0::peripheral *const padsBank0Registers{reinterpret_cast<hw::padsBank0::peripheral *>(padsBank0Location)};
static constexpr hwAddressType ioBank0Location = hw::ioBank0Address;
hw::gpioBank0::peripheral *const ioBank0Registers{reinterpret_cast<hw::gpioBank0::peripheral *>(ioBank0Location)};
static constexpr hwAddressType sioLocation = hw::sioAddress;
hw::sio::peripheral *const sioRegisters{reinterpret_cast<hw::sio::peripheral *>(sioLocation)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupHalgpio) {
  minUnitPass();
}

MINUNIT_ADD(RP2040HalGpio, RP2040SetupHalgpio, RP2040Teardown) {
  minUnitPass();
}