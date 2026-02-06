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
#include <minunit.h>
#include <LPC812M101_teardown.hpp>
#include <common.hpp>

using namespace libmcuhw::sct;
using namespace libmcull::sct;

// peripheral register sets
static constexpr libmcu::HwAddressType sctAddress = libmcuhw::SctAddress;
libmcuhw::sct::Sct *const dutRegisters{reinterpret_cast<libmcuhw::sct::Sct *>(sctAddress)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupSct) {
  MINUNIT_CHECK(Lpc812M101TeardownCorrect() == true);
  syscon_peripheral.EnablePeripheralClocks(
    libmcull::syscon::PeripheralClocks::ClockSct | libmcull::syscon::PeripheralClocks::ClockIocon |
    libmcull::syscon::PeripheralClocks::ClockGpio | libmcull::syscon::PeripheralClocks::ClockSwm);
  swm_peripheral.setup(pwm_out_pin, sct_output_0_function);
  swm_peripheral.setup(test_0_pin, sct_intput_0_function);
  iocon_peripheral.Setup(pwm_in_pin, libmcull::iocon::PullModes::INACTIVE);
  gpio_peripheral.SetLow(test_1_pin);
  gpio_peripheral.SetOutput(test_1_pin);
  // BaBi: When using Test0pin as output it does not work, no idea why!
}

MINUNIT_ADD(LPC812M101CppSctInit, LPC812M101CppSetupSct, LPC812M101Teardown) {
  sct_peripheral.Init(0x4u, CountingModes::Bidirectional);
  MINUNIT_CHECK(dutRegisters->CONFIG == 0x00020001u);
  MINUNIT_CHECK(dutRegisters->CTRL == 0x00040094u);
}

MINUNIT_ADD(LPC812M101CppSctRunning, LPC812M101CppSetupSct, LPC812M101Teardown) {
  sct_peripheral.Init(0x4u, CountingModes::Bidirectional);
  MINUNIT_CHECK(dutRegisters->CONFIG == 0x00020001u);
  MINUNIT_CHECK(dutRegisters->CTRL == 0x00040094u);
  sct_peripheral.SetMatch(Matches::Idx0, 30000u);
  MINUNIT_CHECK(dutRegisters->MATCH[0].U == 30000u);
  MINUNIT_CHECK(dutRegisters->MATCHREL[0].U == 30000u);
  sct_peripheral.Start();
  MINUNIT_CHECK((dutRegisters->CTRL & 0x4u) == 0u);
  std::uint32_t countFirst = sct_peripheral.GetCount();
  std::uint32_t countSecond = sct_peripheral.GetCount();
  MINUNIT_CHECK(countFirst != countSecond);
  sct_peripheral.Halt();
  MINUNIT_CHECK((dutRegisters->CTRL & 0x4u) == 0x4u);
}

MINUNIT_ADD(LPC812M101CppSctSetupPwm, LPC812M101CppSetupSct, LPC812M101Teardown) {
  sct_peripheral.Init(0x0, CountingModes::Bidirectional);
  sct_peripheral.SetMatch(Matches::Idx0, 1000u);
  sct_peripheral.SetupPwm(Matches::Idx1, 500, Events::Idx0, Outputs::Idx1, false);
  MINUNIT_CHECK(dutRegisters->MATCH[1].U == 500u);
  MINUNIT_CHECK(dutRegisters->MATCHREL[1].U == 500u);
  sct_peripheral.Start();
  // detect edges on SCT output1
  int timeout = 10000;
  int edgeCount = 0;
  bool oldOutput = sct_peripheral.GetOutputState(Outputs::Idx1);
  bool newOutput;
  while (timeout > 0 && edgeCount < 500) {
    newOutput = sct_peripheral.GetOutputState(Outputs::Idx1);
    if (oldOutput != newOutput) {
      edgeCount++;
      oldOutput = newOutput;
    }
    timeout--;
  }
  MINUNIT_CHECK(edgeCount == 500);
  MINUNIT_CHECK(timeout != 0);
  sct_peripheral.Halt();
}

MINUNIT_ADD(LPC812M101CppSctSetupCapture, LPC812M101CppSetupSct, LPC812M101Teardown) {
  sct_peripheral.Init(0x0, CountingModes::Bidirectional);
  dutRegisters->CONFIG = dutRegisters->CONFIG | CONFIG::INSYNC(0xF);
  sct_peripheral.SetMatch(Matches::Idx0, 3000000u);
  sct_peripheral.setupCapture(Captures::Idx1, Events::Idx0, Inputs::Idx0, CaptureConditions::Rising);
  sct_peripheral.Start();
  MINUNIT_CHECK(dutRegisters->CAP[1].U == 0u);
  gpio_peripheral.SetLow(test_1_pin);
  gpio_peripheral.SetHigh(test_1_pin);
  // check if we got event in SCT event flag register
  MINUNIT_CHECK(dutRegisters->EVFLAG == 0x01u);
  dutRegisters->EVFLAG = 0x01u;
  std::uint32_t firstCapture = sct_peripheral.GetCapture(Captures::Idx1);
  MINUNIT_CHECK(firstCapture != 0u);
  gpio_peripheral.SetLow(test_1_pin);
  gpio_peripheral.SetHigh(test_1_pin);
  std::uint32_t secondCapture = sct_peripheral.GetCapture(Captures::Idx1);
  MINUNIT_CHECK(firstCapture != secondCapture);
  sct_peripheral.Halt();
}