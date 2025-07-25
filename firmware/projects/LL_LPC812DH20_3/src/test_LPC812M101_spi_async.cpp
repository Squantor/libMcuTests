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

static constexpr int maxIterations = 1000;
static constexpr libmcu::HwAddressType spi0Address = libmcuhw::Spi0Address;
libmcuhw::spi::Spi *const dutRegisters{reinterpret_cast<libmcuhw::spi::Spi *>(spi0Address)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupSpiAsync) {
  minUnitCheck(Lpc812M101TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(libmcull::syscon::PeripheralClocks::ClockSpi0 |
                                           libmcull::syscon::PeripheralClocks::ClockSwm |
                                           libmcull::syscon::PeripheralClocks::ClockIocon);
  swm_peripheral.setup(test_3_pin, spi_main_ssel_function);
  swm_peripheral.setup(test_2_pin, spi_main_sck_function);
  swm_peripheral.setup(test_1_pin, spi_main_mosi_function);
  swm_peripheral.setup(test_0_pin, spi_main_miso_function);
}

MINUNIT_ADD(LPC812M101CppSpiAsyncInits, LPC812M101CppSetupSpiAsync, LPC812M101Teardown) {
  uint32_t actualClock;
  actualClock = spiAsyncPeripheral16.InitMaster(100000);
  minUnitCheck(actualClock == 100000);
  minUnitCheck((dutRegisters->CFG & CFG::RESERVED_MASK) == 0x00000005);
  minUnitCheck(dutRegisters->DIV == 299);
  dutRegisters->CFG = 0x00000000;
  actualClock = spiAsyncPeripheral16.InitMaster(65399, Waveforms::Cpha1Cpol1Lsb, SlavePolaritySelects::High);
  minUnitCheck(actualClock == 65502);
  minUnitCheck((dutRegisters->CFG & CFG::RESERVED_MASK) == 0x0000013D);
  minUnitCheck(dutRegisters->DIV == 457);
}

MINUNIT_ADD(LPC812M101CppSpiAsyncClaimUnclaim, LPC812M101CppSetupSpiAsync, LPC812M101Teardown) {
  spiAsyncPeripheral16.InitMaster(100000);
  minUnitCheck(spiAsyncPeripheral16.Claim() == libmcu::Results::Claimed);
  minUnitCheck(spiAsyncPeripheral16.Claim() == libmcu::Results::InUse);
  minUnitCheck(spiAsyncPeripheral16.Unclaim() == libmcu::Results::Unclaimed);
  minUnitCheck(spiAsyncPeripheral16.Unclaim() == libmcu::Results::Error);
}

MINUNIT_ADD(LPC812M101CppSpiAsyncReadWrite16, LPC812M101CppSetupSpiAsync, LPC812M101Teardown) {
  int iterationCount = 0;
  libmcu::Results status;
  std::array<uint16_t, 5> testDataSend{0x1234, 0x4567, 0x89AB, 0xCDEF, 0x5A5A};
  std::array<uint16_t, 5> testDataReceive;
  testDataReceive.fill(0x0000u);
  spiAsyncPeripheral16.InitMaster(1000000);
  minUnitCheck(spiAsyncPeripheral16.Claim() == libmcu::Results::Claimed);
  minUnitCheck(spiAsyncPeripheral16.Transceive(ChipEnables::Ssel0, testDataSend, testDataReceive, 8, true) ==
               libmcu::Results::Started);
  minUnitCheck(spiAsyncPeripheral16.Transceive(ChipEnables::Ssel0, testDataSend, testDataReceive, 8, true) ==
               libmcu::Results::Error);
  minUnitCheck(spiAsyncPeripheral16.Progress() == libmcu::Results::Busy);

  iterationCount = 0;
  do {
    status = spiAsyncPeripheral16.Progress();
    iterationCount++;
  } while (status == libmcu::Results::Busy && iterationCount < maxIterations);

  minUnitCheck(status == libmcu::Results::Done);
  minUnitCheck(iterationCount < maxIterations);
  minUnitCheck((testDataSend[0] & 0xFF) == testDataReceive[0]);
  // TODO check register statuses?
  testDataReceive.fill(0x0000u);
  minUnitCheck(spiAsyncPeripheral16.Transceive(ChipEnables::None, testDataSend, testDataReceive, 24, true) ==
               libmcu::Results::Started);
  iterationCount = 0;
  do {
    status = spiAsyncPeripheral16.Progress();
    iterationCount++;
  } while (status == libmcu::Results::Busy && iterationCount < maxIterations);

  minUnitCheck(status == libmcu::Results::Done);
  minUnitCheck(iterationCount < maxIterations);
  minUnitCheck(testDataSend[0] == testDataReceive[0]);
  minUnitCheck((testDataSend[1] & 0xFF) == testDataReceive[1]);

  minUnitCheck(spiAsyncPeripheral16.Unclaim() == libmcu::Results::Unclaimed);
}

MINUNIT_ADD(LPC812M101CppSpiAsyncReadWrite8, LPC812M101CppSetupSpiAsync, LPC812M101Teardown) {
  int iterationCount = 0;
  libmcu::Results status;
  std::array<uint8_t, 10> testDataSend{0x12, 0x34, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x5A, 0x5A};
  std::array<uint8_t, 10> testDataReceive;
  testDataReceive.fill(0x00);
  spiAsyncPeripheral8.InitMaster(1000000);
  minUnitCheck(spiAsyncPeripheral8.Claim() == libmcu::Results::Claimed);
  minUnitCheck(spiAsyncPeripheral8.Transceive(ChipEnables::Ssel0, testDataSend, testDataReceive, 8, true) ==
               libmcu::Results::Started);
  minUnitCheck(spiAsyncPeripheral8.Transceive(ChipEnables::Ssel0, testDataSend, testDataReceive, 8, true) ==
               libmcu::Results::Error);
  minUnitCheck(spiAsyncPeripheral8.Progress() == libmcu::Results::Busy);

  iterationCount = 0;
  do {
    status = spiAsyncPeripheral8.Progress();
    iterationCount++;
  } while (status == libmcu::Results::Busy && iterationCount < maxIterations);

  minUnitCheck(status == libmcu::Results::Done);
  minUnitCheck(iterationCount < maxIterations);
  minUnitCheck((testDataSend[0]) == testDataReceive[0]);
  // TODO check register statuses?
  testDataReceive.fill(0x00);
  minUnitCheck(spiAsyncPeripheral8.Transceive(ChipEnables::None, testDataSend, testDataReceive, 28, true) ==
               libmcu::Results::Started);
  iterationCount = 0;
  do {
    status = spiAsyncPeripheral8.Progress();
    iterationCount++;
  } while (status == libmcu::Results::Busy && iterationCount < maxIterations);

  minUnitCheck(status == libmcu::Results::Done);
  minUnitCheck(iterationCount < maxIterations);
  minUnitCheck(testDataReceive[0] == testDataSend[0]);
  minUnitCheck(testDataReceive[1] == testDataSend[1]);
  minUnitCheck(testDataReceive[2] == testDataSend[2]);
  minUnitCheck(testDataReceive[3] == (testDataSend[3] & 0x0F));
  minUnitCheck(testDataReceive[4] == 0x00);

  minUnitCheck(spiAsyncPeripheral8.Unclaim() == libmcu::Results::Unclaimed);
}