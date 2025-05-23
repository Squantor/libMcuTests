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
using namespace libmcull::sw::acmp;

static constexpr std::uint32_t maxPwm = 1000;
static constexpr std::uint32_t settlingDelay = 8000;  // determined empirically ((RC delay + comparator delay) * 2)

// peripheral register sets
static constexpr libmcu::hwAddressType acmpAddress = libmcuhw::acmpAddress;
libmcuhw::acmp::Acmp *const dutRegisters{reinterpret_cast<libmcuhw::acmp::Acmp *>(acmpAddress)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupacmp) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
  sysconPeripheral.powerPeripherals(libmcull::sw::syscon::peripheralPowers::ACMP);
  sysconPeripheral.enablePeripheralClocks(
    libmcull::sw::syscon::peripheralClocks::SCT | libmcull::sw::syscon::peripheralClocks::IOCON |
    libmcull::sw::syscon::peripheralClocks::GPIO | libmcull::sw::syscon::peripheralClocks::SWM |
    libmcull::sw::syscon::peripheralClocks::ACMP);
  // switch matrix
  swmPeriperhal.setup(pwmOutPin, sctOutput0Function);
  swmPeriperhal.enableFixedPins(libmcull::sw::swm::kAcmpIn2);  // enable fixed function on PIO0_1
  ioconPeripheral.setup(pwmInPin, libmcull::sw::iocon::pullModes::INACTIVE);
  gpioPeripheral.low(test1Pin);
  gpioPeripheral.output(test1Pin);
  // sct configuration
  sctPeripheral.init(0x0, libmcull::sw::sct::countingMode::BIDIRECTIONAL);
  sctPeripheral.setMatch(libmcull::sw::sct::matchNumber::MATCH_0, maxPwm);
  sctPeripheral.setupPwm(libmcull::sw::sct::matchNumber::MATCH_1, 1, libmcull::sw::sct::eventNumber::EVENT_0,
                         libmcull::sw::sct::outputNumber::OUTPUT_0, true);
  sctPeripheral.start();
}

MINUNIT_ADD(LPC812M101CppAcmpInit, LPC812M101CppSetupacmp, LPC812M101Teardown) {
  acmpPeripheral.init(inputPositiveSettings::IN2, inputNegativeSettings::REF, edgeDetectSettings::BOTH,
                      outputControlSettings::SYNCED, hysteresisSettings::HYS_20MV);
  libmcu::Delay(settlingDelay);
  // we do not check the comparator mask due to some spurious activations of the edge detector
  std::uint32_t value = dutRegisters->CTRL & (CTRL::kkRESERVED_MASK & ~CTRL::kCOMPEDGE_MASK);
  minUnitCheck(value == 0x06003250);
  minUnitCheck((dutRegisters->LAD & LAD::kkRESERVED_MASK) == 0x00000000);
  acmpPeripheral.init(inputPositiveSettings::REF, inputNegativeSettings::IN2, edgeDetectSettings::FALLING,
                      outputControlSettings::DIRECT, hysteresisSettings::HYS_10MV, ladderReferenceSetting::VDD);
  minUnitCheck((dutRegisters->CTRL & (CTRL::kkRESERVED_MASK & ~CTRL::kCOMPEDGE_MASK)) == 0x04201600);
  minUnitCheck((dutRegisters->LAD & LAD::kkRESERVED_MASK) == 0x00000001);
}

// test comparator functionality with internal reference only
MINUNIT_ADD(LPC812M101CppAcmpRef, LPC812M101CppSetupacmp, LPC812M101Teardown) {
  acmpPeripheral.init(inputPositiveSettings::REF, inputNegativeSettings::IN2, edgeDetectSettings::FALLING,
                      outputControlSettings::SYNCED, hysteresisSettings::HYS_20MV);
  libmcu::Delay(settlingDelay);
  minUnitCheck(acmpPeripheral.comparatorOutput() != 0);
  minUnitCheck(acmpPeripheral.edgeOutput() == 0);
  // set pwm higher then internal reference voltage (903mv at 25C)
  sctPeripheral.setReload(libmcull::sw::sct::matchNumber::MATCH_1, maxPwm / 2);
  libmcu::Delay(settlingDelay);
  minUnitCheck(acmpPeripheral.comparatorOutput() == 0);
  minUnitCheck(acmpPeripheral.edgeOutput() != 0);
  // use the PWM to do a succesive approximation of the reference voltage
  std::uint32_t currentPwm = maxPwm - 1;
  std::uint32_t currentHalfPwm = maxPwm / 2;
  sctPeripheral.setReload(libmcull::sw::sct::matchNumber::MATCH_1, currentPwm);
  while (currentHalfPwm != 0) {
    libmcu::Delay(settlingDelay);
    if (acmpPeripheral.comparatorOutput() == 0) {
      currentPwm = currentPwm - currentHalfPwm;
    } else {
      currentPwm = currentPwm + currentHalfPwm;
    }
    sctPeripheral.setReload(libmcull::sw::sct::matchNumber::MATCH_1, currentPwm);
    currentHalfPwm = currentHalfPwm / 2;
  }
  // reference voltage should be in the range of (855mV and 945mV) according to datasheet with margins
  minUnitCheck(currentPwm > 240);
  minUnitCheck(currentPwm < 301);
}

// test ladder functionality
MINUNIT_ADD(LPC812M101CppAcmpLadder, LPC812M101CppSetupacmp, LPC812M101Teardown) {
  acmpPeripheral.init(inputPositiveSettings::REF, inputNegativeSettings::LADDER, edgeDetectSettings::FALLING,
                      outputControlSettings::SYNCED, hysteresisSettings::HYS_20MV, ladderReferenceSetting::VDD);
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
