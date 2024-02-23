/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef BOARD_TESTS_HPP
#define BOARD_TESTS_HPP

#define CLOCK_XTAL          (12000000u)
#define CLOCK_EXT_IN        (0u)
#define CLOCK_CPU           (12000000u)
#define CLOCK_AHB           (12000000u)
#define CLOCK_MAIN          (12000000u)

#include <mcu_ll.h>

#define TICKS_PER_S         10

#define IOCON_XTAL_IN       IOCON_PIO0_8
#define IOCON_XTAL_OUT      IOCON_PIO0_9

#define IOCON_UART_TX       IOCON_PIO13
#define PIN_UART_TX         (13u)
#define IOCON_UART_RX       IOCON_PIO17
#define PIN_UART_RX         (17u)

#define UART_DEBUG          LPC_USART0
#define UART_BAUD_RATE      (115200u)

// test pin pair 0, connected to eachother via 1KOhm resistor
#define IOCON_TESTPIN_0_0   (IOCON_PIO0_25)
#define PORT_TESTPIN_0_0    (GPIO_PORT0)
#define PIN_TESTPIN_0_0     (25u)
#define SWM_TESTPIN_0_0     (SWM_PORTPIN_P0_25)
#define IOCON_TESTPIN_0_1   (IOCON_PIO0_24)
#define PORT_TESTPIN_0_1    (GPIO_PORT0)
#define PIN_TESTPIN_0_1     (24u)
#define SWM_TESTPIN_0_1     (SWM_PORTPIN_P0_24)
// test pin pair 1, connected to eachother via 1KOhm resistor
#define IOCON_TESTPIN_1_0   (IOCON_PIO1_15)
#define PORT_TESTPIN_1_0    (GPIO_PORT1)
#define PIN_TESTPIN_1_0     (15u)
#define SWM_TESTPIN_1_0     (SWM_PORTPIN_P1_15)
#define IOCON_TESTPIN_1_1   (IOCON_PIO0_26)
#define PORT_TESTPIN_1_1    (GPIO_PORT0)
#define PIN_TESTPIN_1_1     (26u)
#define SWM_TESTPIN_1_1     (SWM_PORTPIN_P0_26)
// loose test pins
#define IOCON_TESTPIN_2     (IOCON_PIO1_14)
#define PORT_TESTPIN_2      (GPIO_PORT1)
#define PIN_TESTPIN_2       (14u)
#define SWM_TESTPIN_2       (SWM_PORTPIN_P1_14)

void boardInit(void);

#endif