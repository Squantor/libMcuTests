/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC845M301 SPI peripheral in polling mode
 */
#include <nuclone_LPC845BD48_small_LL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>
#include <common.hpp>

namespace hardware = libmcuhw::spi;
namespace lowlevel = libmcull::spi;

// peripheral registers
static constexpr libmcu::HwAddressType spi_address = libmcuhw::Spi0Address; /**< peripheral address */
libmcuhw::spi::Spi *const spi_registers{reinterpret_cast<libmcuhw::spi::Spi *>(spi_address)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupSpiPol) {
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::peripheral_clocks_0::Spi0 |
                                             libmcull::syscon::peripheral_clocks_0::Swm |
                                             libmcull::syscon::peripheral_clocks_0::Iocon,
                                           0);
}

/**
 * @brief Tests SPI init functions
 *
 */
MINUNIT_ADD(LPC845M301DH20SpiPolInit, LPC845M301SetupSpiPol, LPC845M301Teardown) {
  minUnitCheck(spi_polled_peripheral.InitMaster<Spi0ClockConfig>(1000000) == 1000000);
  minUnitCheck(spi_polled_peripheral.InitMaster<Spi0ClockConfig>(1000001) == 1016949);
  minUnitPass();
}