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
#include <string.h>

using namespace libMcuHw::dma;
using namespace libMcuLL::dma;

// peripheral register sets
static constexpr libMcu::hwAddressType dmaAddress = libMcuHw::dmaAddress; /**< peripheral address */
libMcuHw::dma::dma *const dutRegisters{reinterpret_cast<libMcuHw::dma::dma *>(dmaAddress)};

// Buffers
std::array<std::uint8_t, 128> inputBuffer;
std::array<std::uint8_t, 128> outputBuffer;

/**
 * @brief DMA setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupDma) {
  minUnitCheck(LPC845M301TeardownCorrect() == true);
  sysconPeripheral.enablePeripheralClocks(libMcuLL::syscon::peripheralClocks0::DMA, 0);
}

/**
 * @brief Tests DMA init function functions
 */
MINUNIT_ADD(LPC845M301DH20DmaInit, LPC845M301SetupDma, LPC845M301Teardown) {
  dmaPeripheral.Init();
  uint32_t PeripheralAddress = dutRegisters->SRAMBASE;
  uint32_t classAddress = reinterpret_cast<uint32_t>(dmaPeripheral.GetDescriptorTable().data());
  minUnitCheck((classAddress & 0x1FF) == 0);  // 512 byte aligned check
  minUnitCheck(PeripheralAddress == classAddress);
}

MINUNIT_ADD(LPC845M301DH20DmaMemToMemTransfer, LPC845M301SetupDma, LPC845M301Teardown) {
  dmaPeripheral.Init();
  DescriptorTable &table = dmaPeripheral.GetDescriptorTable();
  std::uint8_t counter = 37;
  // Fill input buffer with interesting pattern
  for (auto &element : inputBuffer) {
    element = counter;
    counter++;
  }
  outputBuffer.fill(0);
  // setup single memory to memory transfer descriptor
  dmaPeripheral.ConfigureChanDescr(Descriptors::kChannel12, inputBuffer.data() + inputBuffer.size() - 1,
                                   outputBuffer.data() + inputBuffer.size() - 1, nullptr);
  // check descriptor entry
  minUnitCheck(table[12].source_end_address == reinterpret_cast<std::uint32_t>(inputBuffer.data() + inputBuffer.size() - 1));
  minUnitCheck(table[12].destination_end_address == reinterpret_cast<std::uint32_t>(outputBuffer.data() + inputBuffer.size() - 1));
  minUnitCheck(table[12].next_descriptor == nullptr);
  // setup channel hardware
  dmaPeripheral.ConfigureChannel(Descriptors::kChannel12, TriggerConfigs::kNone, BurstSizes::k1, false, false,
                                 ChannelPrios::kLowest);
  dmaPeripheral.ConfigureTransfer(Descriptors::kChannel12, false, false, true, InterruptFlags::kNone, TransferSizes::k8Bit,
                                  SrcIncrements::k1, DstIncrements::k1, inputBuffer.size());
  dmaPeripheral.ValidateChannel(Descriptors::kChannel12);
  dmaPeripheral.EnableChannel(Descriptors::kChannel12);
  dmaPeripheral.SetChannelTrigger(Descriptors::kChannel12);
  // wait until transfer completes
  std::uint32_t wait_count = 0;
  while (dmaPeripheral.IsChannelActive(Descriptors::kChannel12) && wait_count < 1000) {
    wait_count += 1;
  }
  // check transfer results
  minUnitCheck(wait_count < 1000);
  minUnitCheck(dmaPeripheral.IsChannelActive(Descriptors::kChannel12) == false);
  minUnitCheck(memcmp(inputBuffer.data(), outputBuffer.data(), inputBuffer.size()) == 0);
}