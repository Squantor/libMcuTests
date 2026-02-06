/*
SPDX-License-Identifier: MIT

Copyright (c) 2026 Bart Bilos
For conditions of distribution and use, see LICENSE file
*/
/**
 * \file test_blit.cpp
 * \brief Tests for blitting functions
 */
#include <array>
#include <libmcu/bitmap/blit.hpp>
#include <minunit.h>

std::array<std::uint32_t, 144> test_bitmap_data_dest;
std::array<std::uint32_t, 16> test_bitmap_data_src;

MINUNIT_SETUP(test_bitblit_setup) {
  MINUNIT_PASS();
}

MINUNIT_TEARDOWN(test_bitblit_teardown) {
  MINUNIT_PASS();
}

MINUNIT_ADD(test_bitblit_1bbp_bitmaps, test_bitblit_setup, test_bitblit_teardown) {
  Bitmap_view src_bitmap(test_bitmap_data_src.data(), 8, 8, 1);
  Bitmap_view dest_bitmap(test_bitmap_data_dest.data(), 24, 24, 1);
  dest_bitmap.fill(0);
  src_bitmap.fill(1);
  blit(src_bitmap.as_const(), dest_bitmap, 0, 0);
  MINUNIT_CHECK(test_bitmap_data_dest[0] == 0xFF'00'00'FF);
  MINUNIT_CHECK(test_bitmap_data_dest[1] == 0x00'FF'00'00);
  MINUNIT_CHECK(test_bitmap_data_dest[2] == 0x00'00'FF'00);
  MINUNIT_CHECK(test_bitmap_data_dest[3] == 0xFF'00'00'FF);
  MINUNIT_CHECK(test_bitmap_data_dest[4] == 0x00'FF'00'00);
  MINUNIT_CHECK(test_bitmap_data_dest[5] == 0x00'00'FF'00);
  MINUNIT_CHECK(test_bitmap_data_dest[6] == 0x00'00'00'00);
  dest_bitmap.fill(0);
  blit(src_bitmap.as_const(), dest_bitmap, 8, 8);
  MINUNIT_CHECK(test_bitmap_data_dest[5] == 0x00'00'00'00);
  MINUNIT_CHECK(test_bitmap_data_dest[6] == 0x00'00'FF'00);
  MINUNIT_CHECK(test_bitmap_data_dest[7] == 0xFF'00'00'FF);
  MINUNIT_CHECK(test_bitmap_data_dest[8] == 0x00'FF'00'00);
  MINUNIT_CHECK(test_bitmap_data_dest[9] == 0x00'00'FF'00);
  MINUNIT_CHECK(test_bitmap_data_dest[10] == 0xFF'00'00'FF);
  MINUNIT_CHECK(test_bitmap_data_dest[11] == 0x00'FF'00'00);
  MINUNIT_CHECK(test_bitmap_data_dest[12] == 0x00'00'00'00);
  dest_bitmap.fill(0);
  blit(src_bitmap.as_const(), dest_bitmap, 3, 3);
  MINUNIT_CHECK(test_bitmap_data_dest[1] == 0x00'00'00'00);
  MINUNIT_CHECK(test_bitmap_data_dest[2] == 0x00'07'F8'00);
  MINUNIT_CHECK(test_bitmap_data_dest[3] == 0xF8'00'07'F8);
  MINUNIT_CHECK(test_bitmap_data_dest[4] == 0x07'F8'00'07);
  MINUNIT_CHECK(test_bitmap_data_dest[5] == 0x00'07'F8'00);
  MINUNIT_CHECK(test_bitmap_data_dest[6] == 0xF8'00'07'F8);
  MINUNIT_CHECK(test_bitmap_data_dest[7] == 0x07'F8'00'07);
  MINUNIT_CHECK(test_bitmap_data_dest[8] == 0x00'00'00'00);
  dest_bitmap.fill(0);
  MINUNIT_PASS();
}

MINUNIT_ADD(test_bitblit_8bbp_bitmaps, test_bitblit_setup, test_bitblit_teardown) {
  MINUNIT_PASS();
}
