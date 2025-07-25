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
static constexpr libmcu::HwAddressType acmpAddress = libmcuhw::AcmpAddress;
libmcuhw::acmp::Acmp *const dutRegisters{reinterpret_cast<libmcuhw::acmp::Acmp *>(acmpAddress)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupacmp) {
  minUnitCheck(Lpc812M101TeardownCorrect() == true);
  syscon_peripheral.PowerPeripherals(libmcull::syscon::PeripheralPowers::PowerAcmp);
  syscon_peripheral.EnablePeripheralClocks(
    libmcull::syscon::PeripheralClocks::ClockSct | libmcull::syscon::PeripheralClocks::ClockIocon |
    libmcull::syscon::PeripheralClocks::ClockGpio | libmcull::syscon::PeripheralClocks::ClockSwm |
    libmcull::syscon::PeripheralClocks::ClockAcmp);
  // switch matrix
  swm_peripheral.setup(pwm_out_pin, sct_output_0_function);
  swm_peripheral.enableFixedPins(libmcuhw::swm::AcmpIn2);  // enable fixed function on PIO0_1
  iocon_peripheral.Setup(pwm_in_pin, libmcull::iocon::PullModes::INACTIVE);
  gpio_peripheral.SetLow(test_1_pin);
  gpio_peripheral.SetOutput(test_1_pin);
  // sct configuration
  sct_peripheral.Init(0x0, libmcull::sct::CountingModes::Bidirectional);
  sct_peripheral.SetMatch(libmcull::sct::Matches::Idx0, maxPwm);
  sct_peripheral.SetupPwm(libmcull::sct::Matches::Idx1, 1, libmcull::sct::Events::Idx0, libmcull::sct::Outputs::Idx0, true);
  sct_peripheral.Start();
}

MINUNIT_ADD(LPC812M101CppAcmpInit, LPC812M101CppSetupacmp, LPC812M101Teardown) {
  acmp_peripheral.init(PositiveInputs::IN2, NegativeInputs::REF, EdgeDetections::BOTH, OutputOptions::SYNCED,
                       HysteresisOptions::HYS_20MV);
  libmcu::Delay(settlingDelay);
  // we do not check the comparator mask due to some spurious activations of the edge detector
  std::uint32_t value = dutRegisters->CTRL & (CTRL::RESERVED_MASK & ~CTRL::COMPEDGE_MASK);
  minUnitCheck(value == 0x06003250);
  minUnitCheck((dutRegisters->LAD & LAD::RESERVED_MASK) == 0x00000000);
  acmp_peripheral.init(PositiveInputs::REF, NegativeInputs::IN2, EdgeDetections::FALLING, OutputOptions::DIRECT,
                       HysteresisOptions::HYS_10MV, LadderReferences::VDD);
  minUnitCheck((dutRegisters->CTRL & (CTRL::RESERVED_MASK & ~CTRL::COMPEDGE_MASK)) == 0x04201600);
  minUnitCheck((dutRegisters->LAD & LAD::RESERVED_MASK) == 0x00000001);
}

// test comparator functionality with internal reference only
MINUNIT_ADD(LPC812M101CppAcmpRef, LPC812M101CppSetupacmp, LPC812M101Teardown) {
  acmp_peripheral.init(PositiveInputs::REF, NegativeInputs::IN2, EdgeDetections::FALLING, OutputOptions::SYNCED,
                       HysteresisOptions::HYS_20MV);
  libmcu::Delay(settlingDelay);
  minUnitCheck(acmp_peripheral.comparatorOutput() != 0);
  minUnitCheck(acmp_peripheral.edgeOutput() == 0);
  // set pwm higher then internal reference voltage (903mv at 25C)
  sct_peripheral.SetReload(libmcull::sct::Matches::Idx1, maxPwm / 2);
  libmcu::Delay(settlingDelay);
  minUnitCheck(acmp_peripheral.comparatorOutput() == 0);
  minUnitCheck(acmp_peripheral.edgeOutput() != 0);
  // use the PWM to do a succesive approximation of the reference voltage
  std::uint32_t currentPwm = maxPwm - 1;
  std::uint32_t currentHalfPwm = maxPwm / 2;
  sct_peripheral.SetReload(libmcull::sct::Matches::Idx1, currentPwm);
  while (currentHalfPwm != 0) {
    libmcu::Delay(settlingDelay);
    if (acmp_peripheral.comparatorOutput() == 0) {
      currentPwm = currentPwm - currentHalfPwm;
    } else {
      currentPwm = currentPwm + currentHalfPwm;
    }
    sct_peripheral.SetReload(libmcull::sct::Matches::Idx1, currentPwm);
    currentHalfPwm = currentHalfPwm / 2;
  }
  // reference voltage should be in the range of (855mV and 945mV) according to datasheet with margins
  minUnitCheck(currentPwm > 240);
  minUnitCheck(currentPwm < 301);
}

// test ladder functionality
MINUNIT_ADD(LPC812M101CppAcmpLadder, LPC812M101CppSetupacmp, LPC812M101Teardown) {
  acmp_peripheral.init(PositiveInputs::REF, NegativeInputs::LADDER, EdgeDetections::FALLING, OutputOptions::SYNCED,
                       HysteresisOptions::HYS_20MV, LadderReferences::VDD);
  acmp_peripheral.setLadder(0);
  libmcu::Delay(settlingDelay);
  minUnitCheck(acmp_peripheral.comparatorOutput() != 0);
  // use the PWM to do a succesive approximation of the reference voltage
  std::uint32_t currentLadder = 31;
  std::uint32_t currentHalfLadder = currentLadder / 2;
  acmp_peripheral.setLadder(currentLadder);
  while (currentHalfLadder != 0) {
    libmcu::Delay(settlingDelay);
    if (acmp_peripheral.comparatorOutput() == 0) {
      currentLadder = currentLadder - currentHalfLadder;
    } else {
      currentLadder = currentLadder + currentHalfLadder;
    }
    acmp_peripheral.setLadder(currentLadder);
    currentHalfLadder = currentHalfLadder / 2;
  }
  // reference voltage should be in the range of (855mV and 945mV) according to datasheet with margins
  minUnitCheck(currentLadder > 6);
  minUnitCheck(currentLadder < 9);
}
