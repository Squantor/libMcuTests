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

using namespace libMcuLL::sw::spi;
using namespace libMcuLL::hw::spi;

static constexpr libMcuLL::hwAddressType spi0Address = libMcuLL::hw::spi0Address;
libMcuLL::hw::spi::peripheral *const dutRegisters{reinterpret_cast<libMcuLL::hw::spi::peripheral *>(spi0Address)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupSpiSync) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  sysconPeripheral.enablePeripheralClocks(libMcuLL::sw::syscon::peripheralClocks::SPI0 |
                                          libMcuLL::sw::syscon::peripheralClocks::SWM |
                                          libMcuLL::sw::syscon::peripheralClocks::IOCON);
}

MINUNIT_ADD(LPC812M101CppSpiSyncInits, LPC812M101CppSetupSpiSync, LPC812M101Teardown) {
  uint32_t actualClock;
  actualClock = spiSyncPeripheral.initMaster(100000);
  minUnitCheck(actualClock == 100000);
  minUnitCheck((dutRegisters->CFG & CFG::RESERVED_MASK) == 0x00000005);
  minUnitCheck(dutRegisters->DIV == 299);
  dutRegisters->CFG = 0x00000000;
  actualClock = spiSyncPeripheral.initMaster(65399, waveforms::CPHA1_CPOL1_LSB, slavePolaritySelects::SPOL_HIGH);
  minUnitCheck(actualClock == 65502);
  minUnitCheck((dutRegisters->CFG & CFG::RESERVED_MASK) == 0x0000013D);
  minUnitCheck(dutRegisters->DIV == 457);
}

MINUNIT_ADD(LPC812M101CppSpiSyncRxTx, LPC812M101CppSetupSpiSync, LPC812M101Teardown) {
  swmPeriperhal.setup(test2Pin, spiMainSckFunction);
  swmPeriperhal.setup(test1Pin, spiMainMosiFunction);
  swmPeriperhal.setup(test0Pin, spiMainMisoFunction);
  spiSyncPeripheral.initMaster(1000000);
  minUnitCheck((dutRegisters->STAT & STAT::RESERVED_MASK) == 0x00000102);
  minUnitCheck(dutRegisters->DIV == 29);

  std::array<uint16_t, 5> testDataSend{0x1234, 0x4567, 0x89AB, 0xCDEF, 0x5A5A};
  std::array<uint16_t, 5> testDataReceive;
  testDataReceive.fill(0x0000u);
  // test "simple"(below 17 bits) transaction
  spiSyncPeripheral.readWrite(chipEnables::SSEL_NONE, testDataSend, testDataReceive, 8, true);
  minUnitCheck((testDataSend[0] & 0xFF) == testDataReceive[0]);
  // test "multi"(above 17 bits) transaction to test multi transfers
  testDataReceive.fill(0x0000u);
  spiSyncPeripheral.readWrite(chipEnables::SSEL_NONE, testDataSend, testDataReceive, 24, true);
  minUnitCheck(testDataSend[0] == testDataReceive[0]);
  minUnitCheck((testDataSend[1] & 0xFF) == testDataReceive[1]);
  // test receive
  // disable MOSI
  swmPeriperhal.clear(test1Pin, spiMainMosiFunction);
  gpioPeripheral.output(test1Pin);
  gpioPeripheral.high(test1Pin);
  // enable pullup, read all ones
  spiSyncPeripheral.read(chipEnables::SSEL_NONE, testDataReceive, 12, true);
  minUnitCheck(0xFFFu == testDataReceive[0]);
  // enable pulldown, read all zeroes
  gpioPeripheral.low(test1Pin);
  testDataReceive[0] = 0xFFFF;
  spiSyncPeripheral.read(chipEnables::SSEL_NONE, testDataReceive, 12, true);
  minUnitCheck(0x0u == testDataReceive[0]);
}