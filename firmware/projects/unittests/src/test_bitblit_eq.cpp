/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file test_bitblit_eq.cpp
 * @brief bitblit tests for equal data sizes
 */

#include <MinUnit.h>
#include <cstring>
#include <cstdint>
#include <array>
#include <libmcu/libmcu_functions.hpp>
#include <libmcu/bitmap/bitblit1d.hpp>

MINUNIT_ADD(BitBlit1Bbp8To8Move, NULL, NULL) {
  minUnitPass();
  std::array<uint8_t, 5> testDest;
  std::array<uint8_t, 4> testSrc{0x21, 0x43, 0x65, 0x87};
  // test case: less then element write with destination offset
  testDest.fill(0xA5);
  libMcu::bitmap::bitblit<1>(std::span<std::uint8_t>(testDest), 4u, std::span<const std::uint8_t>(testSrc), 0u, 4u,
                             libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[0] == 0xA1);
  // test case: less then element write with source offset
  // test case: less then element write with source and destination offset
  // test case: less then element write with boundary cross
  // test case: full element write with boundary cross
  // test case: full element write with end of buffer clamp, use a subspan
  // test case: multiple element write
  // test case: multiple element write with
}
/*
MINUNIT_ADD(testBitBlit1DMove8bitTo8bit, NULL, NULL) {
  std::array<uint8_t, 5> testDest;
  std::array<uint8_t, 4> testSrc{0x21, 0x43, 0x65, 0x87};
  // test single element write
  testDest.fill(0xA5);
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), 0u, std::span<const std::uint8_t>(testSrc), 8u,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[0] == 0x21);
  // test multi element write
  testDest.fill(0xA5);
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), 0u, std::span<const std::uint8_t>(testSrc), 16u,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[0] == 0x21);
  minUnitCheck(testDest[1] == 0x43);
  // test offset multi element write
  testDest.fill(0xA5);
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), 8u, std::span<const std::uint8_t>(testSrc), 16u,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0x21);
  minUnitCheck(testDest[2] == 0x43);
  // test multi element write with incomplete last element
  testDest.fill(0xA5);
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), 8u, std::span<const std::uint8_t>(testSrc), 12u,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0x21);
  minUnitCheck(testDest[2] == 0xA3);
  // test multi element write with incomplete first element
  testDest.fill(0xA5);
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), 4u, std::span<const std::uint8_t>(testSrc), 12u,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[0] == 0x15);
  minUnitCheck(testDest[1] == 0x32);
  // test out of bounds multi element write that it gets clipped properly
  testDest.fill(0xA5);
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest).subspan(1, 2), 0u, std::span<const std::uint8_t>(testSrc), 24u,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0x21);
  minUnitCheck(testDest[2] == 0x43);
  minUnitCheck(testDest[3] == 0xA5);  // check if we wrote out of bounds
}
*/
/*
MINUNIT_ADD(testBitBlit1DMove8bitTo16bit, NULL, NULL) {
  std::array<uint16_t, 4> testDest;
  testDest.fill(0xA5A5);
  std::array<uint8_t, 4> testSrc{0x12, 0x34, 0x56, 0x78};
}
MINUNIT_ADD(testBitBlit1DMove16bitTo8bit, NULL, NULL) {
  std::array<uint8_t, 5> testDest;
  testDest.fill(0xA5);
  std::array<uint16_t, 2> testSrc{0x1234, 0x5678};
}
*/
/*
MINUNIT_ADD(testBitBlit1DCases, NULL, NULL) {
  // we add one extra byte as canary to check out of bound writes
  std::array<uint8_t, 5> testDest{0xA5, 0xA5, 0xA5, 0xA5, 0xA5};
  std::array<uint8_t, 4> testSrc{0x12, 0x34, 0x56, 0x78};
  std::size_t testDestSize = 32u;
  // less then one byte aligned
  testDest.fill(0xA5);
  testSrc[0] = 0x33;
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 16u, std::span<std::uint8_t>(testSrc), 4u,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[2] == 0xA3);
  // less then one byte not aligned
  testDest.fill(0xA5);
  testSrc[0] = 0x13;
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 12u, std::span<std::uint8_t>(testSrc), 4u,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0x35);
  // less then one byte crossing
  testDest.fill(0xA5);
  testSrc[0] = 0x49;
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 20, std::span<std::uint8_t>(testSrc), 8,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[2] == 0x95);
  minUnitCheck(testDest[3] == 0xA4);
  // single byte aligned
  testDest.fill(0xA5);
  testSrc[0] = 0x13;
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 8, std::span<std::uint8_t>(testSrc), 8,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0x13);
  // single byte crossing boundary
  testDest.fill(0xA5);
  testSrc[0] = 0x2C;
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 4, std::span<std::uint8_t>(testSrc), 8,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[0] == 0xC5);
  minUnitCheck(testDest[1] == 0xA2);
  // single byte crossing boundary at the edge
  testDest.fill(0xA5);
  testSrc[0] = 0x6F;
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 28, std::span<std::uint8_t>(testSrc), 8,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[3] == 0xF5);
  minUnitCheck(testDest[4] == 0xA5);
  // two byte aligned
  testDest.fill(0xA5);
  testSrc[0] = 0x29;
  testSrc[1] = 0x7A;
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 8, std::span<std::uint8_t>(testSrc), 16,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0x29);
  minUnitCheck(testDest[2] == 0x7A);
  // two byte aligned, crossing boundary
  testDest.fill(0xA5);
  testSrc[0] = 0x13;
  testSrc[1] = 0x9C;
  testSrc[2] = 0xC1;
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 8, std::span<std::uint8_t>(testSrc), 20,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0x13);
  minUnitCheck(testDest[2] == 0x9C);
  minUnitCheck(testDest[3] == 0xA1);
  // two byte crossing boundary
  testDest.fill(0xA5);
  testSrc[0] = 0x2C;
  testSrc[1] = 0x9C;
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 12, std::span<std::uint8_t>(testSrc), 16,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0xC5);
  minUnitCheck(testDest[2] == 0xC2);
  minUnitCheck(testDest[3] == 0xA9);
  // n byte aligned
  testDest.fill(0xA5);
  testSrc[0] = 0x12;
  testSrc[1] = 0x34;
  testSrc[2] = 0x56;
  testSrc[3] = 0x78;
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 0, std::span<std::uint8_t>(testSrc), 24,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[0] == 0x12);
  minUnitCheck(testDest[1] == 0x34);
  minUnitCheck(testDest[2] == 0x56);
  minUnitCheck(testDest[3] == 0xA5);
  minUnitCheck(testDest[4] == 0xA5);
  // n byte aligned at the edge
  testDest.fill(0xA5);
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 24, std::span<std::uint8_t>(testSrc), 24,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[2] == 0xA5);
  minUnitCheck(testDest[3] == 0x12);
  minUnitCheck(testDest[4] == 0xA5);
  // n byte aligned crossing boundary
  testDest.fill(0xA5);
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 0, std::span<std::uint8_t>(testSrc), 28,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[0] == 0x12);
  minUnitCheck(testDest[1] == 0x34);
  minUnitCheck(testDest[2] == 0x56);
  minUnitCheck(testDest[3] == 0xA8);
  minUnitCheck(testDest[4] == 0xA5);
  // n byte crossing boundary
  testDest.fill(0xA5);
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 4, std::span<std::uint8_t>(testSrc), 24,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[0] == 0x25);
  minUnitCheck(testDest[1] == 0x41);
  minUnitCheck(testDest[2] == 0x63);
  minUnitCheck(testDest[3] == 0xA5);
  minUnitCheck(testDest[4] == 0xA5);
  // n byte crossing boundary at the edge
  testDest.fill(0xA5);
  libMcu::bitmap::bitblit1d(std::span<std::uint8_t>(testDest), testDestSize, 20, std::span<std::uint8_t>(testSrc), 28,
                            libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[2] == 0x25);
  minUnitCheck(testDest[3] == 0x41);
  minUnitCheck(testDest[4] == 0xA5);
}
*/

/*
MINUNIT_ADD(testBitBlit1DOperations, NULL, NULL) {
  minUnitPass();
}
*/

// bitblit tests for 16 bit