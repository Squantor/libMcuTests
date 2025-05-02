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
using namespace libmcull::sw::syscon;

// peripheral register sets
static constexpr libmcu::hwAddressType sysconAddress = libmcuhw::sysconAddress;
libmcuhw::syscon::Syscon *const SysconRegisters{reinterpret_cast<libmcuhw::syscon::Syscon *>(sysconAddress)};

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC812M101CppSetupSyscon) {
  minUnitCheck(LPC812M101TeardownCorrect() == true);
}

MINUNIT_ADD(LPC812M101CppSysconChipID, LPC812M101CppSetupSyscon, LPC812M101Teardown) {
  minUnitCheck(sysconPeripheral.getChipId() == 0x00008122);
}

MINUNIT_ADD(LPC812M101CppSysconResets, LPC812M101CppSetupSyscon, LPC812M101Teardown) {
  minUnitCheck(SysconRegisters->PRESETCTRL == 0x00001FFF);
  // preset the reset register to a value that will be reset by the test
  uint32_t resetRegister = SysconRegisters->PRESETCTRL & libmcuhw::syscon::PRESETCTRL::kRESERVED_MASK;
  resetRegister = resetRegister & ~(PRESETCTRL::kSPI0_RST_N | PRESETCTRL::kI2C_RST_N | PRESETCTRL::kACMP_RST_N);
  minUnitCheck(resetRegister == 0x00000FBE);
  SysconRegisters->PRESETCTRL =
    (SysconRegisters->PRESETCTRL & ~PRESETCTRL::kRESERVED_MASK) | (PRESETCTRL::kRESERVED_MASK & resetRegister);
  sysconPeripheral.resetPeripherals(peripheralResets::SPI0);
  // the reset function will restore all bits to operational
  minUnitCheck(SysconRegisters->PRESETCTRL == 0x00000FBF);
  sysconPeripheral.resetPeripherals(peripheralResets::I2C | peripheralResets::ACMP);
  minUnitCheck(SysconRegisters->PRESETCTRL == 0x00001FFF);
}

// We only test analog comparator for now
MINUNIT_ADD(LPC812M101CppSysconPowering, LPC812M101CppSetupSyscon, LPC812M101Teardown) {
  minUnitCheck(SysconRegisters->PDRUNCFG == 0x0000ED50);
  sysconPeripheral.powerPeripherals(peripheralPowers::ACMP);
  minUnitCheck(SysconRegisters->PDRUNCFG == 0x00006D50);
  sysconPeripheral.depowerPeripherals(peripheralPowers::ACMP);
  minUnitCheck(SysconRegisters->PDRUNCFG == 0x0000ED50);
}

MINUNIT_ADD(LPC812M101CppSysconClocking, LPC812M101CppSetupSyscon, LPC812M101Teardown) {
  minUnitCheck(SysconRegisters->SYSAHBCLKCTRL == 0x000000DF);
  sysconPeripheral.enablePeripheralClocks(peripheralClocks::ACMP);
  minUnitCheck(SysconRegisters->SYSAHBCLKCTRL == 0x000800DF);
  sysconPeripheral.enablePeripheralClocks(peripheralClocks::IOCON);
  minUnitCheck(SysconRegisters->SYSAHBCLKCTRL == 0x000C00DF);
  sysconPeripheral.disablePeripheralClocks(peripheralClocks::ACMP);
  minUnitCheck(SysconRegisters->SYSAHBCLKCTRL == 0x000400DF);
  sysconPeripheral.disablePeripheralClocks(peripheralClocks::IOCON);
  minUnitCheck(SysconRegisters->SYSAHBCLKCTRL == 0x000000DF);
}