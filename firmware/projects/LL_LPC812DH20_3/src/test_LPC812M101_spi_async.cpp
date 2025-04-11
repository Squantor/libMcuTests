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

using namespace libmcull::sw::spi;
using namespace libmcuhw::spi;

static constexpr int maxIterations = 1000;
static constexpr libmcu::hwAddressType spi0Address = libmcuhw::spi0Address;
libmcuhw::spi::Spi *const dutRegisters{reinterpret_cast<libmcuhw::spi::Spi *>(spi0Address)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupSpiAsync) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  sysconPeripheral.enablePeripheralClocks(libmcull::sw::syscon::peripheralClocks::SPI0 |
                                          libmcull::sw::syscon::peripheralClocks::SWM |
                                          libmcull::sw::syscon::peripheralClocks::IOCON);
  swmPeriperhal.setup(test3Pin, spiMainSselFunction);
  swmPeriperhal.setup(test2Pin, spiMainSckFunction);
  swmPeriperhal.setup(test1Pin, spiMainMosiFunction);
  swmPeriperhal.setup(test0Pin, spiMainMisoFunction);
}

MINUNIT_ADD(LPC812M101CppSpiAsyncInits, LPC812M101CppSetupSpiAsync, LPC812M101Teardown) {
  uint32_t actualClock;
  actualClock = spiAsyncPeripheral16.initMaster(100000);
  minUnitCheck(actualClock == 100000);
  minUnitCheck((dutRegisters->CFG & CFG::kRESERVED_MASK) == 0x00000005);
  minUnitCheck(dutRegisters->DIV == 299);
  dutRegisters->CFG = 0x00000000;
  actualClock = spiAsyncPeripheral16.initMaster(65399, Waveforms::kCpha1Cpol1Lsb, SlavePolaritySelects::kHigh);
  minUnitCheck(actualClock == 65502);
  minUnitCheck((dutRegisters->CFG & CFG::kRESERVED_MASK) == 0x0000013D);
  minUnitCheck(dutRegisters->DIV == 457);
}

MINUNIT_ADD(LPC812M101CppSpiAsyncClaimUnclaim, LPC812M101CppSetupSpiAsync, LPC812M101Teardown) {
  spiAsyncPeripheral16.initMaster(100000);
  minUnitCheck(spiAsyncPeripheral16.claim() == libmcu::Results::CLAIMED);
  minUnitCheck(spiAsyncPeripheral16.claim() == libmcu::Results::IN_USE);
  minUnitCheck(spiAsyncPeripheral16.unclaim() == libmcu::Results::UNCLAIMED);
  minUnitCheck(spiAsyncPeripheral16.unclaim() == libmcu::Results::ERROR);
}

MINUNIT_ADD(LPC812M101CppSpiAsyncReadWrite16, LPC812M101CppSetupSpiAsync, LPC812M101Teardown) {
  int iterationCount = 0;
  libmcu::Results status;
  std::array<uint16_t, 5> testDataSend{0x1234, 0x4567, 0x89AB, 0xCDEF, 0x5A5A};
  std::array<uint16_t, 5> testDataReceive;
  testDataReceive.fill(0x0000u);
  spiAsyncPeripheral16.initMaster(1000000);
  minUnitCheck(spiAsyncPeripheral16.claim() == libmcu::Results::CLAIMED);
  minUnitCheck(spiAsyncPeripheral16.startReadWrite(ChipEnables::kSsel0, testDataSend, testDataReceive, 8, true) ==
               libmcu::Results::STARTED);
  minUnitCheck(spiAsyncPeripheral16.startReadWrite(ChipEnables::kSsel0, testDataSend, testDataReceive, 8, true) ==
               libmcu::Results::ERROR);
  minUnitCheck(spiAsyncPeripheral16.progress() == libmcu::Results::BUSY);

  iterationCount = 0;
  do {
    status = spiAsyncPeripheral16.progress();
    iterationCount++;
  } while (status == libmcu::Results::BUSY && iterationCount < maxIterations);

  minUnitCheck(status == libmcu::Results::DONE);
  minUnitCheck(iterationCount < maxIterations);
  minUnitCheck((testDataSend[0] & 0xFF) == testDataReceive[0]);
  // TODO check register statuses?
  testDataReceive.fill(0x0000u);
  minUnitCheck(spiAsyncPeripheral16.startReadWrite(ChipEnables::kNone, testDataSend, testDataReceive, 24, true) ==
               libmcu::Results::STARTED);
  iterationCount = 0;
  do {
    status = spiAsyncPeripheral16.progress();
    iterationCount++;
  } while (status == libmcu::Results::BUSY && iterationCount < maxIterations);

  minUnitCheck(status == libmcu::Results::DONE);
  minUnitCheck(iterationCount < maxIterations);
  minUnitCheck(testDataSend[0] == testDataReceive[0]);
  minUnitCheck((testDataSend[1] & 0xFF) == testDataReceive[1]);

  minUnitCheck(spiAsyncPeripheral16.unclaim() == libmcu::Results::UNCLAIMED);
}

MINUNIT_ADD(LPC812M101CppSpiAsyncReadWrite8, LPC812M101CppSetupSpiAsync, LPC812M101Teardown) {
  int iterationCount = 0;
  libmcu::Results status;
  std::array<uint8_t, 10> testDataSend{0x12, 0x34, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x5A, 0x5A};
  std::array<uint8_t, 10> testDataReceive;
  testDataReceive.fill(0x00);
  spiAsyncPeripheral8.initMaster(1000000);
  minUnitCheck(spiAsyncPeripheral8.claim() == libmcu::Results::CLAIMED);
  minUnitCheck(spiAsyncPeripheral8.startReadWrite(ChipEnables::kSsel0, testDataSend, testDataReceive, 8, true) ==
               libmcu::Results::STARTED);
  minUnitCheck(spiAsyncPeripheral8.startReadWrite(ChipEnables::kSsel0, testDataSend, testDataReceive, 8, true) ==
               libmcu::Results::ERROR);
  minUnitCheck(spiAsyncPeripheral8.progress() == libmcu::Results::BUSY);

  iterationCount = 0;
  do {
    status = spiAsyncPeripheral8.progress();
    iterationCount++;
  } while (status == libmcu::Results::BUSY && iterationCount < maxIterations);

  minUnitCheck(status == libmcu::Results::DONE);
  minUnitCheck(iterationCount < maxIterations);
  minUnitCheck((testDataSend[0]) == testDataReceive[0]);
  // TODO check register statuses?
  testDataReceive.fill(0x00);
  minUnitCheck(spiAsyncPeripheral8.startReadWrite(ChipEnables::kNone, testDataSend, testDataReceive, 28, true) ==
               libmcu::Results::STARTED);
  iterationCount = 0;
  do {
    status = spiAsyncPeripheral8.progress();
    iterationCount++;
  } while (status == libmcu::Results::BUSY && iterationCount < maxIterations);

  minUnitCheck(status == libmcu::Results::DONE);
  minUnitCheck(iterationCount < maxIterations);
  minUnitCheck(testDataReceive[0] == testDataSend[0]);
  minUnitCheck(testDataReceive[1] == testDataSend[1]);
  minUnitCheck(testDataReceive[2] == testDataSend[2]);
  minUnitCheck(testDataReceive[3] == (testDataSend[3] & 0x0F));
  minUnitCheck(testDataReceive[4] == 0x00);

  minUnitCheck(spiAsyncPeripheral8.unclaim() == libmcu::Results::UNCLAIMED);
}