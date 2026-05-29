/**
 *SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 *
 * @file test_mregel.cpp
 * @brief Tests for the Mregel minimal string class
 */
#include <cstdint>
#include <cstring>
#include <array>
#include <span>
#include "minunit.h"
#include "libmcu/data_struct/mregel.hpp"

std::array<char, 12> hello_world = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
std::array<char, 15> test_array;

namespace {

MINUNIT_ADD(mregel_test_size, nullptr, nullptr) {
  const libmcu::Mregel mregel_empty;
  MINUNIT_CHECK(mregel_empty.size() == 0);
  MINUNIT_CHECK(mregel_empty.data() == nullptr);
  libmcu::Mregel mregel_non_empty{test_array};
  MINUNIT_CHECK(mregel_non_empty.size() == 0);
  MINUNIT_ASSERT(mregel_non_empty.data() == test_array.data());
  MINUNIT_CHECK(mregel_non_empty.data_size() == 14);
  mregel_non_empty.add('H');
  MINUNIT_CHECK(mregel_non_empty.size() == 1);
  mregel_non_empty.reset();
  MINUNIT_CHECK(mregel_non_empty.size() == 0);
}

MINUNIT_ADD(mregel_test_add_cstring, nullptr, nullptr) {
  libmcu::Mregel mregel_non_empty{test_array};
  mregel_non_empty.add("");
  MINUNIT_CHECK(mregel_non_empty.size() == 0);
  mregel_non_empty.add("Hello");
  MINUNIT_CHECK(mregel_non_empty.size() == 5);
  mregel_non_empty.add(" World");
  MINUNIT_CHECK(mregel_non_empty.size() == 11);
  mregel_non_empty.add("!");
  MINUNIT_CHECK(mregel_non_empty.size() == 12);
  MINUNIT_CHECK(strcmp(mregel_non_empty.c_str(), "Hello World!") == 0);
  const std::span mregel_span = mregel_non_empty.span();
  MINUNIT_CHECK(mregel_span.size() == 12);
  MINUNIT_CHECK(memcmp(mregel_span.data(), hello_world.data(), 12) == 0);
}

MINUNIT_ADD(mregel_test_add_char, nullptr, nullptr) {
  libmcu::Mregel mregel_non_empty{test_array};
  mregel_non_empty.add('H');
  MINUNIT_CHECK(mregel_non_empty.size() == 1);
  mregel_non_empty.add('e');
  MINUNIT_CHECK(mregel_non_empty.size() == 2);
  mregel_non_empty.add('l');
  MINUNIT_CHECK(mregel_non_empty.size() == 3);
  mregel_non_empty.add('l');
  MINUNIT_CHECK(mregel_non_empty.size() == 4);
  mregel_non_empty.add('o');
  MINUNIT_CHECK(mregel_non_empty.size() == 5);
  mregel_non_empty.add(' ');
  MINUNIT_CHECK(mregel_non_empty.size() == 6);
  mregel_non_empty.add('W');
  MINUNIT_CHECK(mregel_non_empty.size() == 7);
  mregel_non_empty.add('o');
  MINUNIT_CHECK(mregel_non_empty.size() == 8);
  mregel_non_empty.add('r');
  MINUNIT_CHECK(mregel_non_empty.size() == 9);
  mregel_non_empty.add('l');
  MINUNIT_CHECK(mregel_non_empty.size() == 10);
  mregel_non_empty.add('d');
  MINUNIT_CHECK(mregel_non_empty.size() == 11);
  mregel_non_empty.add('!');
  MINUNIT_CHECK(mregel_non_empty.size() == 12);
  const char *ptr = mregel_non_empty.c_str();
  MINUNIT_CHECK(strncmp(ptr, "Hello World!", mregel_non_empty.size()) == 0);
}

MINUNIT_ADD(mregel_test_add_u32, nullptr, nullptr) {
  libmcu::Mregel mregel_non_empty{test_array};
  mregel_non_empty.add(12345U);
  MINUNIT_CHECK(mregel_non_empty.size() == 5);
  MINUNIT_CHECK(strncmp(mregel_non_empty.c_str(), "12345", mregel_non_empty.size()) == 0);
}

MINUNIT_ADD(mregel_test_add_decimal, nullptr, nullptr) {
  libmcu::Mregel mregel_non_empty{test_array};
  mregel_non_empty.add(libmcu::Dec{-456789});
  MINUNIT_CHECK(mregel_non_empty.size() == 7);
  MINUNIT_CHECK(strncmp(mregel_non_empty.c_str(), "-456789", mregel_non_empty.size()) == 0);
}

MINUNIT_ADD(mregel_test_add_decimal_zero, nullptr, nullptr) {
  libmcu::Mregel mregel_non_empty{test_array};
  mregel_non_empty.add(libmcu::Dec{0});
  MINUNIT_CHECK(mregel_non_empty.size() == 1);
  MINUNIT_CHECK(strncmp(mregel_non_empty.c_str(), "0", mregel_non_empty.size()) == 0);
}

MINUNIT_ADD(mregel_test_add_hex, nullptr, nullptr) {
  libmcu::Mregel mregel_non_empty{test_array};
  mregel_non_empty.add(libmcu::Hex{0x1A3B5C7F});
  MINUNIT_CHECK(mregel_non_empty.size() == 8);
  MINUNIT_CHECK(strncmp(mregel_non_empty.c_str(), "1A3B5C7F", mregel_non_empty.size()) == 0);
}

MINUNIT_ADD(mregel_test_variadic_add, nullptr, nullptr) {
  libmcu::Mregel mregel_non_empty{test_array};
  mregel_non_empty.add("Hello", ' ', libmcu::Dec{-42}, libmcu::Hex{0x42}, '\n');
  MINUNIT_CHECK(mregel_non_empty.size() == 12);
  MINUNIT_CHECK(strncmp(mregel_non_empty.c_str(), "Hello -4242\n", mregel_non_empty.size()) == 0);
}

}  // namespace