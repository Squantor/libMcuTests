/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC812M101 SPI peripheral polling mode
 */
#include <nuclone_LPC812M101DH20_tests.hpp>
#include <MinUnit.h>
#include <LPC812M101_teardown.hpp>
#include <common.hpp>
#include <array>

using namespace libmcull::spi;
using namespace libmcuhw::spi;

static constexpr libmcu::HwAddressType spi0Address = libmcuhw::Spi0Address;
libmcuhw::spi::Spi *const dutRegisters{reinterpret_cast<libmcuhw::spi::Spi *>(spi0Address)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupSpiSync) {
  minUnitCheck(Lpc812M101TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::ClockSpi0 |
                                           libmcull::syscon::PeripheralClocks::ClockSwm |
                                           libmcull::syscon::PeripheralClocks::ClockIocon);
}

MINUNIT_ADD(LPC812M101CppSpiSyncInits, LPC812M101CppSetupSpiSync, LPC812M101Teardown) {
  uint32_t actualClock;
  actualClock = spi_polled_peripheral.InitMaster(100000);
  minUnitCheck(actualClock == 100000);
  minUnitCheck((dutRegisters->CFG & CFG::RESERVED_MASK) == 0x00000005);
  minUnitCheck(dutRegisters->DIV == 299);
  dutRegisters->CFG = 0x00000000;
  actualClock = spi_polled_peripheral.InitMaster(65399, Waveforms::Cpha1Cpol1Lsb, SlavePolaritySelects::High);
  minUnitCheck(actualClock == 65502);
  minUnitCheck((dutRegisters->CFG & CFG::RESERVED_MASK) == 0x0000013D);
  minUnitCheck(dutRegisters->DIV == 457);
}

MINUNIT_ADD(LPC812M101CppSpiSyncRxTx, LPC812M101CppSetupSpiSync, LPC812M101Teardown) {
  swm_peripheral.setup(test_2_pin, spi_main_sck_function);
  swm_peripheral.setup(test_1_pin, spi_main_mosi_function);
  swm_peripheral.setup(test_0_pin, spi_main_miso_function);
  spi_polled_peripheral.InitMaster(1000000);
  minUnitCheck((dutRegisters->STAT & STAT::RESERVED_MASK) == 0x00000102);
  minUnitCheck(dutRegisters->DIV == 29);

  std::array<uint16_t, 5> testDataSend{0x1234, 0x4567, 0x89AB, 0xCDEF, 0x5A5A};
  std::array<uint16_t, 5> testDataReceive;
  testDataReceive.fill(0x0000u);
  // test "simple"(below 17 bits) transaction
  spi_polled_peripheral.Transceive(ChipEnables::None, testDataSend, testDataReceive, 8, true);
  minUnitCheck((testDataSend[0] & 0xFF) == testDataReceive[0]);
  // test "multi"(above 17 bits) transaction to test multi transfers
  testDataReceive.fill(0x0000u);
  spi_polled_peripheral.Transceive(ChipEnables::None, testDataSend, testDataReceive, 24, true);
  minUnitCheck(testDataSend[0] == testDataReceive[0]);
  minUnitCheck((testDataSend[1] & 0xFF) == testDataReceive[1]);
  // test receive
  // disable MOSI
  swm_peripheral.clear(test_1_pin, spi_main_mosi_function);
  gpio_peripheral.SetOutput(test_1_pin);
  gpio_peripheral.SetHigh(test_1_pin);
  // enable pullup, read all ones
  spi_polled_peripheral.Receive(ChipEnables::None, testDataReceive, 12, true);
  minUnitCheck(0xFFFu == testDataReceive[0]);
  // enable pulldown, read all zeroes
  gpio_peripheral.SetLow(test_1_pin);
  testDataReceive[0] = 0xFFFF;
  spi_polled_peripheral.Receive(ChipEnables::None, testDataReceive, 12, true);
  minUnitCheck(0x0u == testDataReceive[0]);
}