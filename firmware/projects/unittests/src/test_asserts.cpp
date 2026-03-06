/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * @file test_asserts.cpp
 * @brief Test libmcu assert policies
 */
#include <cstring>
#include <minunit.h>
#include <libmcu/libmcu.hpp>

static std::uint32_t assertion_counter = 0;
static const char* assertion_cstring = nullptr;

static void assertion_function(const char* message) {
  assertion_cstring = message;
  assertion_counter++;
}

static void assertion_reset(void) {
  assertion_counter = 0;
  assertion_cstring = nullptr;
}

/** @brief Assertion function used for tests
 */
struct Assert_test_policy {
  static constexpr bool enabled = true; /*!< Enable assertions */
  /**
   * @brief Assertion function that increments a counter
   * @param message Cause of the assertion
   */
  static void fail(const char* message) noexcept {
    assertion_function(message);
  }
};

template <libmcu::Assert_concept Assertion_policy = libmcu::Assert_default>
struct Assert_test_wrapper {
  void doit()
  {
    if constexpr (Assertion_policy::enabled) {
      Assertion_policy::fail("test");
    }
  }
};

Assert_test_wrapper<Assert_test_policy> assert_test_active;
Assert_test_wrapper assert_test_default;

MINUNIT_ADD(test_asserts, NULL, NULL) {
  assertion_reset();
  MINUNIT_CHECK(assertion_counter == 0);
  MINUNIT_CHECK(assertion_cstring == nullptr);
  assert_test_default.doit();
  MINUNIT_CHECK(assertion_counter == 0);
  MINUNIT_CHECK(assertion_cstring == nullptr);
  assert_test_active.doit();
  MINUNIT_CHECK(assertion_counter == 1);
  MINUNIT_ASSERT(assertion_cstring != nullptr);
  MINUNIT_CHECK(std::strcmp(assertion_cstring, "test") == 0);
}