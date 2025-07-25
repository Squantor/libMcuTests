/*
SPDX-License-Identifier: MIT

Copyright (c) 2023 Bart Bilos
For conditions of distribution and use, see LICENSE file
*/
#ifndef NUCLONE_LPC812M101DH20_HPP
#define NUCLONE_LPC812M101DH20_HPP

#define CLOCK_MAIN_SOURCE SYSCTL_MAINCLKSRC_PLLOUT

#define CLOCK_XTAL (12000000u)
#define CLOCK_EXT_IN (0u)
#define CLOCK_CPU (30000000u)
#define CLOCK_AHB (30000000u)
#define CLOCK_MAIN (60000000u)

#include <nxp/libmcu_LPC812M101DH20_ll.hpp>

#define TICKS_PER_S 10

// main gpio port
using GpioPort = libmcuhw::Port<libmcuhw::IoPorts::Port0>;

// Crystal osillator pins
using XtalInPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin08>;
using XtalOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin09>;
// test pin pair 0, connected to eachother via 3.3KOhm resistor, testboard PIN1
using Test0PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin00>;
using Test1PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin14>;
// auxillary test pin, connected to nothing, testboard PIN2
using Test2PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin07>;
// auxillary test pin, connected to nothing, testboard PIN3
using Test3PinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin06>;
// PWM test pin pair, connected to 1K 10uF RC, testboard RC_out1
using TestPwmInPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin01>;
// PWM test pin pair, connected to 1K 10uF RC, testboard RC_in1
using TestPwmOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin15>;
// I2C Clock pin pair, pulled up to 2.2K to 3.3V
using I2cSclOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin10>;
// I2C Data pin pair, pulled up to 2.2K to 3.3V
using I2cSdaOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::Port0, libmcuhw::IoPins::Pin11>;

// Peripheral functions
using MainI2cSdaFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::I2cSdaInOut>;
using MainI2cSclFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::I2cSclInOut>;
using XtalInFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::XtalIn>;
using XtalOutFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::XtalOut>;
using MainSpiSckFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Spi0SckInOut>;
using MainSpiSselFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Spi0SselInOut>;
using MainSpiMosiFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Spi0MosiInOut>;
using MainSpiMisoFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Spi0MisoInOut>;
using MainUartTxFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Uart0TxOut>;
using MainUartRxFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Uart0RxIn>;
using SctOutput0FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Sct0Out0Out>;
using SctOutput1FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Sct0Out1Out>;
using SctOutput2FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Sct0Out2Out>;
using SctOutput3FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Sct0Out3Out>;
using SctInput0FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Sct0In0In>;
using SctInput1FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Sct0In1In>;
using SctInput2FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Sct0In2In>;
using SctInput3FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::Sct0In3In>;

constexpr XtalInPinType xtal_in_pin;
constexpr XtalOutPinType xtal_out_pin;
constexpr Test0PinType test_0_pin;
constexpr Test1PinType test_1_pin;
constexpr Test2PinType test_2_pin;
constexpr Test3PinType test_3_pin;
constexpr I2cSclOutPinType i2c_scl_out_pin;
constexpr I2cSdaOutPinType i2c_sda_out_pin;
constexpr TestPwmInPinType pwm_in_pin;
constexpr TestPwmOutPinType pwm_out_pin;
constexpr GpioPort main_port;

constexpr XtalInFunctionType xtal_in_function;
constexpr XtalOutFunctionType xtal_out_function;
constexpr MainI2cSclFunctionType i2c_main_scl_function;
constexpr MainI2cSdaFunctionType i2c_main_sda_function;
constexpr MainSpiSckFunctionType spi_main_sck_function;
constexpr MainSpiSselFunctionType spi_main_ssel_function;
constexpr MainSpiMosiFunctionType spi_main_mosi_function;
constexpr MainSpiMisoFunctionType spi_main_miso_function;
constexpr MainUartTxFunctionType uart_main_tx_function;
constexpr MainUartRxFunctionType uart_main_rx_function;
constexpr SctOutput0FunctionType sct_output_0_function;
constexpr SctOutput1FunctionType sct_output_1_function;
constexpr SctOutput2FunctionType sct_output_2_function;
constexpr SctOutput3FunctionType sct_output_3_function;
constexpr SctInput0FunctionType sct_intput_0_function;
constexpr SctInput1FunctionType sct_input_1_function;
constexpr SctInput2FunctionType sct_input_2_function;
constexpr SctInput3FunctionType sct_input_3_function;

extern libmcull::iocon::Iocon<libmcuhw::IoconAddress> iocon_peripheral;
extern libmcull::swm::Swm<libmcuhw::SwmAddress> swm_peripheral;
extern libmcull::gpio::Gpio<libmcuhw::GpioAddress> gpio_peripheral;
extern libmcull::spi::SpiPolled<libmcuhw::Spi0Address, libmcull::spi::ChipEnables, std::uint16_t> spi_polled_peripheral;
//! @todo these peripherals need to be refactored/renamed
extern libmcull::spi::SpiAsync<libmcuhw::Spi0Address, libmcull::spi::ChipEnables, std::uint16_t> spiAsyncPeripheral16;
extern libmcull::spi::SpiAsync<libmcuhw::Spi0Address, libmcull::spi::ChipEnables, std::uint8_t> spiAsyncPeripheral8;
extern libmcull::syscon::Syscon<libmcuhw::SysconAddress> syscon_peripheral;
extern libmcull::usart::UsartPolled<libmcuhw::Usart0Address, std::uint8_t> usart_polled_peripheral;
extern libmcull::usart::UsartAsync<libmcuhw::Usart0Address, std::uint8_t> usartAsyncPeripheral;
extern libmcull::i2c::I2cPolled<libmcuhw::I2c0Address> i2c_peripheral;
extern libmcull::sct::Sct<libmcuhw::SctAddress> sct_peripheral;
extern libmcull::acmp::Acmp<libmcuhw::AcmpAddress> acmp_peripheral;

void board_init(void);
void crudeDelay(uint32_t iterations);

#endif