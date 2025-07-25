/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file nuclone_LPC845BD48_small_HAL.hpp
 * @brief definitions for LPC845 small nuclone HAL tester
 */
#ifndef NUCLONE_LPC845BD48_SMALL_HAL_HPP
#define NUCLONE_LPC845BD48_SMALL_HAL_HPP

#include <nxp/libmcu_LPC845M301BD48_hal.hpp>

#define TICKS_PER_S 10

// pin types
// Crystal osillator pins
using XtalInPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin08>;
using XtalOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin09>;
using TestPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port1, libmcuhw::IoPins::Pin05>;
// connected pin 1 and 2 via 3.3K resistors, links UART RX/TX, PIN1 on board
using Pin1Type = libmcuhw::Pin<libmcuhw::IoPorts::Port1, libmcuhw::IoPins::Pin08>;
using Pin1PortType = libmcuhw::Port<libmcuhw::IoPorts::Port1>;
using Pin2Type = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin17>;
using Pin2PortType = libmcuhw::Port<libmcuhw::IoPorts::Port0>;
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO, PIN3 on board
using Pin3Type = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin13>;
using Pin3PortType = libmcuhw::Port<libmcuhw::IoPorts::Port0>;
using Pin4Type = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin18>;
using Pin4PortType = libmcuhw::Port<libmcuhw::IoPorts::Port0>;
// connected pin 5 and 6 via 3.3K resistors, links SPI CS/SCK, PIN2 on board
using Pin5Type = libmcuhw::Pin<libmcuhw::IoPorts::Port1, libmcuhw::IoPins::Pin09>;
using Pin5PortType = libmcuhw::Port<libmcuhw::IoPorts::Port1>;
using Pin6Type = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin19>;
using Pin6PortType = libmcuhw::Port<libmcuhw::IoPorts::Port0>;
// connected pin 7 and 8 via 3.3K resistors
using Pin7Type = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin04>;
using Pin7PortType = libmcuhw::Port<libmcuhw::IoPorts::Port0>;
using Pin8Type = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin20>;
using Pin8PortType = libmcuhw::Port<libmcuhw::IoPorts::Port0>;
// SDA pin with 2.2K pull up resistor
using I2cSdaPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin11>;
// SCL pin with 2.2K pull up resistor
using I2cSclPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin10>;

// function types
using XtalInFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::XtalIn>;
using XtalOutFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::XtalOut>;
using ClockOutFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::ClkOut>;
using MainI2cSdaFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::I2c0SdaInOut>;
using MainI2cSclFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::I2c0SclInOut>;
using MainUartTxFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Uart0TxOut>;
using MainUartRxFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Uart0RxIn>;

// pin instances
constexpr XtalInPinType xtal_in_pin;
constexpr XtalOutPinType xtal_out_pin;
constexpr TestPinType test_pin;
constexpr I2cSclPinType i2c_scl_pin;
constexpr I2cSdaPinType i2c_sda_pin;
constexpr Pin1Type test_pin_1;
constexpr Pin2Type test_pin_2;
constexpr Pin3Type test_pin_3;
constexpr Pin4Type test_pin_4;
constexpr Pin5Type test_pin_5;
constexpr Pin6Type test_pin_6;
constexpr Pin7Type test_pin_7;
constexpr Pin8Type test_pin_8;
constexpr Pin1PortType test_pin_1_port;
constexpr Pin2PortType test_pin_2_port;
constexpr Pin3PortType test_pin_3_port;
constexpr Pin4PortType test_pin_4_port;
constexpr Pin5PortType test_pin_5_port;
constexpr Pin6PortType test_pin_6_port;
constexpr Pin7PortType test_pin_7_port;
constexpr Pin8PortType test_pin_8_port;

// function instances
constexpr XtalInFunctionType xtal_in_function;
constexpr XtalOutFunctionType xtal_out_function;
constexpr ClockOutFunctionType clock_out_function;
constexpr MainI2cSclFunctionType i2c_main_scl_function;
constexpr MainI2cSdaFunctionType i2c_main_sda_function;
constexpr MainUartTxFunctionType uart_main_tx_function;
constexpr MainUartRxFunctionType uart_main_rx_function;

using UartTransferType = std::uint8_t;

// peripheral externs
extern libmcull::systick::Systick<libmcuhw::SystickAddress> systick_peripheral;
extern libmcull::nvic::Nvic<libmcuhw::NvicAddress, libmcuhw::ScbAddress> nvic_peripheral;
extern libmcull::iocon::Iocon<libmcuhw::IoconAddress> iocon_peripheral;
extern libmcull::swm::Swm<libmcuhw::SwmAddress> swm_peripheral;
extern libmcull::gpio::Gpio<libmcuhw::GpioAddress> gpio_peripheral;
extern libmcull::syscon::Syscon<libmcuhw::SysconAddress> syscon_peripheral;
constexpr inline libmcuhw::clock::McuClockConfig<libmcuhw::clock::ClockInputSources::XTAL, 12'000'000u, 30'000'000u>
  nuclone_clock_config;
constexpr inline libmcuhw::clock::PeriClockConfig<nuclone_clock_config, libmcuhw::clock::PeriSelect::UART0,
                                                  libmcuhw::clock::PeriSource::MAIN>
  uart0_clock_config;
constexpr inline libmcuhw::clock::PeriClockConfig<nuclone_clock_config, libmcuhw::clock::PeriSelect::I2C0,
                                                  libmcuhw::clock::PeriSource::MAIN>
  i2c0_clock_config;
extern libmcull::usart::UartPolled<libmcuhw::Usart0Address, UartTransferType> usart_peripheral;
extern libmcull::i2c::I2cInterrupt<libmcuhw::I2c0Address> i2c_interrupt_peripheral;
extern libmcuhal::usart::UartPolled<usart_peripheral, UartTransferType> hal_usart_peripheral;
extern libmcuhal::i2c::I2cInterrupt<i2c_interrupt_peripheral> hal_i2c_interrupt_peripheral;

void board_init(void);

#endif