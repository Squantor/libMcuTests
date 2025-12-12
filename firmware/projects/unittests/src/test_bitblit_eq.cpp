/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file test_Bitblit_eq.cpp
 * @brief Bitblit tests for equal data sizes
 */

#include <MinUnit.h>
#include <cstring>
#include <cstdint>
#include <array>
#include <libmcu/functions.hpp>
#include <libmcu/bitmap/bitblit1d.hpp>

MINUNIT_ADD(BitBlit1Bbp8To8Move, NULL, NULL) {
  std::array<std::uint8_t, 5> testDest;
  std::array<std::uint8_t, 4> testSrc{0x21, 0x43, 0x65, 0x87};
  // test case: less then element write with destination offset
  testDest.fill(0xA5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint8_t>(testDest), 4u, std::span<const std::uint8_t>(testSrc), 0u, 4u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x15);
  // test case: less then element write with source offset
  testDest.fill(0xA5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint8_t>(testDest), 0u, std::span<const std::uint8_t>(testSrc), 4u, 4u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0xA2);
  // test case: less then element write with source and destination offset
  testDest.fill(0xA5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint8_t>(testDest), 2u, std::span<const std::uint8_t>(testSrc), 2u, 4u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0xA1);
  // test case: less then element write with boundary cross
  testDest.fill(0xA5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint8_t>(testDest), 6u, std::span<const std::uint8_t>(testSrc), 0u, 4u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x65);
  minUnitCheck(testDest[1] == 0xA4);
  // test case: full element write with boundary cross
  testDest.fill(0xA5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint8_t>(testDest), 4u, std::span<const std::uint8_t>(testSrc), 0u, 8u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x15);
  minUnitCheck(testDest[1] == 0xA2);
  // test case: full element write with end of buffer clamp, use a subspan
  testDest.fill(0xA5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint8_t>(testDest).subspan(0, 1), 4u, std::span<const std::uint8_t>(testSrc), 0u, 8u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x15);
  minUnitCheck(testDest[1] == 0xA5);
  // test case: multiple element write with clamp and offsets
  testDest.fill(0xA5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint8_t>(testDest).subspan(0, 2), 4u, std::span<const std::uint8_t>(testSrc), 0u, 20u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x15);
  minUnitCheck(testDest[1] == 0x32);
  minUnitCheck(testDest[2] == 0xA5);
}

MINUNIT_ADD(BitBlit1Bbp16To16Move, NULL, NULL) {
  std::array<std::uint16_t, 4> testDest;
  std::array<std::uint16_t, 3> testSrc{0x3210, 0x7654, 0xBA98};
  // test case: less then element write with destination offset
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint16_t>(testDest), 4u, std::span<const std::uint16_t>(testSrc), 0u, 8u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0xA105);
  // test case: less then element write with source offset
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint16_t>(testDest), 0u, std::span<const std::uint16_t>(testSrc), 4u, 8u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0xA521);
  // test case: less then element write with source and destination offset
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint16_t>(testDest), 4u, std::span<const std::uint16_t>(testSrc), 4u, 8u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0xA215);
  // test case: less then element write with boundary cross
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint16_t>(testDest), 12u, std::span<const std::uint16_t>(testSrc), 0u, 8u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x05A5);
  minUnitCheck(testDest[1] == 0xA5A1);
  // test case: full element write with boundary cross
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint16_t>(testDest), 8u, std::span<const std::uint16_t>(testSrc), 0u, 16u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x10A5);
  minUnitCheck(testDest[1] == 0xA532);
  // test case: full element write with end of buffer clamp, use a subspan
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint16_t>(testDest).subspan(0, 1), 4u, std::span<const std::uint16_t>(testSrc), 0u, 24u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x2105);
  minUnitCheck(testDest[1] == 0xA5A5);
  // test case: multiple element write with clamp and offsets
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<1>(std::span<std::uint16_t>(testDest).subspan(0, 2), 4u, std::span<const std::uint16_t>(testSrc), 0u, 40u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x2105);
  minUnitCheck(testDest[1] == 0x6543);
  minUnitCheck(testDest[2] == 0xA5A5);
}

MINUNIT_ADD(BitBlit4Bbp16To16Move, NULL, NULL) {
  std::array<std::uint16_t, 4> testDest;
  std::array<std::uint16_t, 3> testSrc{0x3210, 0x7654, 0xBA98};
  // test case: less then element write with destination offset
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest), 1u, std::span<const std::uint16_t>(testSrc), 0u, 2u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0xA105);
  // test case: less then element write with source offset
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest), 0u, std::span<const std::uint16_t>(testSrc), 1u, 2u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0xA521);
  // test case: less then element write with source and destination offset
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest), 1u, std::span<const std::uint16_t>(testSrc), 1u, 2u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0xA215);
  // test case: less then element write with boundary cross
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest), 3u, std::span<const std::uint16_t>(testSrc), 0u, 2u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x05A5);
  minUnitCheck(testDest[1] == 0xA5A1);
  // test case: full element write with boundary cross
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest), 2u, std::span<const std::uint16_t>(testSrc), 0u, 4u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x10A5);
  minUnitCheck(testDest[1] == 0xA532);
  // test case: full element write with end of buffer clamp, use a subspan
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest).subspan(0, 1), 1u, std::span<const std::uint16_t>(testSrc), 0u, 6u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x2105);
  minUnitCheck(testDest[1] == 0xA5A5);
  // test case: multiple element write with clamp and offsets
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest).subspan(0, 2), 1u, std::span<const std::uint16_t>(testSrc), 0u, 10u,
                             libmcu::bitmap::BitblitOperations::Move);
  minUnitCheck(testDest[0] == 0x2105);
  minUnitCheck(testDest[1] == 0x6543);
  minUnitCheck(testDest[2] == 0xA5A5);
}

MINUNIT_ADD(BitBlit4Bbp16To16And, NULL, NULL) {
  std::array<std::uint16_t, 4> testDest;
  std::array<std::uint16_t, 3> testSrc{0x3210, 0x7654, 0xBA98};
  // test case: less then element write with destination offset
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest), 1u, std::span<const std::uint16_t>(testSrc), 0u, 2u,
                             libmcu::bitmap::BitblitOperations::And);
  minUnitCheck(testDest[0] == 0xA105);
  // test case: less then element write with source offset
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest), 0u, std::span<const std::uint16_t>(testSrc), 1u, 2u,
                             libmcu::bitmap::BitblitOperations::And);
  minUnitCheck(testDest[0] == 0xA521);
  // test case: less then element write with source and destination offset
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest), 1u, std::span<const std::uint16_t>(testSrc), 1u, 2u,
                             libmcu::bitmap::BitblitOperations::And);
  minUnitCheck(testDest[0] == 0xA005);
  // test case: less then element write with boundary cross
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest), 3u, std::span<const std::uint16_t>(testSrc), 0u, 2u,
                             libmcu::bitmap::BitblitOperations::And);
  minUnitCheck(testDest[0] == 0x05A5);
  minUnitCheck(testDest[1] == 0xA5A1);
  // test case: full element write with boundary cross
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest), 2u, std::span<const std::uint16_t>(testSrc), 0u, 4u,
                             libmcu::bitmap::BitblitOperations::And);
  minUnitCheck(testDest[0] == 0x00A5);
  minUnitCheck(testDest[1] == 0xA520);
  // test case: full element write with end of buffer clamp, use a subspan
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest).subspan(0, 1), 1u, std::span<const std::uint16_t>(testSrc), 0u, 6u,
                             libmcu::bitmap::BitblitOperations::And);
  minUnitCheck(testDest[0] == 0x2105);
  minUnitCheck(testDest[1] == 0xA5A5);
  // test case: multiple element write with clamp and offsets
  testDest.fill(0xA5A5);
  libmcu::bitmap::Bitblit<4>(std::span<std::uint16_t>(testDest).subspan(0, 2), 1u, std::span<const std::uint16_t>(testSrc), 0u, 10u,
                             libmcu::bitmap::BitblitOperations::And);
  minUnitCheck(testDest[0] == 0x2105);
  minUnitCheck(testDest[1] == 0x2501);  // 0x6543
  minUnitCheck(testDest[2] == 0xA5A5);
}

// TODO: Bitblit tests for 16 bit with 3 bit per pixel moves
