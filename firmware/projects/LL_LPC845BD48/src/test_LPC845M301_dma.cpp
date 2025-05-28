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

using namespace libmcuhw::dma;
using namespace libmcull::dma;
using namespace libmcull::usart;

// peripheral register sets
static constexpr libmcu::HwAddressType dma_address = libmcuhw::dmaAddress; /**< peripheral address */
libmcuhw::dma::Dma *const dma_registers{reinterpret_cast<libmcuhw::dma::Dma *>(dma_address)};

// Buffers
std::array<std::uint8_t, 128> inputBuffer;
std::array<std::uint8_t, 128> outputBuffer;

/**
 * @brief DMA setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupDma) {
  minUnitCheck(LPC845M301TeardownCorrect() == true);
  sysconPeripheral.resetPeripherals(libmcull::syscon::peripheralResets0::DMA | libmcull::syscon::peripheralResets0::UART0, 0);
  sysconPeripheral.enablePeripheralClocks(libmcull::syscon::peripheralClocks0::DMA | libmcull::syscon::peripheralClocks0::UART0 |
                                            libmcull::syscon::peripheralClocks0::SWM | libmcull::syscon::peripheralClocks0::IOCON,
                                          0);
}

/**
 * @brief Tests DMA init function functions
 */
MINUNIT_ADD(LPC845M301DH20DmaInit, LPC845M301SetupDma, LPC845M301Teardown) {
  dmaPeripheral.Init();
  uint32_t PeripheralAddress = dma_registers->SRAMBASE;
  uint32_t classAddress = reinterpret_cast<uint32_t>(dmaPeripheral.GetDescriptorTable().data());
  minUnitCheck((classAddress & 0x1FF) == 0);  // 512 byte aligned check
  minUnitCheck(PeripheralAddress == classAddress);
  for (auto &element : dmaPeripheral.GetDescriptorTable()) {
    minUnitCheck(element.reserved == 0);
    minUnitCheck(element.source_end_address == 0);
    minUnitCheck(element.destination_end_address == 0);
    minUnitCheck(element.next_descriptor == nullptr);
  }
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
  dmaPeripheral.ConfigureChanDescr(SoftwareDescriptors::kChannel12, inputBuffer.data() + inputBuffer.size() - 1,
                                   outputBuffer.data() + inputBuffer.size() - 1, nullptr);
  // check descriptor entry
  minUnitCheck(table[12].source_end_address == reinterpret_cast<std::uint32_t>(inputBuffer.data() + inputBuffer.size() - 1));
  minUnitCheck(table[12].destination_end_address == reinterpret_cast<std::uint32_t>(outputBuffer.data() + inputBuffer.size() - 1));
  minUnitCheck(table[12].next_descriptor == nullptr);
  // setup channel hardware
  dmaPeripheral.ConfigureChannel(SoftwareDescriptors::kChannel12, TriggerConfigs::kNone, BurstSizes::k1, false, false,
                                 ChannelPrios::kLowest);
  dmaPeripheral.ConfigureTransfer(SoftwareDescriptors::kChannel12, false, true, InterruptFlags::kNone, TransferSizes::k8Bit,
                                  SrcIncrements::k1, DstIncrements::k1, inputBuffer.size());
  dmaPeripheral.ValidateChannel(SoftwareDescriptors::kChannel12);
  dmaPeripheral.EnableChannel(SoftwareDescriptors::kChannel12);
  dmaPeripheral.SetChannelTrigger(SoftwareDescriptors::kChannel12);
  // wait until transfer completes
  std::uint32_t wait_count = 0;
  while (dmaPeripheral.IsChannelActive(SoftwareDescriptors::kChannel12) && wait_count < 1000) {
    wait_count += 1;
  }
  // check transfer results
  minUnitCheck(wait_count < 1000);
  minUnitCheck(dmaPeripheral.IsChannelActive(SoftwareDescriptors::kChannel12) == false);
  minUnitCheck(memcmp(inputBuffer.data(), outputBuffer.data(), inputBuffer.size()) == 0);
}

MINUNIT_ADD(LPC845M301DH20DmaMemToPeriTransfer, LPC845M301SetupDma, LPC845M301Teardown) {
  dmaPeripheral.Init();
  std::uint8_t counter = 83;
  // Setup buffers
  for (auto &element : inputBuffer) {
    element = counter;
    counter++;
  }
  outputBuffer.fill(0);
  // setup uart but only TX
  swmPeriperhal.setup(testPin2, uartMainTxFunction);
  usartPeripheral.init<uart0ClockConfig>(115200);
  sysconPeripheral.peripheralClockSource(libmcull::syscon::ClockSourceSelects::UART0, libmcull::syscon::clockSources::MAIN);
  // check if UART is ready
  std::uint32_t status = usartPeripheral.status();
  minUnitCheck((status & uartStatus::TXIDLE) != 0);
  minUnitCheck((status & uartStatus::RXIDLE) != 0);
  // setup DMA descriptors
  // setup single memory to peripheral transfer
  dmaPeripheral.ConfigureChanDescr(HardwareDescriptors::kUSART0_TX_DMA, inputBuffer.data() + inputBuffer.size() - 1,
                                   usartPeripheral.getTxDatAddress(), nullptr);
  dmaPeripheral.ConfigureChannel(HardwareDescriptors::kUSART0_TX_DMA, TriggerConfigs::kRisingEdge, BurstSizes::k1, false, false,
                                 ChannelPrios::kLowest);
  dmaPeripheral.ConfigureTransfer(HardwareDescriptors::kUSART0_TX_DMA, false, true, InterruptFlags::kNone, TransferSizes::k8Bit,
                                  SrcIncrements::k1, DstIncrements::k0, inputBuffer.size());
  dmaPeripheral.ValidateChannel(HardwareDescriptors::kUSART0_TX_DMA);
  dmaPeripheral.EnableChannel(HardwareDescriptors::kUSART0_TX_DMA);
  dmaPeripheral.SetChannelTrigger(HardwareDescriptors::kUSART0_TX_DMA);
  // starts automatically as uart is ready to transmit
  // wait until transfer completes
  std::uint32_t wait_count = 0;
  while (dmaPeripheral.IsChannelActive(HardwareDescriptors::kUSART0_TX_DMA) && wait_count < 2000) {
    wait_count += 1;
  }
  // check transfer results
  minUnitCheck(wait_count < 2000);
  minUnitCheck(dmaPeripheral.IsChannelActive(HardwareDescriptors::kUSART0_TX_DMA) == false);
  // DMA seems to wrap around to 1024 when completing its transfer
  minUnitCheck(XFERCFG::GetXFERCOUNT(dma_registers->CHANNEL[1].XFERCFG) == 1024);
}

MINUNIT_ADD(LPC845M301DH20DmaMemToPeriAndPeriToMemTransfer, LPC845M301SetupDma, LPC845M301Teardown) {
  dmaPeripheral.Init();
  std::uint8_t counter = 83;
  // Setup buffers
  for (auto &element : inputBuffer) {
    element = counter;
    counter++;
  }
  outputBuffer.fill(0);
  // setup uart pins and connect RX to TX
  swmPeriperhal.setup(testPin1, uartMainRxFunction);
  swmPeriperhal.setup(testPin2, uartMainTxFunction);
  usartPeripheral.init<uart0ClockConfig>(115200);
  sysconPeripheral.peripheralClockSource(libmcull::syscon::ClockSourceSelects::UART0, libmcull::syscon::clockSources::MAIN);
  // check if UART is ready
  std::uint32_t status = usartPeripheral.status();
  minUnitCheck((status & uartStatus::TXIDLE) != 0);
  minUnitCheck((status & uartStatus::RXIDLE) != 0);
  // setup DMA for single peripheral to memory transfer for uart RX
  dmaPeripheral.ConfigureChanDescr(HardwareDescriptors::kUSART0_RX_DMA, usartPeripheral.getRxDatAddress(),
                                   outputBuffer.data() + outputBuffer.size() - 1, nullptr);
  dmaPeripheral.ConfigureChannel(HardwareDescriptors::kUSART0_RX_DMA, TriggerConfigs::kRisingEdge, BurstSizes::k1, false, false,
                                 ChannelPrios::kLowest);
  dmaPeripheral.ConfigureTransfer(HardwareDescriptors::kUSART0_RX_DMA, false, true, InterruptFlags::kNone, TransferSizes::k8Bit,
                                  SrcIncrements::k0, DstIncrements::k1, outputBuffer.size());
  dmaPeripheral.ValidateChannel(HardwareDescriptors::kUSART0_RX_DMA);
  dmaPeripheral.EnableChannel(HardwareDescriptors::kUSART0_RX_DMA);
  dmaPeripheral.SetChannelTrigger(HardwareDescriptors::kUSART0_RX_DMA);
  // setup DMA for single memory to peripheral transfer for uart TX
  dmaPeripheral.ConfigureChanDescr(HardwareDescriptors::kUSART0_TX_DMA, inputBuffer.data() + inputBuffer.size() - 1,
                                   usartPeripheral.getTxDatAddress(), nullptr);
  dmaPeripheral.ConfigureChannel(HardwareDescriptors::kUSART0_TX_DMA, TriggerConfigs::kRisingEdge, BurstSizes::k1, false, false,
                                 ChannelPrios::kLowest);
  dmaPeripheral.ConfigureTransfer(HardwareDescriptors::kUSART0_TX_DMA, false, true, InterruptFlags::kNone, TransferSizes::k8Bit,
                                  SrcIncrements::k1, DstIncrements::k0, inputBuffer.size());
  dmaPeripheral.ValidateChannel(HardwareDescriptors::kUSART0_TX_DMA);
  dmaPeripheral.EnableChannel(HardwareDescriptors::kUSART0_TX_DMA);
  dmaPeripheral.SetChannelTrigger(HardwareDescriptors::kUSART0_TX_DMA);
  // starts automatically as uart is ready to transmit
  // wait until transfer completes
  std::uint32_t wait_count = 0;
  while ((dmaPeripheral.IsChannelActive(HardwareDescriptors::kUSART0_TX_DMA) ||
          dmaPeripheral.IsChannelActive(HardwareDescriptors::kUSART0_RX_DMA)) &&
         wait_count < 2000) {
    wait_count += 1;
  }
  // check transfer results
  minUnitCheck(wait_count < 2000);
  minUnitCheck(dmaPeripheral.IsChannelActive(HardwareDescriptors::kUSART0_TX_DMA) == false);
  minUnitCheck(dmaPeripheral.IsChannelActive(HardwareDescriptors::kUSART0_RX_DMA) == false);
  // DMA seems to wrap around to 1024 when completing its transfer
  minUnitCheck(XFERCFG::GetXFERCOUNT(dma_registers->CHANNEL[0].XFERCFG) == 1024);
  minUnitCheck(XFERCFG::GetXFERCOUNT(dma_registers->CHANNEL[1].XFERCFG) == 1024);
  minUnitCheck(std::equal(inputBuffer.begin(), inputBuffer.end(), outputBuffer.begin()) == true);
}
// todo test pingpong mode with interrupts