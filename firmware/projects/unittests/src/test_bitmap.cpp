/*
SPDX-License-Identifier: MIT

Copyright (c) 2026 Bart Bilos
For conditions of distribution and use, see LICENSE file
*/
/**
 * \file test_bitmap.cpp
 * \brief Tests for bitmap class
 */

#include <array>
#include <libmcu/bitmap/bitmap_view.hpp>
#include <minunit.h>

std::array<std::uint8_t, 32> test_bitmap_data_bytes;
std::array<std::uint32_t, 32> test_bitmap_data_dwords;

MINUNIT_SETUP(test_bitmap_setup) {
  test_bitmap_data_bytes.fill(0);
  test_bitmap_data_dwords.fill(0);
  MINUNIT_PASS();
}

MINUNIT_TEARDOWN(test_bitmap_teardown) {
  MINUNIT_PASS();
}

MINUNIT_ADD(test_bitmap_geometry, test_bitmap_setup, test_bitmap_teardown) {
  libmcu::bitmap::Bitmap_view test_bitmap(test_bitmap_data_bytes.data(), 16, 16, 1);
  MINUNIT_CHECK(test_bitmap.get_width() == 16);
  MINUNIT_CHECK(test_bitmap.get_height() == 16);
  MINUNIT_CHECK(test_bitmap.get_bits_per_pixel() == 1);
  MINUNIT_CHECK(test_bitmap_data_bytes.data() == test_bitmap.data());
}

MINUNIT_ADD(test_bitmap_operations, test_bitmap_setup, test_bitmap_teardown) {
  libmcu::bitmap::Bitmap_view test_bitmap(test_bitmap_data_bytes.data(), 8, 8, 2);
  test_bitmap.fill(0b11);
  for (uint16_t x_coord = 0; x_coord < 8; x_coord++) {
    for (uint16_t y_coord = 0; y_coord < 8; y_coord++) {
      MINUNIT_CHECK(test_bitmap.get_pixel(x_coord, y_coord) == 0b11);
    }
  }
  test_bitmap.set_pixel(4, 4, 0b01);
  for (uint16_t x_coord = 0; x_coord < 8; x_coord++) {
    for (uint16_t y_coord = 0; y_coord < 8; y_coord++) {
      if (x_coord == 4 && y_coord == 4)
        MINUNIT_CHECK(test_bitmap.get_pixel(x_coord, y_coord) == 0b01);
      else
        MINUNIT_CHECK(test_bitmap.get_pixel(x_coord, y_coord) == 0b11);
    }
  }
}

MINUNIT_ADD(test_dword_bitmap_operations, test_bitmap_setup, test_bitmap_teardown) {
  libmcu::bitmap::Bitmap_view test_bitmap(test_bitmap_data_dwords.data(), 8, 8, 16);
  test_bitmap.fill(0x1234);
  for (uint16_t x_coord = 0; x_coord < 8; x_coord++) {
    for (uint16_t y_coord = 0; y_coord < 8; y_coord++) {
      MINUNIT_CHECK(test_bitmap.get_pixel(x_coord, y_coord) == 0x1234);
    }
  }
  test_bitmap.set_pixel(4, 4, 0x4321);
  for (uint16_t x_coord = 0; x_coord < 8; x_coord++) {
    for (uint16_t y_coord = 0; y_coord < 8; y_coord++) {
      if (x_coord == 4 && y_coord == 4)
        MINUNIT_CHECK(test_bitmap.get_pixel(x_coord, y_coord) == 0x4321);
      else
        MINUNIT_CHECK(test_bitmap.get_pixel(x_coord, y_coord) == 0x1234);
    }
  }
}

MINUNIT_ADD(test_bitmap_const, test_bitmap_setup, test_bitmap_teardown) {
  libmcu::bitmap::Bitmap_view test_bitmap(test_bitmap_data_bytes.data(), 8, 8, 1);
  libmcu::bitmap::Bitmap_view<const std::uint8_t> test_bitmap_const(test_bitmap);
  MINUNIT_CHECK(test_bitmap_const.get_width() == 8);
  MINUNIT_CHECK(test_bitmap_const.get_height() == 8);
  MINUNIT_CHECK(test_bitmap_const.get_bits_per_pixel() == 1);
  MINUNIT_CHECK(test_bitmap_const.data() == test_bitmap.data());
}

MINUNIT_ADD(test_bitmap_outofbounds_write, test_bitmap_setup, test_bitmap_teardown) {
  libmcu::bitmap::Bitmap_view test_bitmap(test_bitmap_data_bytes.data(), 8, 8, 1);
  test_bitmap.fill(1);
  test_bitmap.set_pixel(9, 1, 0);
  for (uint16_t x_coord = 0; x_coord < 8; x_coord++) {
    for (uint16_t y_coord = 0; y_coord < 8; y_coord++) {
      MINUNIT_CHECK(test_bitmap.get_pixel(x_coord, y_coord) == 1);
    }
  }
  MINUNIT_CHECK(test_bitmap.get_pixel(9, 1) == 0);
}