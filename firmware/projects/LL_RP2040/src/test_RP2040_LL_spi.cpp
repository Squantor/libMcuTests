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
static constexpr libmcu::HwAddressType spi0Address = libmcuhw::kSpi0Address;
libmcuhw::spi::Spi *const spi0Registers{reinterpret_cast<libmcuhw::spi::Spi *>(spi0Address)};

/**
 * @brief sio setup and initialisation
 */
MINUNIT_SETUP(RP2040SetupSPI) {
  minUnitCheck(Rp2040Teardown_correct() == true);
  resets_peripheral.Reset(libmcull::resets::kIoBank0 | libmcull::resets::kPadsBank0 | libmcull::resets::kSpi0, 100000);
  // connect all GPIO's
  gpio_bank0_peripheral.Setup(spiOutPin);
  pads_bank0_peripheral.Setup(spiOutPin, libmcull::pads::DriveModes::k4mA, libmcull::pads::PullModes::kNone, false, true);
  gpio_bank0_peripheral.Setup(spiInPin);
  pads_bank0_peripheral.Setup(spiInPin, libmcull::pads::DriveModes::k4mA, libmcull::pads::PullModes::kNone, true, false);
  gpio_bank0_peripheral.Setup(spiSckPin);
  pads_bank0_peripheral.Setup(spiSckPin, libmcull::pads::DriveModes::k4mA, libmcull::pads::PullModes::kNone, false, true);
  gpio_bank0_peripheral.Setup(spiCsPin);
  pads_bank0_peripheral.Setup(spiCsPin, libmcull::pads::DriveModes::k4mA, libmcull::pads::PullModes::kNone, false, true);
}

MINUNIT_ADD(RP2040spiSetup, RP2040SetupSPI, Rp2040Teardown) {
  std::uint32_t bitRate;
  bitRate = spi_polled_peripheral.SetupMaster(1'000'000, libmcull::spi::Waveforms::kCpha0Cpol0);
  minUnitCheck(bitRate == 1'000'000);
  minUnitCheck(spi0Registers->SSPCR0 == 0x3C00);
  bitRate = spi_polled_peripheral.SetupMaster(1'234'567, libmcull::spi::Waveforms::kCpha1Cpol1);
  minUnitCheck(bitRate == 1'250'000);
  minUnitCheck(spi0Registers->SSPCR0 == 0x30C0);
}

MINUNIT_ADD(RP2040spiComms, RP2040SetupSPI, Rp2040Teardown) {
  std::array<std::uint16_t, 1> singleTransmitBuffer{0x1234};
  std::array<std::uint16_t, 4> MultiTransmitBuffer{0x0123, 0x4567, 0x89AB, 0xCDEF};
  std::array<std::uint16_t, 10> receiveBuffer;
  spi_polled_peripheral.SetupMaster(1'000'000, libmcull::spi::Waveforms::kCpha0Cpol0);
  spi_polled_peripheral.Transceive(singleTransmitBuffer, receiveBuffer, 12);
  minUnitCheck(receiveBuffer[0] == 0x234);
  spi_polled_peripheral.Transceive(MultiTransmitBuffer, receiveBuffer, 8);
  minUnitCheck(receiveBuffer[0] == 0x23);
  minUnitCheck(receiveBuffer[1] == 0x67);
  minUnitCheck(receiveBuffer[2] == 0xAB);
  minUnitCheck(receiveBuffer[3] == 0xEF);
  spi_polled_peripheral.Transceive(MultiTransmitBuffer, receiveBuffer, 16);
  minUnitCheck(receiveBuffer[0] == 0x0123);
  minUnitCheck(receiveBuffer[1] == 0x4567);
  minUnitCheck(receiveBuffer[2] == 0x89AB);
  minUnitCheck(receiveBuffer[3] == 0xCDEF);
}