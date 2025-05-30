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
using xtalInPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN08>;
using xtalOutPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN09>;
using testPinType = libmcuhw::pin<libmcuhw::IOports::PORT1, libmcuhw::IOpins::PIN05>;
// connected pin 1 and 2 via 3.3K resistors, links UART RX/TX, PIN1 on board
using pin1Type = libmcuhw::pin<libmcuhw::IOports::PORT1, libmcuhw::IOpins::PIN08>;
using pin1PortType = libmcuhw::port<libmcuhw::IOports::PORT1>;
using pin2Type = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN17>;
using pin2PortType = libmcuhw::port<libmcuhw::IOports::PORT0>;
// connected pin 3 and 4 via 3.3K resistors, links SPI MOSI/MISO, PIN3 on board
using pin3Type = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN13>;
using pin3PortType = libmcuhw::port<libmcuhw::IOports::PORT0>;
using pin4Type = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN18>;
using pin4PortType = libmcuhw::port<libmcuhw::IOports::PORT0>;
// connected pin 5 and 6 via 3.3K resistors, links SPI CS/SCK, PIN2 on board
using pin5Type = libmcuhw::pin<libmcuhw::IOports::PORT1, libmcuhw::IOpins::PIN09>;
using pin5PortType = libmcuhw::port<libmcuhw::IOports::PORT1>;
using pin6Type = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN19>;
using pin6PortType = libmcuhw::port<libmcuhw::IOports::PORT0>;
// connected pin 7 and 8 via 3.3K resistors
using pin7Type = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN04>;
using pin7PortType = libmcuhw::port<libmcuhw::IOports::PORT0>;
using pin8Type = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN20>;
using pin8PortType = libmcuhw::port<libmcuhw::IOports::PORT0>;
// SDA pin with 2.2K pull up resistor
using i2cSdaPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN11>;
// SCL pin with 2.2K pull up resistor
using i2cSclPinType = libmcuhw::pin<libmcuhw::IOports::PORT0, libmcuhw::IOpins::PIN10>;

// function types
using xtalInFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kXtalIn>;
using xtalOutFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kXtalOut>;
using clockOutFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kClkOut>;
using mainI2cSdaFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kI2c0SdaInOut>;
using mainI2cSclFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kI2c0SclInOut>;
using mainUartTxFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kUart0TxOut>;
using mainUartRxFunctionType = libmcuhw::swm::PinFunction<libmcuhw::swm::PinFunctions::kUart0RxIn>;

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

using UartTransferType = std::uint8_t;

// peripheral externs
extern libmcull::iocon::Iocon<libmcuhw::ioconAddress> ioconPeripheral;
extern libmcull::swm::Swm<libmcuhw::swmAddress> swmPeriperhal;
extern libmcull::gpio::gpio<libmcuhw::gpioAddress> gpioPeripheral;
extern libmcull::syscon::Syscon<libmcuhw::sysconAddress> sysconPeripheral;
constexpr inline libmcuhw::clock::mcuClockConfig<libmcuhw::clock::clockInputSources::XTAL, 12'000'000u, 30'000'000u>
  nucloneClockConfig;
constexpr inline libmcuhw::clock::periClockConfig<nucloneClockConfig, libmcuhw::clock::periSelect::UART0,
                                                  libmcuhw::clock::periSource::MAIN>
  uart0ClockConfig;
extern libmcull::usart::SyncUart<libmcuhw::kUsart0Address, UartTransferType> usart_peripheral;
extern libmcuhal::usart::SyncUart<usart_peripheral, UartTransferType> hal_usart_peripheral;

void boardInit(void);

#endif