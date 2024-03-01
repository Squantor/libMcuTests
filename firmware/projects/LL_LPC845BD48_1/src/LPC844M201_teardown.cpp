/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief Teardown function that resets the LPC845 to a state
 * that more or less matches reset state
 */
#include <nuclone_LPC844M201BD64_tests.hpp>
#include <LPC844M201_teardown.hpp>
#include <test_conditions.hpp>
#include <MinUnit.h>

#define IOCON_NORMAL_DEFAULT 0x000000B0 /**< Default value for most pins */
#define IOCON_I2CPIN_MASK (~0x00000400) /**< some I2C pins have no default value, so mask them */
#define IOCON_I2CPIN_DEFAULT 0x000000B0 /**< I2C pins have some bits extra and lose some bits */
#define IOCON_XTAL_DEFAULT 0x00000080   /**< Crystal oscillator deviate from reset values */

/** @brief resets all the registers to their default states
 */
MINUNIT_TEARDOWN(LPC844M201Teardown) {
  sysconEnableClocks(SYSCON, CLKCTRL0_I2C0 | CLKCTRL0_I2C1, CLKCTRL1_NONE);
  sysconDisableResets(SYSCON, RESETCTRL0_I2C0 | RESETCTRL0_I2C1, RESETCTRL1_NONE);
  sysconEnableResets(SYSCON, RESETCTRL0_I2C0 | RESETCTRL0_I2C1, RESETCTRL1_NONE);
  sysconDisableClocks(SYSCON, CLKCTRL0_I2C0 | CLKCTRL0_I2C1, CLKCTRL1_NONE);

  sysconEnableClocks(SYSCON, CLKCTRL0_SPI0, CLKCTRL1_NONE);
  SPI0->CFG = 0x00000000;
  SPI0->STAT = SPI_STAT_CLEAR;
  sysconDisableClocks(SYSCON, CLKCTRL0_SPI0, CLKCTRL1_NONE);

  sysconEnableClocks(SYSCON, CLKCTRL0_IOCON, CLKCTRL1_NONE);
  IOCON->PIO[IOCON_PIO0_17] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_13] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_12] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_5] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_4] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_3] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_2] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_11] = IOCON_I2CPIN_DEFAULT;
  IOCON->PIO[IOCON_PIO0_10] = IOCON_I2CPIN_DEFAULT;
  IOCON->PIO[IOCON_PIO0_16] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_15] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_1] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_9] = IOCON_XTAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_8] = IOCON_XTAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_7] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_6] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_0] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_14] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_28] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_27] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_26] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_25] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_24] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_23] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_22] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_21] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_20] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_19] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_18] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_8] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_9] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_12] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_13] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_31] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_0] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_1] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_2] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_14] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_15] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_3] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_4] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_5] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_16] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_17] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_6] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_18] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_19] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_7] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_29] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO0_30] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_20] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_21] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_11] = IOCON_NORMAL_DEFAULT;
  IOCON->PIO[IOCON_PIO1_10] = IOCON_NORMAL_DEFAULT;
  sysconDisableClocks(SYSCON, CLKCTRL0_IOCON, CLKCTRL1_NONE);

  sysconEnableClocks(SYSCON, CLKCTRL0_GPIO0 | CLKCTRL0_GPIO1, CLKCTRL1_NONE);
  GPIO->DIR[0] = 0x00000000;
  GPIO->DIR[1] = 0x00000000;
  GPIO->MASK[0] = 0x00000000;
  GPIO->MASK[1] = 0x00000000;
  GPIO->PIN[0] = 0x00000000;
  GPIO->PIN[1] = 0x00000000;
  GPIO->SET[0] = 0x00000000;
  GPIO->SET[1] = 0x00000000;
  GPIO->DIRSET[0] = 0x00000000;
  GPIO->DIRSET[1] = 0x00000000;
  sysconDisableClocks(SYSCON, CLKCTRL0_GPIO0 | CLKCTRL0_GPIO1, CLKCTRL1_NONE);

  sysconEnableClocks(SYSCON, CLKCTRL0_SWM, CLKCTRL1_NONE);
  SWM->PINASSIGN_DATA[0] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[1] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[2] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[3] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[4] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[5] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[6] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[7] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[8] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[9] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[10] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[11] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[12] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[13] = 0xFFFFFFFF;
  SWM->PINASSIGN_DATA[14] = 0xFFFFFFFF;
  SWM->PINENABLE0 = 0xFFFFFC1F;
  SWM->PINENABLE1 = 0x0000007F;
  sysconDisableClocks(SYSCON, CLKCTRL0_SWM, CLKCTRL1_NONE);

  // keep syscon as last
  SYSCON->SYSAHBCLKCTRL0 = 0x17;
  SYSCON->SYSAHBCLKCTRL1 = 0x0;
  SYSCON->PRESETCTRL0 = 0xFFFFFFFF;
  SYSCON->PRESETCTRL1 = 0x1F;
  minUnitCheck(LPC844M201TeardownCorrect() == true);
}

/** @brief checks if all the registers to their default states
 *  @return if all registers are correctly torndown
 */
bool LPC844M201TeardownCorrect(void) {
  // check i2c reset states
  sysconEnableClocks(SYSCON, CLKCTRL0_I2C0 | CLKCTRL0_I2C1, CLKCTRL1_NONE);
  TESTANDRETURN(I2C0->CFG == 0x0);
  TESTANDRETURN(I2C0->STAT == 0x801);
  TESTANDRETURN(I2C0->MSTTIME == 0x77);
  TESTANDRETURN(I2C1->CFG == 0x0);
  TESTANDRETURN(I2C1->STAT == 0x801);
  TESTANDRETURN(I2C1->MSTTIME == 0x77);
  sysconDisableClocks(SYSCON, CLKCTRL0_I2C0 | CLKCTRL0_I2C1, CLKCTRL1_NONE);
  // check IOCON reset state
  sysconEnableClocks(SYSCON, CLKCTRL0_IOCON, CLKCTRL1_NONE);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_17] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_13] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_12] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_5] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_4] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_3] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_2] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN((IOCON->PIO[IOCON_PIO0_11] & IOCON_I2CPIN_MASK) == IOCON_I2CPIN_DEFAULT);
  TESTANDRETURN((IOCON->PIO[IOCON_PIO0_10] & IOCON_I2CPIN_MASK) == IOCON_I2CPIN_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_16] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_15] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_1] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_9] == IOCON_XTAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_8] == IOCON_XTAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_7] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_6] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_0] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_14] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_28] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_27] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_26] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_25] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_24] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_23] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_22] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_21] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_20] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_19] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_18] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_8] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_9] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_12] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_13] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_31] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_0] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_1] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_2] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_14] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_15] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_3] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_4] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_5] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_16] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_17] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_6] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_18] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_19] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_7] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_29] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO0_30] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_20] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_21] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_11] == IOCON_NORMAL_DEFAULT);
  TESTANDRETURN(IOCON->PIO[IOCON_PIO1_10] == IOCON_NORMAL_DEFAULT);
  sysconDisableClocks(SYSCON, CLKCTRL0_IOCON, CLKCTRL1_NONE);
  // check SWM reset state
  sysconEnableClocks(SYSCON, CLKCTRL0_SWM, CLKCTRL1_NONE);
  TESTANDRETURN(SWM->PINASSIGN_DATA[0] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[1] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[2] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[3] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[4] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[5] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[6] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[7] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[8] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[9] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[10] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[11] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[12] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[13] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINASSIGN_DATA[14] == 0xFFFFFFFF);
  TESTANDRETURN(SWM->PINENABLE0 == 0xFFFFFC1F);
  TESTANDRETURN(SWM->PINENABLE1 == 0x0000007F);
  sysconDisableClocks(SYSCON, CLKCTRL0_SWM, CLKCTRL1_NONE);
  // verify GPIO reset state
  sysconEnableClocks(SYSCON, CLKCTRL0_GPIO0 | CLKCTRL0_GPIO1, CLKCTRL1_NONE);
  TESTANDRETURN(GPIO->DIR[0] == 0x00000000);
  TESTANDRETURN(GPIO->DIR[1] == 0x00000000);
  TESTANDRETURN(GPIO->MASK[0] == 0x00000000);
  TESTANDRETURN(GPIO->MASK[1] == 0x00000000);
  TESTANDRETURN(GPIO->SET[0] == 0x00000000);
  TESTANDRETURN(GPIO->SET[1] == 0x00000000);
  TESTANDRETURN(GPIO->DIRSET[0] == 0x00000000);
  TESTANDRETURN(GPIO->DIRSET[1] == 0x00000000);
  sysconDisableClocks(SYSCON, CLKCTRL0_GPIO0 | CLKCTRL0_GPIO1, CLKCTRL1_NONE);
  // verify UART0 reset state
  sysconEnableClocks(SYSCON, CLKCTRL0_UART0, CLKCTRL1_NONE);
  TESTANDRETURN(USART0->CFG == 0x00000000);
  TESTANDRETURN(USART0->CTL == 0x00000000);
  TESTANDRETURN((USART0->STAT & 0x0001FD6F) == 0x0000000E);
  TESTANDRETURN(USART0->INTENSET == 0x00000000);
  // this differs from the datasheet
  TESTANDRETURN((USART0->INTSTAT & 0x0001F96D) == 0x00000000);
  TESTANDRETURN(USART0->OSR == 0x0000000F);
  TESTANDRETURN(USART0->ADDR == 0x00000000);
  sysconDisableClocks(SYSCON, CLKCTRL0_UART0, CLKCTRL1_NONE);
  // verify SPI reset state
  sysconEnableClocks(SYSCON, CLKCTRL0_SPI0, CLKCTRL1_NONE);
  TESTANDRETURN(SPI0->CFG == 0x00000000);
  sysconDisableClocks(SYSCON, CLKCTRL0_SPI0, CLKCTRL1_NONE);
  // verify peripheral reset state (template)
  sysconEnableClocks(SYSCON, CLKCTRL0_NONE, CLKCTRL1_NONE);
  sysconDisableClocks(SYSCON, CLKCTRL0_NONE, CLKCTRL1_NONE);
  // verify SYSCON reset state, keep it for last
  TESTANDRETURN(SYSCON->SYSAHBCLKCTRL0 == 0x17);
  TESTANDRETURN(SYSCON->SYSAHBCLKCTRL1 == 0x0);
  TESTANDRETURN(SYSCON->PRESETCTRL0 == 0xFFFFFFFF);
  TESTANDRETURN(SYSCON->PRESETCTRL1 == 0x1F);
  return true;
}