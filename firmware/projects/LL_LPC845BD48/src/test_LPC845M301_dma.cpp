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
static constexpr libmcu::HwAddressType dma_address = libmcuhw::kDmaAddress; /**< peripheral address */
libmcuhw::dma::Dma *const dma_registers{reinterpret_cast<libmcuhw::dma::Dma *>(dma_address)};

// Buffers
std::array<std::uint8_t, 128> inputBuffer;
std::array<std::uint8_t, 128> outputBuffer;

/**
 * @brief DMA setup and initialisation
 */
MINUNIT_SETUP(LPC845M301SetupDma) {
  minUnitCheck(Lpc845M301TeardownCorrect() == true);
  syscon_peripheral.ResetPeripherals(libmcull::syscon::peripheral_resets_0::kDma | libmcull::syscon::peripheral_resets_0::kUart0,
                                     0);
  syscon_peripheral.EnablePeripheralClocks(
    libmcull::syscon::peripheral_clocks_0::kDma | libmcull::syscon::peripheral_clocks_0::kUart0 |
      libmcull::syscon::peripheral_clocks_0::kSwm | libmcull::syscon::peripheral_clocks_0::kIocon,
    0);
}

/**
 * @brief Tests DMA init function functions
 */
MINUNIT_ADD(LPC845M301DH20DmaInit, LPC845M301SetupDma, LPC845M301Teardown) {
  dma_peripheral.Init();
  uint32_t PeripheralAddress = dma_registers->SRAMBASE;
  uint32_t classAddress = reinterpret_cast<uint32_t>(dma_peripheral.GetDescriptorTable().data());
  minUnitCheck((classAddress & 0x1FF) == 0);  // 512 byte aligned check
  minUnitCheck(PeripheralAddress == classAddress);
  for (auto &element : dma_peripheral.GetDescriptorTable()) {
    minUnitCheck(element.reserved == 0);
    minUnitCheck(element.source_end_address == 0);
    minUnitCheck(element.destination_end_address == 0);
    minUnitCheck(element.next_descriptor == nullptr);
  }
}

MINUNIT_ADD(LPC845M301DH20DmaMemToMemTransfer, LPC845M301SetupDma, LPC845M301Teardown) {
  dma_peripheral.Init();
  DescriptorTable &table = dma_peripheral.GetDescriptorTable();
  std::uint8_t counter = 37;
  // Fill input buffer with interesting pattern
  for (auto &element : inputBuffer) {
    element = counter;
    counter++;
  }
  outputBuffer.fill(0);
  // setup single memory to memory transfer descriptor
  dma_peripheral.ConfigureChanDescr(SoftwareDescriptors::kChannel12, inputBuffer.data() + inputBuffer.size() - 1,
                                    outputBuffer.data() + inputBuffer.size() - 1, nullptr);
  // check descriptor entry
  minUnitCheck(table[12].source_end_address == reinterpret_cast<std::uint32_t>(inputBuffer.data() + inputBuffer.size() - 1));
  minUnitCheck(table[12].destination_end_address == reinterpret_cast<std::uint32_t>(outputBuffer.data() + inputBuffer.size() - 1));
  minUnitCheck(table[12].next_descriptor == nullptr);
  // setup channel hardware
  dma_peripheral.ConfigureChannel(SoftwareDescriptors::kChannel12, TriggerConfigs::kNone, BurstSizes::k1, false, false,
                                  ChannelPrios::kLowest);
  dma_peripheral.ConfigureTransfer(SoftwareDescriptors::kChannel12, false, true, InterruptFlags::kNone, TransferSizes::k8Bit,
                                   SrcIncrements::k1, DstIncrements::k1, inputBuffer.size());
  dma_peripheral.ValidateChannel(SoftwareDescriptors::kChannel12);
  dma_peripheral.EnableChannel(SoftwareDescriptors::kChannel12);
  dma_peripheral.SetChannelTrigger(SoftwareDescriptors::kChannel12);
  // wait until transfer completes
  std::uint32_t wait_count = 0;
  while (dma_peripheral.IsChannelActive(SoftwareDescriptors::kChannel12) && wait_count < 1000) {
    wait_count += 1;
  }
  // check transfer results
  minUnitCheck(wait_count < 1000);
  minUnitCheck(dma_peripheral.IsChannelActive(SoftwareDescriptors::kChannel12) == false);
  minUnitCheck(memcmp(inputBuffer.data(), outputBuffer.data(), inputBuffer.size()) == 0);
}

MINUNIT_ADD(LPC845M301DH20DmaMemToPeriTransfer, LPC845M301SetupDma, LPC845M301Teardown) {
  dma_peripheral.Init();
  std::uint8_t counter = 83;
  // Setup buffers
  for (auto &element : inputBuffer) {
    element = counter;
    counter++;
  }
  outputBuffer.fill(0);
  // setup uart but only TX
  swm_peripheral.Setup(test_pin_2, uart_main_tx_function);
  usart_polled_peripheral.Init<uart_0_clock_config>(115200);
  syscon_peripheral.PeripheralClockSource(libmcull::syscon::ClockSourceSelects::kUart0, libmcull::syscon::ClockSources::kMain);
  // check if UART is ready
  std::uint32_t status = usart_polled_peripheral.Status();
  minUnitCheck((status & UartStatuses::kTxIdle) != 0);
  minUnitCheck((status & UartStatuses::kRxIdle) != 0);
  // setup DMA descriptors
  // setup single memory to peripheral transfer
  dma_peripheral.ConfigureChanDescr(HardwareDescriptors::kUSART0_TX_DMA, inputBuffer.data() + inputBuffer.size() - 1,
                                    usart_polled_peripheral.GetTxDataAddress(), nullptr);
  dma_peripheral.ConfigureChannel(HardwareDescriptors::kUSART0_TX_DMA, TriggerConfigs::kRisingEdge, BurstSizes::k1, false, false,
                                  ChannelPrios::kLowest);
  dma_peripheral.ConfigureTransfer(HardwareDescriptors::kUSART0_TX_DMA, false, true, InterruptFlags::kNone, TransferSizes::k8Bit,
                                   SrcIncrements::k1, DstIncrements::k0, inputBuffer.size());
  dma_peripheral.ValidateChannel(HardwareDescriptors::kUSART0_TX_DMA);
  dma_peripheral.EnableChannel(HardwareDescriptors::kUSART0_TX_DMA);
  dma_peripheral.SetChannelTrigger(HardwareDescriptors::kUSART0_TX_DMA);
  // starts automatically as uart is ready to transmit
  // wait until transfer completes
  std::uint32_t wait_count = 0;
  while (dma_peripheral.IsChannelActive(HardwareDescriptors::kUSART0_TX_DMA) && wait_count < 2000) {
    wait_count += 1;
  }
  // check transfer results
  minUnitCheck(wait_count < 2000);
  minUnitCheck(dma_peripheral.IsChannelActive(HardwareDescriptors::kUSART0_TX_DMA) == false);
  // DMA seems to wrap around to 1024 when completing its transfer
  minUnitCheck(XFERCFG::GetXFERCOUNT(dma_registers->CHANNEL[1].XFERCFG) == 1024);
}

MINUNIT_ADD(LPC845M301DH20DmaMemToPeriAndPeriToMemTransfer, LPC845M301SetupDma, LPC845M301Teardown) {
  dma_peripheral.Init();
  std::uint8_t counter = 83;
  // Setup buffers
  for (auto &element : inputBuffer) {
    element = counter;
    counter++;
  }
  outputBuffer.fill(0);
  // setup uart pins and connect RX to TX
  swm_peripheral.Setup(test_pin_1, uart_main_rx_function);
  swm_peripheral.Setup(test_pin_2, uart_main_tx_function);
  usart_polled_peripheral.Init<uart_0_clock_config>(115200);
  syscon_peripheral.PeripheralClockSource(libmcull::syscon::ClockSourceSelects::kUart0, libmcull::syscon::ClockSources::kMain);
  // check if UART is ready
  std::uint32_t status = usart_polled_peripheral.Status();
  minUnitCheck((status & UartStatuses::kTxIdle) != 0);
  minUnitCheck((status & UartStatuses::kRxIdle) != 0);
  // setup DMA for single peripheral to memory transfer for uart RX
  dma_peripheral.ConfigureChanDescr(HardwareDescriptors::kUSART0_RX_DMA, usart_polled_peripheral.GetRxDataAddress(),
                                    outputBuffer.data() + outputBuffer.size() - 1, nullptr);
  dma_peripheral.ConfigureChannel(HardwareDescriptors::kUSART0_RX_DMA, TriggerConfigs::kRisingEdge, BurstSizes::k1, false, false,
                                  ChannelPrios::kLowest);
  dma_peripheral.ConfigureTransfer(HardwareDescriptors::kUSART0_RX_DMA, false, true, InterruptFlags::kNone, TransferSizes::k8Bit,
                                   SrcIncrements::k0, DstIncrements::k1, outputBuffer.size());
  dma_peripheral.ValidateChannel(HardwareDescriptors::kUSART0_RX_DMA);
  dma_peripheral.EnableChannel(HardwareDescriptors::kUSART0_RX_DMA);
  dma_peripheral.SetChannelTrigger(HardwareDescriptors::kUSART0_RX_DMA);
  // setup DMA for single memory to peripheral transfer for uart TX
  dma_peripheral.ConfigureChanDescr(HardwareDescriptors::kUSART0_TX_DMA, inputBuffer.data() + inputBuffer.size() - 1,
                                    usart_polled_peripheral.GetTxDataAddress(), nullptr);
  dma_peripheral.ConfigureChannel(HardwareDescriptors::kUSART0_TX_DMA, TriggerConfigs::kRisingEdge, BurstSizes::k1, false, false,
                                  ChannelPrios::kLowest);
  dma_peripheral.ConfigureTransfer(HardwareDescriptors::kUSART0_TX_DMA, false, true, InterruptFlags::kNone, TransferSizes::k8Bit,
                                   SrcIncrements::k1, DstIncrements::k0, inputBuffer.size());
  dma_peripheral.ValidateChannel(HardwareDescriptors::kUSART0_TX_DMA);
  dma_peripheral.EnableChannel(HardwareDescriptors::kUSART0_TX_DMA);
  dma_peripheral.SetChannelTrigger(HardwareDescriptors::kUSART0_TX_DMA);
  // starts automatically as uart is ready to transmit
  // wait until transfer completes
  std::uint32_t wait_count = 0;
  while ((dma_peripheral.IsChannelActive(HardwareDescriptors::kUSART0_TX_DMA) ||
          dma_peripheral.IsChannelActive(HardwareDescriptors::kUSART0_RX_DMA)) &&
         wait_count < 2000) {
    wait_count += 1;
  }
  // check transfer results
  minUnitCheck(wait_count < 2000);
  minUnitCheck(dma_peripheral.IsChannelActive(HardwareDescriptors::kUSART0_TX_DMA) == false);
  minUnitCheck(dma_peripheral.IsChannelActive(HardwareDescriptors::kUSART0_RX_DMA) == false);
  // DMA seems to wrap around to 1024 when completing its transfer
  minUnitCheck(XFERCFG::GetXFERCOUNT(dma_registers->CHANNEL[0].XFERCFG) == 1024);
  minUnitCheck(XFERCFG::GetXFERCOUNT(dma_registers->CHANNEL[1].XFERCFG) == 1024);
  minUnitCheck(std::equal(inputBuffer.begin(), inputBuffer.end(), outputBuffer.begin()) == true);
}
// todo test pingpong mode with interrupts