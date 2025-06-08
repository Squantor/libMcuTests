/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC812M101 SCT peripheral
 */
#include <nuclone_LPC812M101DH20_tests.hpp>
#include <MinUnit.h>
#include <LPC812M101_teardown.hpp>
#include <common.hpp>

using namespace libmcuhw::acmp;
using namespace libmcull::acmp;

static constexpr std::uint32_t maxPwm = 1000;
static constexpr std::uint32_t settlingDelay = 8000;  // determined empirically ((RC delay + comparator delay) * 2)

// peripheral register sets
static constexpr libmcu::HwAddressType acmpAddress = libmcuhw::kAcmpAddress;
libmcuhw::acmp::Acmp *const dutRegisters{reinterpret_cast<libmcuhw::acmp::Acmp *>(acmpAddress)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupacmp) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  sysconPeripheral.PowerPeripherals(libmcull::syscon::PeripheralPowers::kPowerAcmp);
  sysconPeripheral.EnablePeripheralClocks(
    libmcull::syscon::PeripheralClocks::kClockSct | libmcull::syscon::PeripheralClocks::kClockIocon |
    libmcull::syscon::PeripheralClocks::kClockGpio | libmcull::syscon::PeripheralClocks::kClockSwm |
    libmcull::syscon::PeripheralClocks::kClockAcmp);
  // switch matrix
  swmPeriperhal.setup(pwmOutPin, sctOutput0Function);
  swmPeriperhal.enableFixedPins(libmcuhw::swm::kAcmpIn2);  // enable fixed function on PIO0_1
  ioconPeripheral.Setup(pwmInPin, libmcull::iocon::PullModes::INACTIVE);
  gpioPeripheral.SetLow(test1Pin);
  gpioPeripheral.SetOutput(test1Pin);
  // sct configuration
  sctPeripheral.Init(0x0, libmcull::sct::CountingModes::kBidirectional);
  sctPeripheral.SetMatch(libmcull::sct::Matches::k0, maxPwm);
  sctPeripheral.SetupPwm(libmcull::sct::Matches::k1, 1, libmcull::sct::Events::k0, libmcull::sct::Outputs::k0, true);
  sctPeripheral.Start();
}

MINUNIT_ADD(LPC812M101CppAcmpInit, LPC812M101CppSetupacmp, LPC812M101Teardown) {
  acmpPeripheral.init(PositiveInputs::IN2, NegativeInputs::REF, EdgeDetections::BOTH, OutputOptions::SYNCED,
                      HysteresisOptions::HYS_20MV);
  libmcu::Delay(settlingDelay);
  // we do not check the comparator mask due to some spurious activations of the edge detector
  std::uint32_t value = dutRegisters->CTRL & (CTRL::kkRESERVED_MASK & ~CTRL::kCOMPEDGE_MASK);
  minUnitCheck(value == 0x06003250);
  minUnitCheck((dutRegisters->LAD & LAD::kkRESERVED_MASK) == 0x00000000);
  acmpPeripheral.init(PositiveInputs::REF, NegativeInputs::IN2, EdgeDetections::FALLING, OutputOptions::DIRECT,
                      HysteresisOptions::HYS_10MV, LadderReferences::VDD);
  minUnitCheck((dutRegisters->CTRL & (CTRL::kkRESERVED_MASK & ~CTRL::kCOMPEDGE_MASK)) == 0x04201600);
  minUnitCheck((dutRegisters->LAD & LAD::kkRESERVED_MASK) == 0x00000001);
}

// test comparator functionality with internal reference only
MINUNIT_ADD(LPC812M101CppAcmpRef, LPC812M101CppSetupacmp, LPC812M101Teardown) {
  acmpPeripheral.init(PositiveInputs::REF, NegativeInputs::IN2, EdgeDetections::FALLING, OutputOptions::SYNCED,
                      HysteresisOptions::HYS_20MV);
  libmcu::Delay(settlingDelay);
  minUnitCheck(acmpPeripheral.comparatorOutput() != 0);
  minUnitCheck(acmpPeripheral.edgeOutput() == 0);
  // set pwm higher then internal reference voltage (903mv at 25C)
  sctPeripheral.SetReload(libmcull::sct::Matches::k1, maxPwm / 2);
  libmcu::Delay(settlingDelay);
  minUnitCheck(acmpPeripheral.comparatorOutput() == 0);
  minUnitCheck(acmpPeripheral.edgeOutput() != 0);
  // use the PWM to do a succesive approximation of the reference voltage
  std::uint32_t currentPwm = maxPwm - 1;
  std::uint32_t currentHalfPwm = maxPwm / 2;
  sctPeripheral.SetReload(libmcull::sct::Matches::k1, currentPwm);
  while (currentHalfPwm != 0) {
    libmcu::Delay(settlingDelay);
    if (acmpPeripheral.comparatorOutput() == 0) {
      currentPwm = currentPwm - currentHalfPwm;
    } else {
      currentPwm = currentPwm + currentHalfPwm;
    }
    sctPeripheral.SetReload(libmcull::sct::Matches::k1, currentPwm);
    currentHalfPwm = currentHalfPwm / 2;
  }
  // reference voltage should be in the range of (855mV and 945mV) according to datasheet with margins
  minUnitCheck(currentPwm > 240);
  minUnitCheck(currentPwm < 301);
}

// test ladder functionality
MINUNIT_ADD(LPC812M101CppAcmpLadder, LPC812M101CppSetupacmp, LPC812M101Teardown) {
  acmpPeripheral.init(PositiveInputs::REF, NegativeInputs::LADDER, EdgeDetections::FALLING, OutputOptions::SYNCED,
                      HysteresisOptions::HYS_20MV, LadderReferences::VDD);
  acmpPeripheral.setLadder(0);
  libmcu::Delay(settlingDelay);
  minUnitCheck(acmpPeripheral.comparatorOutput() != 0);
  // use the PWM to do a succesive approximation of the reference voltage
  std::uint32_t currentLadder = 31;
  std::uint32_t currentHalfLadder = currentLadder / 2;
  acmpPeripheral.setLadder(currentLadder);
  while (currentHalfLadder != 0) {
    libmcu::Delay(settlingDelay);
    if (acmpPeripheral.comparatorOutput() == 0) {
      currentLadder = currentLadder - currentHalfLadder;
    } else {
      currentLadder = currentLadder + currentHalfLadder;
    }
    acmpPeripheral.setLadder(currentLadder);
    currentHalfLadder = currentHalfLadder / 2;
  }
  // reference voltage should be in the range of (855mV and 945mV) according to datasheet with margins
  minUnitCheck(currentLadder > 6);
  minUnitCheck(currentLadder < 9);
}
