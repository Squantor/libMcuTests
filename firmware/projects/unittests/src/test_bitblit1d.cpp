/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * \file test_bitblit1d.cpp
 *
 * Bit blit 1d tests
 */

#include <MinUnit.h>
#include <string.h>
#include <stdint.h>
#include <libmcu/bitmap/bitblit1d.hpp>

MINUNIT_ADD(testBitBlit1DCases, NULL, NULL) {
  // we add one extra byte as canary to check out of bound writes
  uint8_t testDest[5]{0xA5, 0xA5, 0xA5, 0xA5, 0xA5};
  uint8_t testSrc[4]{0x12, 0x34, 0x56, 0x78};
  unsigned int testDestSize = 32;
  // less then one byte aligned
  memset(testDest, 0xA5, sizeof(testDest));
  testSrc[0] = 0x33;
  util::bitblit1d(testDest, testDestSize, static_cast<unsigned int>(16), testSrc, static_cast<unsigned int>(4),
                  util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[2] == 0xA3);
  // less then one byte not aligned
  memset(testDest, 0xA5, sizeof(testDest));
  testSrc[0] = 0x13;
  util::bitblit1d(testDest, testDestSize, static_cast<unsigned int>(12), testSrc, static_cast<unsigned int>(4),
                  util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0x35);
  // less then one byte crossing
  memset(testDest, 0xA5, sizeof(testDest));
  testSrc[0] = 0x49;
  util::bitblit1d(testDest, testDestSize, 20, testSrc, 8, util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[2] == 0x95);
  minUnitCheck(testDest[3] == 0xA4);
  // single byte aligned
  memset(testDest, 0xA5, sizeof(testDest));
  testSrc[0] = 0x13;
  util::bitblit1d(testDest, testDestSize, 8, testSrc, 8, util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0x13);
  // single byte crossing boundary
  memset(testDest, 0xA5, sizeof(testDest));
  testSrc[0] = 0x2C;
  util::bitblit1d(testDest, testDestSize, 4, testSrc, 8, util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[0] == 0xC5);
  minUnitCheck(testDest[1] == 0xA2);
  // single byte crossing boundary at the edge
  memset(testDest, 0xA5, sizeof(testDest));
  testSrc[0] = 0x6F;
  util::bitblit1d(testDest, testDestSize, 28, testSrc, 8, util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[3] == 0xF5);
  minUnitCheck(testDest[4] == 0xA5);
  // two byte aligned
  memset(testDest, 0xA5, sizeof(testDest));
  testSrc[0] = 0x29;
  testSrc[1] = 0x7A;
  util::bitblit1d(testDest, testDestSize, 8, testSrc, 16, util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0x29);
  minUnitCheck(testDest[2] == 0x7A);
  // two byte aligned, crossing boundary
  memset(testDest, 0xA5, sizeof(testDest));
  testSrc[0] = 0x13;
  testSrc[1] = 0x9C;
  testSrc[2] = 0xC1;
  util::bitblit1d(testDest, testDestSize, 8, testSrc, 20, util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0x13);
  minUnitCheck(testDest[2] == 0x9C);
  minUnitCheck(testDest[3] == 0xA1);
  // two byte crossing boundary
  memset(testDest, 0xA5, sizeof(testDest));
  testSrc[0] = 0x2C;
  testSrc[1] = 0x9C;
  util::bitblit1d(testDest, testDestSize, 12, testSrc, 16, util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[1] == 0xC5);
  minUnitCheck(testDest[2] == 0xC2);
  minUnitCheck(testDest[3] == 0xA9);
  // n byte aligned
  memset(testDest, 0xA5, sizeof(testDest));
  testSrc[0] = 0x12;
  testSrc[1] = 0x34;
  testSrc[2] = 0x56;
  testSrc[3] = 0x78;
  util::bitblit1d(testDest, testDestSize, 0, testSrc, 24, util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[0] == 0x12);
  minUnitCheck(testDest[1] == 0x34);
  minUnitCheck(testDest[2] == 0x56);
  minUnitCheck(testDest[3] == 0xA5);
  minUnitCheck(testDest[4] == 0xA5);
  // n byte aligned at the edge
  memset(testDest, 0xA5, sizeof(testDest));
  util::bitblit1d(testDest, testDestSize, 24, testSrc, 24, util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[2] == 0xA5);
  minUnitCheck(testDest[3] == 0x12);
  minUnitCheck(testDest[4] == 0xA5);
  // n byte aligned crossing boundary
  memset(testDest, 0xA5, sizeof(testDest));
  util::bitblit1d(testDest, testDestSize, 0, testSrc, 28, util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[0] == 0x12);
  minUnitCheck(testDest[1] == 0x34);
  minUnitCheck(testDest[2] == 0x56);
  minUnitCheck(testDest[3] == 0xA8);
  minUnitCheck(testDest[4] == 0xA5);
  // n byte crossing boundary
  memset(testDest, 0xA5, sizeof(testDest));
  util::bitblit1d(testDest, testDestSize, 4, testSrc, 24, util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[0] == 0x25);
  minUnitCheck(testDest[1] == 0x41);
  minUnitCheck(testDest[2] == 0x63);
  minUnitCheck(testDest[3] == 0xA5);
  minUnitCheck(testDest[4] == 0xA5);
  // n byte crossing boundary at the edge
  memset(testDest, 0xA5, sizeof(testDest));
  util::bitblit1d(testDest, testDestSize, 20, testSrc, 28, util::bitblitOperation::OP_MOV);
  minUnitCheck(testDest[2] == 0x25);
  minUnitCheck(testDest[3] == 0x41);
  minUnitCheck(testDest[4] == 0xA5);
}

MINUNIT_ADD(testBitBlit1DOperations, NULL, NULL) {
  minUnitPass();
}