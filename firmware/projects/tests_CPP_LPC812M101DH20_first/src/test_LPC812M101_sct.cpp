/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC812M101 SCT peripheral
 */
#include <nuclone_LPC812M101DH20_tests.hpp>
#include <MinUnit.h>
#include <LPC812M101_teardown.hpp>
#include <common.hpp>

using namespace libMcuLL::hw::sct;
using namespace libMcuLL::sw::sct;

// peripheral register sets
static constexpr libMcuLL::hwAddressType sctAddress = libMcuLL::hw::sctAddress;
libMcuLL::hw::sct::peripheral *const dutRegisters{reinterpret_cast<libMcuLL::hw::sct::peripheral *>(sctAddress)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupSct) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  sysconPeripheral.enablePeripheralClocks(
    libMcuLL::sw::syscon::peripheralClocks::SCT | libMcuLL::sw::syscon::peripheralClocks::IOCON |
    libMcuLL::sw::syscon::peripheralClocks::GPIO | libMcuLL::sw::syscon::peripheralClocks::SWM);
  swmPeriperhal.setup(pwmOutPin, sctOutput0Function);
  swmPeriperhal.setup(test0Pin, sctInput0Function);
  ioconPeripheral.setup(pwmInPin, libMcuLL::sw::iocon::pullModes::INACTIVE);
  gpioPeripheral.low(test1Pin);
  gpioPeripheral.output(test1Pin);
  // BaBi: When using Test0pin as output it does not work, no idea why!
}

MINUNIT_ADD(LPC812M101CppSctInit, LPC812M101CppSetupSct, LPC812M101Teardown) {
  sctPeripheral.init(0x4u, countingMode::BIDIRECTIONAL);
  minUnitCheck(dutRegisters->CONFIG == 0x00020001u);
  minUnitCheck(dutRegisters->CTRL == 0x00040094u);
}

MINUNIT_ADD(LPC812M101CppSctRunning, LPC812M101CppSetupSct, LPC812M101Teardown) {
  sctPeripheral.init(0x4u, countingMode::BIDIRECTIONAL);
  minUnitCheck(dutRegisters->CONFIG == 0x00020001u);
  minUnitCheck(dutRegisters->CTRL == 0x00040094u);
  sctPeripheral.setMatch(matchNumber::MATCH_0, 30000u);
  minUnitCheck(dutRegisters->MATCH[0].U == 30000u);
  minUnitCheck(dutRegisters->MATCHREL[0].U == 30000u);
  sctPeripheral.start();
  minUnitCheck((dutRegisters->CTRL & 0x4u) == 0u);
  std::uint32_t countFirst = sctPeripheral.counter();
  std::uint32_t countSecond = sctPeripheral.counter();
  minUnitCheck(countFirst != countSecond);
  sctPeripheral.halt();
  minUnitCheck((dutRegisters->CTRL & 0x4u) == 0x4u);
}

MINUNIT_ADD(LPC812M101CppSctSetupPwm, LPC812M101CppSetupSct, LPC812M101Teardown) {
  sctPeripheral.init(0x0, countingMode::BIDIRECTIONAL);
  sctPeripheral.setMatch(matchNumber::MATCH_0, 1000u);
  sctPeripheral.setupPwm(matchNumber::MATCH_1, 500, eventNumber::EVENT_0, outputNumber::OUTPUT_1, false);
  minUnitCheck(dutRegisters->MATCH[1].U == 500u);
  minUnitCheck(dutRegisters->MATCHREL[1].U == 500u);
  sctPeripheral.start();
  // detect edges on SCT output1
  int timeout = 10000;
  int edgeCount = 0;
  bool oldOutput = sctPeripheral.output(outputNumber::OUTPUT_1);
  bool newOutput;
  while (timeout > 0 && edgeCount < 500) {
    newOutput = sctPeripheral.output(outputNumber::OUTPUT_1);
    if (oldOutput != newOutput) {
      edgeCount++;
      oldOutput = newOutput;
    }
    timeout--;
  }
  minUnitCheck(edgeCount == 500);
  minUnitCheck(timeout != 0);
  sctPeripheral.halt();
}

MINUNIT_ADD(LPC812M101CppSctSetupCapture, LPC812M101CppSetupSct, LPC812M101Teardown) {
  sctPeripheral.init(0x0, countingMode::BIDIRECTIONAL);
  dutRegisters->CONFIG = dutRegisters->CONFIG | CONFIG::INSYNC(0xF);
  sctPeripheral.setMatch(matchNumber::MATCH_0, 3000000u);
  sctPeripheral.setupCapture(captureNumber::CAPTURE_1, eventNumber::EVENT_0, inputNumber::INPUT_0, captureCondition::CAPTURE_RISE);
  sctPeripheral.start();
  minUnitCheck(dutRegisters->CAP[1].U == 0u);
  gpioPeripheral.low(test1Pin);
  gpioPeripheral.high(test1Pin);
  // check if we got event in SCT event flag register
  minUnitCheck(dutRegisters->EVFLAG == 0x01u);
  dutRegisters->EVFLAG = 0x01u;
  std::uint32_t firstCapture = sctPeripheral.getCapture(captureNumber::CAPTURE_1);
  minUnitCheck(firstCapture != 0u);
  gpioPeripheral.low(test1Pin);
  gpioPeripheral.high(test1Pin);
  std::uint32_t secondCapture = sctPeripheral.getCapture(captureNumber::CAPTURE_1);
  minUnitCheck(firstCapture != secondCapture);
  sctPeripheral.halt();
}