/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2022 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * \file test_rmw.cpp
 *
 * Read Modify Write tests
 */

#include <minunit.h>
#include <string.h>
#include <stdint.h>
#include <libmcu/bitmap/readmodifywrite.hpp>

MINUNIT_ADD(testReadModifyWrite8bit, NULL, NULL) {
  uint8_t testSource = 0x00;
  uint8_t testDestination = 0x00;
  // test AND
  testSource = 0xAA;
  testDestination = 0xCC;
  libmcu::bitmap::ReadModifyWrite(testDestination, testSource, static_cast<uint8_t>(0x0F), 0,
                                  libmcu::bitmap::BitblitOperations::And);
  MINUNIT_CHECK(testDestination == 0xC8);
  // test MOV
  testSource = 0xF;
  testDestination = 0x00;
  libmcu::bitmap::ReadModifyWrite(testDestination, testSource, static_cast<uint8_t>(0x3C), 2,
                                  libmcu::bitmap::BitblitOperations::Move);
  MINUNIT_CHECK(testDestination == 0x3C);
  testSource = 0xF0;
  testDestination = 0x00;
  libmcu::bitmap::ReadModifyWrite(testDestination, testSource, static_cast<uint8_t>(0x3C), -2,
                                  libmcu::bitmap::BitblitOperations::Move);
  MINUNIT_CHECK(testDestination == 0x3C);
  testSource = 0x0F;
  testDestination = 0x01;
  libmcu::bitmap::ReadModifyWrite(testDestination, testSource, static_cast<uint8_t>(0x3C), 2,
                                  libmcu::bitmap::BitblitOperations::Move);
  MINUNIT_CHECK(testDestination == 0x3D);
  testSource = 0x0F;
  testDestination = 0x91;
  libmcu::bitmap::ReadModifyWrite(testDestination, testSource, static_cast<uint8_t>(0x3C), 2,
                                  libmcu::bitmap::BitblitOperations::Move);
  MINUNIT_CHECK(testDestination == 0xBD);
  testSource = 0x02;
  testDestination = 0x81;
  libmcu::bitmap::ReadModifyWrite(testDestination, testSource, static_cast<uint8_t>(0x03), 0,
                                  libmcu::bitmap::BitblitOperations::Move);
  MINUNIT_CHECK(testDestination == 0x82);
  // test NOT
  testSource = 0xF;
  testDestination = 0xFF;
  libmcu::bitmap::ReadModifyWrite(testDestination, testSource, static_cast<uint8_t>(0x3C), 2,
                                  libmcu::bitmap::BitblitOperations::Not);
  MINUNIT_CHECK(testDestination == 0xC3);
  testSource = 0xF0;
  testDestination = 0x0F;
  libmcu::bitmap::ReadModifyWrite(testDestination, testSource, static_cast<uint8_t>(0x3C), -2,
                                  libmcu::bitmap::BitblitOperations::Not);
  MINUNIT_CHECK(testDestination == 0x03);
  // test OR
  testSource = 0xAA;
  testDestination = 0xCC;
  libmcu::bitmap::ReadModifyWrite(testDestination, testSource, static_cast<uint8_t>(0xF0), 4,
                                  libmcu::bitmap::BitblitOperations::Or);
  MINUNIT_CHECK(testDestination == 0xEC);
  // test XOR
  testSource = 0xAA;
  testDestination = 0xCC;
  libmcu::bitmap::ReadModifyWrite(testDestination, testSource, static_cast<uint8_t>(0xF0), 4,
                                  libmcu::bitmap::BitblitOperations::Xor);
  MINUNIT_CHECK(testDestination == 0x6C);
}

MINUNIT_ADD(testReadModifyWrite16bit, NULL, NULL) {
  uint8_t testSource = 0x00;
  uint16_t testDestination = 0x00;
  // test MOV
  testSource = 0xFF;
  testDestination = 0x0000;
  libmcu::bitmap::ReadModifyWrite(testDestination, testSource, static_cast<uint16_t>(0x3C00), 8,
                                  libmcu::bitmap::BitblitOperations::Move);
  MINUNIT_CHECK(testDestination == 0x3C00);
  MINUNIT_PASS();
  // TODO the rest
}

MINUNIT_ADD(testReadModifyWrite32bit, NULL, NULL) {
  uint8_t testSource = 0x00;
  uint32_t testDestination = 0x00000000;
  // test MOV
  testSource = 0xFF;
  testDestination = 0x00000000;
  libmcu::bitmap::ReadModifyWrite(testDestination, testSource, static_cast<uint32_t>(0x0003C000), 12,
                                  libmcu::bitmap::BitblitOperations::Move);
  MINUNIT_CHECK(testDestination == 0x0003C000);
  // TODO the rest
}