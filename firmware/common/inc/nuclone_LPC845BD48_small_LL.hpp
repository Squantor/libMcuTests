/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @brief definitions for LPC845 small nuclone LL tester
 */
#ifndef NUCLONE_LPC845BD48_SMALL_LL_HPP
#define NUCLONE_LPC845BD48_SMALL_LL_HPP

#include <nxp/libmcu_LPC845M301BD48_ll.hpp>

#define TICKS_PER_S 10

// pin types
// Crystal osillator pins
using xtalInPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN08>;
using xtalOutPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN09>;
using testPinType = libMcuHw::pin<libMcuHw::IOports::PORT1, libMcuHw::IOpins::PIN05>;
// connected pin 1 and 2 via 3.3K resistors, links UART RX/TX, PIN1 on board
using pin1Type = libMcuHw::pin<libMcuHw::IOports::PORT1, libMcuHw::IOpins::PIN08>;
using pin1PortType = libMcuHw::port<libMcuHw::IOports::PORT1>;
using pin2Type = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN17>;
using pin2PortType = libMcuHw::port<libMcuHw::IOports::PORT0>;
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO, PIN3 on board
using pin3Type = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN13>;
using pin3PortType = libMcuHw::port<libMcuHw::IOports::PORT0>;
using pin4Type = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN18>;
using pin4PortType = libMcuHw::port<libMcuHw::IOports::PORT0>;
// connected pin 5 and 6 via 3.3K resistors, links SPI CS/SCK, PIN2 on board
using pin5Type = libMcuHw::pin<libMcuHw::IOports::PORT1, libMcuHw::IOpins::PIN09>;
using pin5PortType = libMcuHw::port<libMcuHw::IOports::PORT1>;
using pin6Type = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN19>;
using pin6PortType = libMcuHw::port<libMcuHw::IOports::PORT0>;
// connected pin 7 and 8 via 3.3K resistors
using pin7Type = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN04>;
using pin7PortType = libMcuHw::port<libMcuHw::IOports::PORT0>;
using pin8Type = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN20>;
using pin8PortType = libMcuHw::port<libMcuHw::IOports::PORT0>;
// SDA pin with 2.2K pull up resistor
using i2cSdaPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN11>;
// SCL pin with 2.2K pull up resistor
using i2cSclPinType = libMcuHw::pin<libMcuHw::IOports::PORT0, libMcuHw::IOpins::PIN10>;

// function types
using xtalInFunctionType = libMcuHw::swm::pinFunction<libMcuHw::swm::pinFunctions::XTALIN>;
using xtalOutFunctionType = libMcuHw::swm::pinFunction<libMcuHw::swm::pinFunctions::XTALOUT>;
using clockOutFunctionType = libMcuHw::swm::pinFunction<libMcuHw::swm::pinFunctions::CLKOUT_O>;
using mainI2cSdaFunctionType = libMcuHw::swm::pinFunction<libMcuHw::swm::pinFunctions::I2C0_SDA_IO>;
using mainI2cSclFunctionType = libMcuHw::swm::pinFunction<libMcuHw::swm::pinFunctions::I2C0_SCL_IO>;
using mainUartTxFunctionType = libMcuHw::swm::pinFunction<libMcuHw::swm::pinFunctions::U0_TXD_O>;
using mainUartRxFunctionType = libMcuHw::swm::pinFunction<libMcuHw::swm::pinFunctions::U0_RXD_I>;

// pin instances
constexpr xtalInPinType xtalInPin;
constexpr xtalOutPinType xtalOutPin;
constexpr testPinType testPin;
constexpr i2cSclPinType i2cSclPin;
constexpr i2cSdaPinType i2cSdaPin;
constexpr pin1Type testPin1;
constexpr pin2Type testPin2;
constexpr pin3Type testPin3;
constexpr pin4Type testPin4;
constexpr pin5Type testPin5;
constexpr pin6Type testPin6;
constexpr pin7Type testPin7;
constexpr pin8Type testPin8;
constexpr pin1PortType testPin1Port;
constexpr pin2PortType testPin2Port;
constexpr pin3PortType testPin3Port;
constexpr pin4PortType testPin4Port;
constexpr pin5PortType testPin5Port;
constexpr pin6PortType testPin6Port;
constexpr pin7PortType testPin7Port;
constexpr pin8PortType testPin8Port;

// function instances
constexpr xtalInFunctionType xtalInFunction;
constexpr xtalOutFunctionType xtalOutFunction;
constexpr clockOutFunctionType clockOutFunction;
constexpr mainI2cSclFunctionType i2cMainSclFunction;
constexpr mainI2cSdaFunctionType i2cMainSdaFunction;
constexpr mainUartTxFunctionType uartMainTxFunction;
constexpr mainUartRxFunctionType uartMainRxFunction;

// peripheral externs
extern libMcuLL::iocon::iocon<libMcuHw::ioconAddress> ioconPeripheral;
extern libMcuLL::swm::swm<libMcuHw::swmAddress> swmPeriperhal;
extern libMcuLL::gpio::gpio<libMcuHw::gpioAddress> gpioPeripheral;
extern libMcuLL::syscon::syscon<libMcuHw::sysconAddress> sysconPeripheral;
extern libMcuLL::usart::usart<libMcuHw::usart0Address, std::uint8_t> usartPeripheral;
constexpr inline libMcuHw::clock::mcuClockConfig<libMcuHw::clock::clockInputSources::XTAL, 12'000'000u, 30'000'000u>
  nucloneClockConfig;
constexpr inline libMcuHw::clock::periClockConfig<nucloneClockConfig, libMcuHw::clock::periSelect::UART0,
                                                  libMcuHw::clock::periSource::MAIN>
  uart0ClockConfig;

void boardInit(void);

#endif