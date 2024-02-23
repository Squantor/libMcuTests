/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief tests for the LPC844M201 SPI peripheral polling mode
 */
#include <nuclone_LPC844M201BD64_tests.hpp>
#include <MinUnit.h>
#include <LPC844M201_teardown.hpp>
#include <common.hpp>

/**
 * @brief Spi setup and initialisation
 */
MINUNIT_SETUP(LPC844M201SetupI2C) {
  minUnitCheck(LPC844M201TeardownCorrect() == true);
  sysconEnableClocks(SYSCON, CLKCTRL0_I2C0 | CLKCTRL0_I2C1 | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
  sysconEnableResets(SYSCON, RESETCTRL0_I2C0 | RESETCTRL0_I2C1 | RESETCTRL0_SWM | RESETCTRL0_IOCON, RESETCTRL1_NONE);
  ioconSetupPin(IOCON, IOCON_I2C_SCL_OUT, IOCON_I2CMODE(IOCON_I2CMODE_STD));
  ioconSetupPin(IOCON, IOCON_I2C_SDA_OUT, IOCON_I2CMODE(IOCON_I2CMODE_STD));
  ioconSetupPin(IOCON, IOCON_I2C_SCL_TEST, IOCON_MODE(IOCON_MODE_INACTIVE));
  ioconSetupPin(IOCON, IOCON_I2C_SDA_TEST, IOCON_MODE(IOCON_MODE_INACTIVE));
  swmEnableFixedPin(SWM, SWM_EN0_I2C0_SCL, SWM_EN1_NONE);
  swmEnableFixedPin(SWM, SWM_EN0_I2C0_SDA, SWM_EN1_NONE);
  SwmMovablePinAssign(SWM, SWM_I2C1_SCL, SWM_I2C_SCL_TEST);
  SwmMovablePinAssign(SWM, SWM_I2C1_SDA, SWM_I2C_SDA_TEST);
  sysconPeripheralClockSelect(SYSCON, I2C0CLKSEL, CLKSRC_MAIN);
  sysconPeripheralClockSelect(SYSCON, I2C1CLKSEL, CLKSRC_MAIN);
}

MINUNIT_ADD(LPC844M201I2CWriteNoAck, LPC844M201SetupI2C, LPC844M201Teardown) {
  i2cSetClockDivider(I2C0, 5);
  i2cSetConfiguration(I2C0, I2C_CFG_MSTEN);
  // write to address 0x7 with !W/R bit unset
  i2cSetMasterData(I2C0, 0x0E);
  i2cSetMasterControl(I2C0, I2C_MSCTL_MSTSTART);
  // waiting for status
  uint32_t i2cStatus;
  int i = 0;
  do {
    i2cStatus = i2cGetStatus(I2C0);
    i++;
  } while (I2C_STAT_MSTSTATE(i2cStatus) != I2C_STAT_MSSTATE_NACK_ADDRESS && i < 1000);
  // timed out? and in a not acknowledged state?
  minUnitCheck(i < 1000);
  minUnitCheck(I2C_STAT_MSTSTATE(i2cStatus) == I2C_STAT_MSSTATE_NACK_ADDRESS);
  swmDisableFixedPin(SWM, SWM_EN0_I2C0_SCL, SWM_EN1_NONE);
  swmDisableFixedPin(SWM, SWM_EN0_I2C0_SDA, SWM_EN1_NONE);
  sysconDisableClocks(SYSCON, CLKCTRL0_I2C0 | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
}

MINUNIT_ADD(LPC844M201I2CReadNoAck, LPC844M201SetupI2C, LPC844M201Teardown) {
  i2cSetClockDivider(I2C0, 5);
  i2cSetConfiguration(I2C0, I2C_CFG_MSTEN);
  // write to address 0x7 with !W/R bit set
  i2cSetMasterData(I2C0, 0x0F);
  i2cSetMasterControl(I2C0, I2C_MSCTL_MSTSTART);
  // waiting for status
  uint32_t i2cStatus;
  int i = 0;
  do {
    i2cStatus = i2cGetStatus(I2C0);
    i++;
  } while (I2C_STAT_MSTSTATE(i2cStatus) != I2C_STAT_MSSTATE_NACK_ADDRESS && i < 1000);
  // timed out? and in a not acknowledged state?
  minUnitCheck(i < 1000);
  minUnitCheck(I2C_STAT_MSTSTATE(i2cStatus) == I2C_STAT_MSSTATE_NACK_ADDRESS);
  swmDisableFixedPin(SWM, SWM_EN0_I2C0_SCL, SWM_EN1_NONE);
  swmDisableFixedPin(SWM, SWM_EN0_I2C0_SDA, SWM_EN1_NONE);
  sysconDisableClocks(SYSCON, CLKCTRL0_I2C0 | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
}

MINUNIT_ADD(LPC844M201I2CWriteAck, LPC844M201SetupI2C, LPC844M201Teardown) {
  i2cSetClockDivider(I2C0, 5);
  i2cSetClockDivider(I2C1, 5);
  i2cSetConfiguration(I2C0, I2C_CFG_MSTEN);
  i2cSetConfiguration(I2C1, I2C_CFG_SLVEN);
  i2cSetSlaveAddress(I2C1, I2C_SLVADR_1, 0x2E);
  // write to address 0x7 with !W/R bit unset
  i2cSetMasterData(I2C0, 0x2E);
  // actual transfer code
  uint32_t masterStatus, slaveStatus;
  int i = 0;
  // Address transfer phase
  i2cSetMasterControl(I2C0, I2C_MSCTL_MSTSTART);
  // check if slave i2c needs attention and is in the proper state
  do {
    slaveStatus = i2cGetStatus(I2C1);
    i++;
  } while (((slaveStatus & I2C_STAT_SLVPENDING) == 0) && i < 1000);
  minUnitCheck(I2C_STAT_SLVSTATE(slaveStatus) == I2C_STAT_SLVSTATE_ADDR);
  minUnitCheck(i < 1000);
  i = 0;
  // yes, acknowledge address
  i2cSetSlaveControl(I2C1, I2C_SLVCTL_SLVCONTINUE);
  // check if master i2c needs attention and is in the proper state
  do {
    masterStatus = i2cGetStatus(I2C0);
    i++;
  } while (((masterStatus & I2C_STAT_MSTPENDING) == 0) && i < 1000);
  minUnitCheck(I2C_STAT_MSTSTATE(masterStatus) == I2C_STAT_MSSTATE_TRANSMIT_READY);
  minUnitCheck(i < 1000);
  i = 0;
  // data transfer phase, setup master to transfer data
  i2cSetMasterData(I2C0, 0x5A);
  i2cSetMasterControl(I2C0, I2C_MSCTL_MSTCONTINUE);
  // check if slave i2c needs attention and is in the proper state
  do {
    slaveStatus = i2cGetStatus(I2C1);
    i++;
  } while (((slaveStatus & I2C_STAT_SLVPENDING) == 0) && i < 1000);
  minUnitCheck(I2C_STAT_SLVSTATE(slaveStatus) == I2C_STAT_SLVSTATE_RECEIVE);
  minUnitCheck(i2cGetSlaveData(I2C1) == 0x5A);
  minUnitCheck(i < 1000);
  i = 0;
  // yes, acknowledge master and continue
  i2cSetSlaveControl(I2C1, I2C_SLVCTL_SLVCONTINUE);
  // check if master has gotten acknowledge from slave
  do {
    masterStatus = i2cGetStatus(I2C0);
    i++;
  } while (((masterStatus & I2C_STAT_MSTPENDING) == 0) && i < 1000);
  minUnitCheck(I2C_STAT_MSTSTATE(masterStatus) == I2C_STAT_MSSTATE_TRANSMIT_READY);
  minUnitCheck(i < 1000);
  i = 0;
  // data transfer done, stop operation
  i2cSetMasterControl(I2C0, I2C_MSCTL_MSTSTOP);
  // check if slave i2c needs attention and is in the proper state
  do {
    slaveStatus = i2cGetStatus(I2C1);
    i++;
  } while (((slaveStatus & I2C_STAT_SLVPENDING) != 0) && i < 1000);
  minUnitCheck(i < 1000);
  i = 0;
  sysconDisableClocks(SYSCON, CLKCTRL0_I2C0 | CLKCTRL0_I2C1 | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
}

#if 0
// Somehow I cant get this to work. The waveform shows that the data transmit from slave to master starts
// and just stops all of a sudden.
MINUNIT_ADD(LPC844M201I2CReadAck, LPC844M201SetupI2C, LPC844M201Teardown) {
  i2cSetClockDivider(I2C0, 5);
  i2cSetClockDivider(I2C1, 5);
  i2cSetConfiguration(I2C0, I2C_CFG_MSTEN);
  i2cSetConfiguration(I2C1, I2C_CFG_SLVEN);
  i2cSetSlaveAddress(I2C1, I2C_SLVADR_1, 0x2E);
  // write to address 0x7 with !W/R bit set
  i2cSetMasterData(I2C0, 0x2F);
  // actual transfer code
  uint32_t masterStatus, slaveStatus;
  int i = 0;
  // Address transfer phase
  i2cSetMasterControl(I2C0, I2C_MSCTL_MSTSTART);
  // check if slave i2c needs attention and is in the proper state
  do {
    slaveStatus = i2cGetStatus(I2C1);
    i++;
  } while (((slaveStatus & I2C_STAT_SLVPENDING) == 0) && i < 1000);
  minUnitCheck(I2C_STAT_SLVSTATE(slaveStatus) == I2C_STAT_SLVSTATE_ADDR);
  minUnitCheck(I2C_STAT_SLVIDX(slaveStatus) == I2C_STAT_SLVIDX_ADDR1);
  minUnitCheck(i < 1000);
  i = 0;
  // yes, acknowledge address
  i2cSetSlaveControl(I2C1, I2C_SLVCTL_SLVCONTINUE);
  // loop slave until we can continue
  do {
    slaveStatus = i2cGetStatus(I2C1);
    i++;
  } while (((slaveStatus & I2C_STAT_SLVPENDING) == 0) && i < 1000);
  minUnitCheck(I2C_STAT_SLVSTATE(slaveStatus) == I2C_STAT_SLVSTATE_TRANSMIT);
  minUnitCheck(i < 1000);
  i = 0;
  // prepare data transfer
  i2cSetSlaveData(I2C1, 0x12);
  // prepare to transfer data
  i2cSetSlaveControl(I2C1, I2C_SLVCTL_SLVCONTINUE);
  // check if master i2c needs attention and is in the proper state
  do {
    masterStatus = i2cGetStatus(I2C0);
    i++;
  } while (((masterStatus & I2C_STAT_MSTPENDING) == 0) && i < 1000);
  minUnitCheck(I2C_STAT_MSTSTATE(masterStatus) == I2C_STAT_MSSTATE_RECEIVE_READY);
  minUnitCheck(i2cGetMasterData(I2C0) == 0xA1);
  minUnitCheck(i < 1000);
  i = 0;
  // done, stop transfer and check status
  i2cSetMasterControl(I2C0, I2C_MSCTL_MSTSTOP);
  // check if master i2c needs attention and is in the proper state
  do {
    masterStatus = i2cGetStatus(I2C0);
    i++;
  } while (((masterStatus & I2C_STAT_MSTPENDING) == 0) && i < 1000);
  minUnitCheck(I2C_STAT_MSTSTATE(masterStatus) == I2C_STAT_MSSTATE_IDLE);
  sysconDisableClocks(SYSCON, CLKCTRL0_I2C0 | CLKCTRL0_I2C1 | CLKCTRL0_SWM | CLKCTRL0_IOCON, CLKCTRL1_NONE);
}

#endif