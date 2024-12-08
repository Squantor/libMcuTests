/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * \file test_bitblit2dsmall.cpp
 *
 * small version bitblit 2d tests
 */

#include <MinUnit.h>
#include <string.h>
#include <stdint.h>
#include <libmcu/bitmap/bitblit2dsmall.hpp>

MINUNIT_ADD(testBitBlit2DSameBits, NULL, NULL) {
  uint8_t src[] = {0x00, 0x00, 0x00, 0x00};
  uint8_t dest[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  // destination aligned, no destination element crossing
  memset(dest, 0x22, sizeof(dest));
  memset(src, 0xcf, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 0u, 0u, src, 8u, 2u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0xcf);
  minUnitCheck(dest[1] == 0x22);
  minUnitCheck(dest[2] == 0xcf);
  minUnitCheck(dest[3] == 0x22);
  minUnitCheck(dest[4] == 0x22);
  minUnitCheck(dest[5] == 0x22);
  minUnitCheck(dest[6] == 0x22);
  minUnitCheck(dest[7] == 0x22);

  // destination aligned, destination element crossing
  memset(dest, 0x5c, sizeof(dest));
  memset(src, 0x29, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 0u, 1u, src, 12u, 2u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0x5c);
  minUnitCheck(dest[1] == 0x5c);
  minUnitCheck(dest[2] == 0x29);
  minUnitCheck(dest[3] == 0x59);
  minUnitCheck(dest[4] == 0x29);
  minUnitCheck(dest[5] == 0x59);
  minUnitCheck(dest[6] == 0x5c);
  minUnitCheck(dest[7] == 0x5c);

  // destination not aligned, no destination element crossing
  memset(dest, 0x00, sizeof(dest));
  memset(src, 0xFF, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 8u, 8u, 2u, 2u, src, 4u, 4u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0x00);
  minUnitCheck(dest[1] == 0x00);
  minUnitCheck(dest[2] == 0x3C);
  minUnitCheck(dest[3] == 0x3C);
  minUnitCheck(dest[4] == 0x3C);
  minUnitCheck(dest[5] == 0x3C);
  minUnitCheck(dest[6] == 0x00);
  minUnitCheck(dest[7] == 0x00);

  // destination not aligned, destination element crossing
  memset(dest, 0x31, sizeof(dest));
  memset(src, 0xC2, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 4u, 2u, src, 8u, 2u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0x31);
  minUnitCheck(dest[1] == 0x31);
  minUnitCheck(dest[2] == 0x31);
  minUnitCheck(dest[3] == 0x31);
  minUnitCheck(dest[4] == 0x21);
  minUnitCheck(dest[5] == 0x3C);
  minUnitCheck(dest[6] == 0x21);
  minUnitCheck(dest[7] == 0x3C);

  // destination aligned, out of bounds cross
  memset(dest, 0xCC, sizeof(dest));
  memset(src, 0xAA, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 8u, 1u, src, 12u, 2u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0xCC);
  minUnitCheck(dest[1] == 0xCC);
  minUnitCheck(dest[2] == 0xCC);
  minUnitCheck(dest[3] == 0xAA);
  minUnitCheck(dest[4] == 0xCC);
  minUnitCheck(dest[5] == 0xAA);
  minUnitCheck(dest[6] == 0xCC);
  minUnitCheck(dest[7] == 0xCC);

  // destination unaligned, out of bounds cross
  memset(dest, 0xAA, sizeof(dest));
  memset(src, 0x54, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 12u, 1u, src, 12u, 2u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0xAA);
  minUnitCheck(dest[1] == 0xAA);
  minUnitCheck(dest[2] == 0xAA);
  minUnitCheck(dest[3] == 0x4A);
  minUnitCheck(dest[4] == 0xAA);
  minUnitCheck(dest[5] == 0x4A);
  minUnitCheck(dest[6] == 0xAA);
  minUnitCheck(dest[7] == 0xAA);

  // destination unaligned, out of bounds X and Y
  memset(dest, 0x2C, sizeof(dest));
  memset(src, 0xFF, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 12u, 3u, src, 12u, 2u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0x2C);
  minUnitCheck(dest[1] == 0x2C);
  minUnitCheck(dest[2] == 0x2C);
  minUnitCheck(dest[3] == 0x2C);
  minUnitCheck(dest[4] == 0x2C);
  minUnitCheck(dest[5] == 0x2C);
  minUnitCheck(dest[6] == 0x2C);
  minUnitCheck(dest[7] == 0xFC);

  // fully out of bounds
  memset(dest, 0x2C, sizeof(dest));
  memset(src, 0xFF, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 8u, 8u, 12u, 12u, src, 8u, 4u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0x2C);
  minUnitCheck(dest[1] == 0x2C);
  minUnitCheck(dest[2] == 0x2C);
  minUnitCheck(dest[3] == 0x2C);
  minUnitCheck(dest[4] == 0x2C);
  minUnitCheck(dest[5] == 0x2C);
  minUnitCheck(dest[6] == 0x2C);
  minUnitCheck(dest[7] == 0x2C);
}

MINUNIT_ADD(testBitBlit2DBiggerBits, NULL, NULL) {
  uint8_t src[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t dest[] = {0x0000, 0x0000, 0x0000, 0x0000};
  // destination aligned, no destination element crossing
  memset(dest, 0x22, sizeof(dest));
  memset(src, 0xcf, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 0u, 0u, src, 8u, 2u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0x22cf);
  minUnitCheck(dest[1] == 0x22cf);
  minUnitCheck(dest[2] == 0x2222);
  minUnitCheck(dest[3] == 0x2222);

  // destination aligned, destination element crossing
  memset(dest, 0x5c, sizeof(dest));
  memset(src, 0x29, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 32u, 2u, 0u, 1u, src, 20u, 1u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0x5c5c);
  minUnitCheck(dest[1] == 0x5c5c);
  minUnitCheck(dest[2] == 0x2929);
  minUnitCheck(dest[3] == 0x5c59);
}

MINUNIT_ADD(testBitBlit2DSmallerBits, NULL, NULL) {
  uint16_t src[] = {0x0000, 0x0000};
  uint8_t dest[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  // destination aligned, no destination element crossing
  memset(dest, 0x22, sizeof(dest));
  memset(src, 0xcf, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 0u, 0u, src, 8u, 2u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0xcf);
  minUnitCheck(dest[1] == 0x22);
  minUnitCheck(dest[2] == 0xcf);
  minUnitCheck(dest[3] == 0x22);
  minUnitCheck(dest[4] == 0x22);
  minUnitCheck(dest[5] == 0x22);
  minUnitCheck(dest[6] == 0x22);
  minUnitCheck(dest[7] == 0x22);

  // destination aligned, destination element crossing
  memset(dest, 0x5c, sizeof(dest));
  memset(src, 0x29, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 0u, 1u, src, 12u, 2u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0x5c);
  minUnitCheck(dest[1] == 0x5c);
  minUnitCheck(dest[2] == 0x29);
  minUnitCheck(dest[3] == 0x59);
  minUnitCheck(dest[4] == 0x29);
  minUnitCheck(dest[5] == 0x59);
  minUnitCheck(dest[6] == 0x5c);
  minUnitCheck(dest[7] == 0x5c);
}

MINUNIT_ADD(testBitBlit2DOperations, NULL, NULL) {
  uint8_t src[] = {0x00, 0x00, 0x00, 0x00};
  uint8_t dest[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  // operation AND
  memset(dest, 0x22, sizeof(dest));
  memset(src, 0x0F, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 0u, 0u, src, 8u, 2u, libMcu::bitmap::bitblitOperation::OP_AND);
  minUnitCheck(dest[0] == 0x02);
  minUnitCheck(dest[1] == 0x22);
  minUnitCheck(dest[2] == 0x02);
  minUnitCheck(dest[3] == 0x22);
  minUnitCheck(dest[4] == 0x22);
  minUnitCheck(dest[5] == 0x22);
  minUnitCheck(dest[6] == 0x22);
  minUnitCheck(dest[7] == 0x22);
  // operation move
  memset(dest, 0x22, sizeof(dest));
  memset(src, 0x0F, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 0u, 0u, src, 8u, 2u, libMcu::bitmap::bitblitOperation::OP_MOV);
  minUnitCheck(dest[0] == 0x0F);
  minUnitCheck(dest[1] == 0x22);
  minUnitCheck(dest[2] == 0x0F);
  minUnitCheck(dest[3] == 0x22);
  minUnitCheck(dest[4] == 0x22);
  minUnitCheck(dest[5] == 0x22);
  minUnitCheck(dest[6] == 0x22);
  minUnitCheck(dest[7] == 0x22);
  // operation not
  memset(dest, 0x22, sizeof(dest));
  memset(src, 0x0F, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 0u, 0u, src, 8u, 2u, libMcu::bitmap::bitblitOperation::OP_NOT);
  minUnitCheck(dest[0] == 0xF0);
  minUnitCheck(dest[1] == 0x22);
  minUnitCheck(dest[2] == 0xF0);
  minUnitCheck(dest[3] == 0x22);
  minUnitCheck(dest[4] == 0x22);
  minUnitCheck(dest[5] == 0x22);
  minUnitCheck(dest[6] == 0x22);
  minUnitCheck(dest[7] == 0x22);
  // operation not
  memset(dest, 0x22, sizeof(dest));
  memset(src, 0x0F, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 0u, 0u, src, 8u, 2u, libMcu::bitmap::bitblitOperation::OP_OR);
  minUnitCheck(dest[0] == 0x2F);
  minUnitCheck(dest[1] == 0x22);
  minUnitCheck(dest[2] == 0x2F);
  minUnitCheck(dest[3] == 0x22);
  minUnitCheck(dest[4] == 0x22);
  minUnitCheck(dest[5] == 0x22);
  minUnitCheck(dest[6] == 0x22);
  minUnitCheck(dest[7] == 0x22);
  // operation not
  memset(dest, 0x22, sizeof(dest));
  memset(src, 0x0F, sizeof(src));
  libMcu::bitmap::bitblit2dsmall(dest, 16u, 4u, 0u, 0u, src, 8u, 2u, libMcu::bitmap::bitblitOperation::OP_XOR);
  minUnitCheck(dest[0] == 0x2D);
  minUnitCheck(dest[1] == 0x22);
  minUnitCheck(dest[2] == 0x2D);
  minUnitCheck(dest[3] == 0x22);
  minUnitCheck(dest[4] == 0x22);
  minUnitCheck(dest[5] == 0x22);
  minUnitCheck(dest[6] == 0x22);
  minUnitCheck(dest[7] == 0x22);
}