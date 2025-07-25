/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file tests for the LPC812M101 SYSCON peripheral
 */
#include <nuclone_LPC812M101DH20_tests.hpp>
#include <MinUnit.h>
#include <LPC812M101_teardown.hpp>
#include <common.hpp>

using namespace libmcuhw::syscon;
using namespace libmcull::syscon;

// peripheral register sets
static constexpr libmcu::HwAddressType sysconAddress = libmcuhw::SysconAddress;
libmcuhw::syscon::Syscon *const SysconRegisters{reinterpret_cast<libmcuhw::syscon::Syscon *>(sysconAddress)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupSyscon) {
  minUnitCheck(Lpc812M101TeardownCorrect() == true);
}

MINUNIT_ADD(LPC812M101CppSysconChipID, LPC812M101CppSetupSyscon, LPC812M101Teardown) {
  minUnitCheck(syscon_peripheral.GetChipId() == 0x00008122);
}

MINUNIT_ADD(LPC812M101CppSysconResets, LPC812M101CppSetupSyscon, LPC812M101Teardown) {
  minUnitCheck(SysconRegisters->PRESETCTRL == 0x00001FFF);
  // preset the reset register to a value that will be reset by the test
  uint32_t resetRegister = SysconRegisters->PRESETCTRL & libmcuhw::syscon::PRESETCTRL::RESERVED_MASK;
  resetRegister = resetRegister & ~(PRESETCTRL::SPI0_RST_N | PRESETCTRL::I2C_RST_N | PRESETCTRL::ACMP_RST_N);
  minUnitCheck(resetRegister == 0x00000FBE);
  SysconRegisters->PRESETCTRL =
    (SysconRegisters->PRESETCTRL & ~PRESETCTRL::RESERVED_MASK) | (PRESETCTRL::RESERVED_MASK & resetRegister);
  syscon_peripheral.ResetPeripherals(PeripheralResets::ResetSpi0);
  // the reset function will restore all bits to operational
  minUnitCheck(SysconRegisters->PRESETCTRL == 0x00000FBF);
  syscon_peripheral.ResetPeripherals(PeripheralResets::ResetI2c | PeripheralResets::ResetAcmp);
  minUnitCheck(SysconRegisters->PRESETCTRL == 0x00001FFF);
}

// We only test analog comparator for now
MINUNIT_ADD(LPC812M101CppSysconPowering, LPC812M101CppSetupSyscon, LPC812M101Teardown) {
  minUnitCheck(SysconRegisters->PDRUNCFG == 0x0000ED50);
  syscon_peripheral.PowerPeripherals(PeripheralPowers::PowerAcmp);
  minUnitCheck(SysconRegisters->PDRUNCFG == 0x00006D50);
  syscon_peripheral.DepowerPeripherals(PeripheralPowers::PowerAcmp);
  minUnitCheck(SysconRegisters->PDRUNCFG == 0x0000ED50);
}

MINUNIT_ADD(LPC812M101CppSysconClocking, LPC812M101CppSetupSyscon, LPC812M101Teardown) {
  minUnitCheck(SysconRegisters->SYSAHBCLKCTRL == 0x000000DF);
  syscon_peripheral.EnablePeripheralClocks(PeripheralClocks::ClockAcmp);
  minUnitCheck(SysconRegisters->SYSAHBCLKCTRL == 0x000800DF);
  syscon_peripheral.EnablePeripheralClocks(PeripheralClocks::ClockIocon);
  minUnitCheck(SysconRegisters->SYSAHBCLKCTRL == 0x000C00DF);
  syscon_peripheral.DisablePeripheralClocks(PeripheralClocks::ClockAcmp);
  minUnitCheck(SysconRegisters->SYSAHBCLKCTRL == 0x000400DF);
  syscon_peripheral.DisablePeripheralClocks(PeripheralClocks::ClockIocon);
  minUnitCheck(SysconRegisters->SYSAHBCLKCTRL == 0x000000DF);
}