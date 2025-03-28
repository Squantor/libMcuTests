/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the RP2040 SPI
 */
#include <nuclone_RP2040_LL.hpp>
#include <MinUnit.h>
#include <RP2040_LL_teardown.hpp>
#include <common.hpp>

using namespace libmcull;

// peripheral register sets
static constexpr libmcu::hwAddressType spi0Address = libmcuhw::spi0Address;
libmcuhw::spi::spi *const spi0Registers{reinterpret_cast<libmcuhw::spi::spi *>(spi0Address)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupSPI) {
  minUnitCheck(RP2040TeardownCorrect() == true);
  resetsPeripheral.reset(libmcull::resets::IO_BANK0 | libmcull::resets::PADS_BANK0 | libmcull::resets::SPI0, 100000);
  // connect all GPIO's
  gpioBank0Peripheral.setup(spiOutPin);
  padsBank0Peripheral.setup(spiOutPin, libmcull::pads::driveModes::DRIVE_4MA, false, false, false, true);
  gpioBank0Peripheral.setup(spiInPin);
  padsBank0Peripheral.setup(spiInPin, libmcull::pads::driveModes::DRIVE_4MA, false, false, true, false);
  gpioBank0Peripheral.setup(spiSckPin);
  padsBank0Peripheral.setup(spiSckPin, libmcull::pads::driveModes::DRIVE_4MA, false, false, false, true);
  gpioBank0Peripheral.setup(spiCsPin);
  padsBank0Peripheral.setup(spiCsPin, libmcull::pads::driveModes::DRIVE_4MA, false, false, false, true);
}

MINUNIT_ADD(RP2040spiSetup, RP2040SetupSPI, RP2040Teardown) {
  std::uint32_t bitRate;
  bitRate = spiPeripheral.setupMaster(1'000'000, libmcull::spi::waveforms::CPHA0_CPOL0);
  minUnitCheck(bitRate == 1'000'000);
  minUnitCheck(spi0Registers->SSPCR0 == 0x3C00);
  bitRate = spiPeripheral.setupMaster(1'234'567, libmcull::spi::waveforms::CPHA1_CPOL1);
  minUnitCheck(bitRate == 1'250'000);
  minUnitCheck(spi0Registers->SSPCR0 == 0x30C0);
}

MINUNIT_ADD(RP2040spiComms, RP2040SetupSPI, RP2040Teardown) {
  std::array<std::uint16_t, 1> singleTransmitBuffer{0x1234};
  std::array<std::uint16_t, 4> MultiTransmitBuffer{0x0123, 0x4567, 0x89AB, 0xCDEF};
  std::array<std::uint16_t, 10> receiveBuffer;
  spiPeripheral.setupMaster(1'000'000, libmcull::spi::waveforms::CPHA0_CPOL0);
  spiPeripheral.readWrite(singleTransmitBuffer, receiveBuffer, 12);
  minUnitCheck(receiveBuffer[0] == 0x234);
  spiPeripheral.readWrite(MultiTransmitBuffer, receiveBuffer, 8);
  minUnitCheck(receiveBuffer[0] == 0x23);
  minUnitCheck(receiveBuffer[1] == 0x67);
  minUnitCheck(receiveBuffer[2] == 0xAB);
  minUnitCheck(receiveBuffer[3] == 0xEF);
  spiPeripheral.readWrite(MultiTransmitBuffer, receiveBuffer, 16);
  minUnitCheck(receiveBuffer[0] == 0x0123);
  minUnitCheck(receiveBuffer[1] == 0x4567);
  minUnitCheck(receiveBuffer[2] == 0x89AB);
  minUnitCheck(receiveBuffer[3] == 0xCDEF);
}