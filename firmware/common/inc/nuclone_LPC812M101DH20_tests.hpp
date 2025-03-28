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
using gpioPort = libmcuhw::port<libmcuhw::IOports::PORT0>;

// Crystal osillator pins
using xtalInPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN08>;
using xtalOutPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN09>;
// test pin pair 0, connected to eachother via 3.3KOhm resistor, testboard PIN1
using test0PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN00>;
using test1PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN14>;
// auxillary test pin, connected to nothing, testboard PIN2
using test2PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN07>;
// auxillary test pin, connected to nothing, testboard PIN3
using test3PinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN06>;
// PWM test pin pair, connected to 1K 10uF RC, testboard RC_out1
using testPwmInPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN01>;
// PWM test pin pair, connected to 1K 10uF RC, testboard RC_in1
using testPwmOutPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN15>;
// I2C Clock pin pair, pulled up to 2.2K to 3.3V
using i2cSclOutPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN10>;
// I2C Data pin pair, pulled up to 2.2K to 3.3V
using i2cSdaOutPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN11>;

// Peripheral functions
using mainI2cSdaFunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::I2C_SDA_IO>;
using mainI2cSclFunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::I2C_SCL_IO>;
using xtalInFunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::XTALIN>;
using xtalOutFunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::XTALOUT>;
using mainSpiSckFunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::SPI0_SCK_IO>;
using mainSpiSselFunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::SPI0_SSEL_IO>;
using mainSpiMosiFunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::SPI0_MOSI_IO>;
using mainSpiMisoFunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::SPI0_MISO_IO>;
using mainUartTxFunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::U0_TXD_O>;
using mainUartRxFunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::U0_RXD_I>;
using sctOutput0FunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::CTOUT_0_O>;
using sctOutput1FunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::CTOUT_1_O>;
using sctOutput2FunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::CTOUT_2_O>;
using sctOutput3FunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::CTOUT_3_O>;
using sctInput0FunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::CTIN_0_I>;
using sctInput1FunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::CTIN_1_I>;
using sctInput2FunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::CTIN_2_I>;
using sctInput3FunctionType = libmcuhw::swm::pinFunction<libmcuhw::swm::pinFunctions::CTIN_3_I>;

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

extern libmcull::sw::iocon::iocon<libmcuhw::ioconAddress> ioconPeripheral;
extern libmcull::sw::swm::swm<libmcuhw::swmAddress> swmPeriperhal;
extern libmcull::sw::gpio::gpio<libmcuhw::gpioAddress> gpioPeripheral;
extern libmcull::sw::spi::spiSync<libmcuhw::spi0Address, libmcull::sw::spi::chipEnables, std::uint16_t> spiSyncPeripheral;
extern libmcull::sw::spi::spiAsync<libmcuhw::spi0Address, libmcull::sw::spi::chipEnables, std::uint16_t> spiAsyncPeripheral16;
extern libmcull::sw::spi::spiAsync<libmcuhw::spi0Address, libmcull::sw::spi::chipEnables, std::uint8_t> spiAsyncPeripheral8;
extern libmcull::sw::syscon::syscon<libmcuhw::sysconAddress> sysconPeripheral;
extern libmcull::sw::usart::usartSync<libmcuhw::usart0Address, std::uint8_t> usartSyncPeripheral;
extern libmcull::sw::usart::usartAsync<libmcuhw::usart0Address, std::uint8_t> usartAsyncPeripheral;
extern libmcull::sw::i2c::i2c<libmcuhw::i2c0Address> i2cPeripheral;
extern libmcull::sw::sct::sct<libmcuhw::sctAddress> sctPeripheral;
extern libmcull::sw::acmp::acmp<libmcuhw::acmpAddress> acmpPeripheral;

void boardInit(void);
void crudeDelay(uint32_t iterations);

#endif