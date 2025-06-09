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
using gpioPort = libmcuhw::Port<libmcuhw::IoPorts::kPort0>;

// Crystal osillator pins
using xtalInPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin08>;
using xtalOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin09>;
// test pin pair 0, connected to eachother via 3.3KOhm resistor, testboard PIN1
using test0PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin00>;
using test1PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin14>;
// auxillary test pin, connected to nothing, testboard PIN2
using test2PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin07>;
// auxillary test pin, connected to nothing, testboard PIN3
using test3PinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin06>;
// PWM test pin pair, connected to 1K 10uF RC, testboard RC_out1
using testPwmInPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin01>;
// PWM test pin pair, connected to 1K 10uF RC, testboard RC_in1
using testPwmOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin15>;
// I2C Clock pin pair, pulled up to 2.2K to 3.3V
using i2cSclOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin10>;
// I2C Data pin pair, pulled up to 2.2K to 3.3V
using i2cSdaOutPinType = libmcuhw::Pin<libmcuhw::IoPorts::kPort0, libmcuhw::IoPins::kPin11>;

// Peripheral functions
using mainI2cSdaFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kI2cSdaInOut>;
using mainI2cSclFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kI2cSclInOut>;
using xtalInFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kXtalIn>;
using xtalOutFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kXtalOut>;
using mainSpiSckFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kSpi0SckInOut>;
using mainSpiSselFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kSpi0SselInOut>;
using mainSpiMosiFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kSpi0MosiInOut>;
using mainSpiMisoFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kSpi0MisoInOut>;
using mainUartTxFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kUart0TxOut>;
using mainUartRxFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kUart0RxIn>;
using sctOutput0FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kSct0Out0Out>;
using sctOutput1FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kSct0Out1Out>;
using sctOutput2FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kSct0Out2Out>;
using sctOutput3FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kSct0Out3Out>;
using sctInput0FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kSct0In0In>;
using sctInput1FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kSct0In1In>;
using sctInput2FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kSct0In2In>;
using sctInput3FunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kSct0In3In>;

constexpr xtalInPinType xtalInPin;
constexpr xtalOutPinType xtalOutPin;
constexpr test0PinType test0Pin;
constexpr test1PinType test1Pin;
constexpr test2PinType test2Pin;
constexpr test3PinType test3Pin;
constexpr i2cSclOutPinType i2cSclOutPin;
constexpr i2cSdaOutPinType i2cSdaOutPin;
constexpr testPwmInPinType pwmInPin;
constexpr testPwmOutPinType pwmOutPin;
constexpr gpioPort mainPort;

constexpr xtalInFunctionType xtalInFunction;
constexpr xtalOutFunctionType xtalOutFunction;
constexpr mainI2cSclFunctionType i2cMainSclFunction;
constexpr mainI2cSdaFunctionType i2cMainSdaFunction;
constexpr mainSpiSckFunctionType spiMainSckFunction;
constexpr mainSpiSselFunctionType spiMainSselFunction;
constexpr mainSpiMosiFunctionType spiMainMosiFunction;
constexpr mainSpiMisoFunctionType spiMainMisoFunction;
constexpr mainUartTxFunctionType uartMainTxFunction;
constexpr mainUartRxFunctionType uartMainRxFunction;
constexpr sctOutput0FunctionType sctOutput0Function;
constexpr sctOutput1FunctionType sctOutput1Function;
constexpr sctOutput2FunctionType sctOutput2Function;
constexpr sctOutput3FunctionType sctOutput3Function;
constexpr sctInput0FunctionType sctInput0Function;
constexpr sctInput1FunctionType sctInput1Function;
constexpr sctInput2FunctionType sctInput2Function;
constexpr sctInput3FunctionType sctInput3Function;

extern libmcull::iocon::Iocon<libmcuhw::kIoconAddress> ioconPeripheral;
extern libmcull::swm::Swm<libmcuhw::kSwmAddress> swmPeriperhal;
extern libmcull::gpio::Gpio<libmcuhw::kGpioAddress> gpioPeripheral;
extern libmcull::spi::SpiPolled<libmcuhw::kSpi0Address, libmcull::spi::ChipEnables, std::uint16_t> spiSyncPeripheral;
extern libmcull::spi::SpiAsync<libmcuhw::kSpi0Address, libmcull::spi::ChipEnables, std::uint16_t> spiAsyncPeripheral16;
extern libmcull::spi::SpiAsync<libmcuhw::kSpi0Address, libmcull::spi::ChipEnables, std::uint8_t> spiAsyncPeripheral8;
extern libmcull::syscon::Syscon<libmcuhw::kSysconAddress> sysconPeripheral;
extern libmcull::usart::usartSync<libmcuhw::kUsart0Address, std::uint8_t> usartSyncPeripheral;
extern libmcull::usart::UsartAsync<libmcuhw::kUsart0Address, std::uint8_t> usartAsyncPeripheral;
extern libmcull::i2c::I2c<libmcuhw::kI2c0Address> i2cPeripheral;
extern libmcull::sct::Sct<libmcuhw::kSctAddress> sctPeripheral;
extern libmcull::acmp::Acmp<libmcuhw::kAcmpAddress> acmpPeripheral;

void boardInit(void);
void crudeDelay(uint32_t iterations);

#endif