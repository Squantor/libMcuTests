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

using namespace libmcuhw::sct;
using namespace libmcull::sct;

// peripheral register sets
static constexpr libmcu::HwAddressType sctAddress = libmcuhw::kSctAddress;
libmcuhw::sct::Sct *const dutRegisters{reinterpret_cast<libmcuhw::sct::Sct *>(sctAddress)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupSct) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  sysconPeripheral.EnablePeripheralClocks(
    libmcull::syscon::PeripheralClocks::kClockSct | libmcull::syscon::PeripheralClocks::kClockIocon |
    libmcull::syscon::PeripheralClocks::kClockGpio | libmcull::syscon::PeripheralClocks::kClockSwm);
  swmPeriperhal.setup(pwmOutPin, sctOutput0Function);
  swmPeriperhal.setup(test0Pin, sctInput0Function);
  ioconPeripheral.Setup(pwmInPin, libmcull::iocon::PullModes::INACTIVE);
  gpioPeripheral.SetLow(test1Pin);
  gpioPeripheral.SetOutput(test1Pin);
  // BaBi: When using Test0pin as output it does not work, no idea why!
}

MINUNIT_ADD(LPC812M101CppSctInit, LPC812M101CppSetupSct, LPC812M101Teardown) {
  sctPeripheral.Init(0x4u, CountingModes::kBidirectional);
  minUnitCheck(dutRegisters->CONFIG == 0x00020001u);
  minUnitCheck(dutRegisters->CTRL == 0x00040094u);
}

MINUNIT_ADD(LPC812M101CppSctRunning, LPC812M101CppSetupSct, LPC812M101Teardown) {
  sctPeripheral.Init(0x4u, CountingModes::kBidirectional);
  minUnitCheck(dutRegisters->CONFIG == 0x00020001u);
  minUnitCheck(dutRegisters->CTRL == 0x00040094u);
  sctPeripheral.SetMatch(Matches::k0, 30000u);
  minUnitCheck(dutRegisters->MATCH[0].U == 30000u);
  minUnitCheck(dutRegisters->MATCHREL[0].U == 30000u);
  sctPeripheral.Start();
  minUnitCheck((dutRegisters->CTRL & 0x4u) == 0u);
  std::uint32_t countFirst = sctPeripheral.GetCount();
  std::uint32_t countSecond = sctPeripheral.GetCount();
  minUnitCheck(countFirst != countSecond);
  sctPeripheral.Halt();
  minUnitCheck((dutRegisters->CTRL & 0x4u) == 0x4u);
}

MINUNIT_ADD(LPC812M101CppSctSetupPwm, LPC812M101CppSetupSct, LPC812M101Teardown) {
  sctPeripheral.Init(0x0, CountingModes::kBidirectional);
  sctPeripheral.SetMatch(Matches::k0, 1000u);
  sctPeripheral.SetupPwm(Matches::k1, 500, Events::k0, Outputs::k1, false);
  minUnitCheck(dutRegisters->MATCH[1].U == 500u);
  minUnitCheck(dutRegisters->MATCHREL[1].U == 500u);
  sctPeripheral.Start();
  // detect edges on SCT output1
  int timeout = 10000;
  int edgeCount = 0;
  bool oldOutput = sctPeripheral.GetOutputState(Outputs::k1);
  bool newOutput;
  while (timeout > 0 && edgeCount < 500) {
    newOutput = sctPeripheral.GetOutputState(Outputs::k1);
    if (oldOutput != newOutput) {
      edgeCount++;
      oldOutput = newOutput;
    }
    timeout--;
  }
  minUnitCheck(edgeCount == 500);
  minUnitCheck(timeout != 0);
  sctPeripheral.Halt();
}

MINUNIT_ADD(LPC812M101CppSctSetupCapture, LPC812M101CppSetupSct, LPC812M101Teardown) {
  sctPeripheral.Init(0x0, CountingModes::kBidirectional);
  dutRegisters->CONFIG = dutRegisters->CONFIG | CONFIG::INSYNC(0xF);
  sctPeripheral.SetMatch(Matches::k0, 3000000u);
  sctPeripheral.setupCapture(Captures::k1, Events::k0, Inputs::k0, CaptureConditions::kRising);
  sctPeripheral.Start();
  minUnitCheck(dutRegisters->CAP[1].U == 0u);
  gpioPeripheral.SetLow(test1Pin);
  gpioPeripheral.SetHigh(test1Pin);
  // check if we got event in SCT event flag register
  minUnitCheck(dutRegisters->EVFLAG == 0x01u);
  dutRegisters->EVFLAG = 0x01u;
  std::uint32_t firstCapture = sctPeripheral.GetCapture(Captures::k1);
  minUnitCheck(firstCapture != 0u);
  gpioPeripheral.SetLow(test1Pin);
  gpioPeripheral.SetHigh(test1Pin);
  std::uint32_t secondCapture = sctPeripheral.GetCapture(Captures::k1);
  minUnitCheck(firstCapture != secondCapture);
  sctPeripheral.Halt();
}