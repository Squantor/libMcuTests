/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file test_LPC845M301_dma.cpp
 * @brief tests for the LPC845M301 DMA peripheral
 */
#include <nuclone_LPC845BD48_small_LL.hpp>
#include <MinUnit.h>
#include <LPC845M301_teardown.hpp>
#include <common.hpp>

using namespace libMcuHw::dma;
using namespace libMcuLL::dma;

// peripheral register sets
static constexpr libMcu::hwAddressType dmaAddress = libMcuHw::dmaAddress; /**< peripheral address */
libMcuHw::dma::dma *const dutRegisters{reinterpret_cast<libMcuHw::dma::dma *>(dmaAddress)};

/**
 * @brief DMA setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupDma) {
  minUnitCheck(LPC845M301TeardownCorrect() == true);
  sysconPeripheral.enablePeripheralClocks(libMcuLL::syscon::peripheralClocks0::DMA, 0);
}

/**
 * @brief Tests iocon pullup/down functions
 */
MINUNIT_ADD(LPC845M301DH20DmaInit, LPC845M301SetupDma, LPC845M301Teardown) {
  dmaPeripheral.init();
  uint32_t PeripheralAddress = dutRegisters->SRAMBASE;
  uint32_t classAddress = reinterpret_cast<uint32_t>(dmaPeripheral.getDescriptorTable().data());
  minUnitCheck((classAddress & 0x1FF) == 0);  // 512 byte aligned check
  minUnitCheck(PeripheralAddress == classAddress);
}