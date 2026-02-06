/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
/**
 * \file test_ringbuf.cpp
 *
 * All tests for the ring buffer class
 */
#include <minunit.h>
#include <stdint.h>
#include <libmcu/ringbuffer.hpp>

libmcu::RingBuffer<uint16_t, 10> RingBufferDutU16;

MINUNIT_ADD(RingBufferCppIsEmpty, NULL, NULL) {
  RingBufferDutU16.Reset();
  MINUNIT_CHECK(RingBufferDutU16.IsEmpty() == true);
  MINUNIT_CHECK(RingBufferDutU16.IsFull() == false);
}

MINUNIT_ADD(RingBufferCppPushBack, NULL, NULL) {
  uint16_t number = 42;
  RingBufferDutU16.Reset();
  MINUNIT_CHECK(RingBufferDutU16.IsEmpty() == true);
  MINUNIT_CHECK(RingBufferDutU16.PushBack(number) == true);
  MINUNIT_CHECK(RingBufferDutU16.IsEmpty() == false);
}

MINUNIT_ADD(RingBufferCppPushBackIsFull, NULL, NULL) {
  uint16_t number = 42;
  RingBufferDutU16.Reset();
  MINUNIT_CHECK(RingBufferDutU16.IsEmpty() == true);
  for (uint16_t i = 0; i < 10; i++) {
    MINUNIT_CHECK(RingBufferDutU16.PushBack(i) == true);
  }
  MINUNIT_CHECK(RingBufferDutU16.PushBack(number) == false);
  MINUNIT_CHECK(RingBufferDutU16.IsFull() == true);
}

MINUNIT_ADD(RingBufferCppPushFront, NULL, NULL) {
  uint16_t number = 42;
  RingBufferDutU16.Reset();
  MINUNIT_CHECK(RingBufferDutU16.IsEmpty() == true);
  MINUNIT_CHECK(RingBufferDutU16.PushFront(number) == true);
  MINUNIT_CHECK(RingBufferDutU16.IsEmpty() == false);
}

MINUNIT_ADD(RingBufferCppPushFrontIsFull, NULL, NULL) {
  uint16_t number = 42;
  RingBufferDutU16.Reset();
  MINUNIT_CHECK(RingBufferDutU16.IsEmpty() == true);
  for (uint16_t i = 0; i < 10; i++) {
    MINUNIT_CHECK(RingBufferDutU16.PushFront(i) == true);
  }
  MINUNIT_CHECK(RingBufferDutU16.PushFront(number) == false);
  MINUNIT_CHECK(RingBufferDutU16.IsFull() == true);
}

MINUNIT_ADD(RingBufferCppPopBack, NULL, NULL) {
  uint16_t number = 42;
  uint16_t output = 0;
  RingBufferDutU16.Reset();
  MINUNIT_CHECK(RingBufferDutU16.PopBack(output) == false);
  MINUNIT_CHECK(RingBufferDutU16.PushFront(number) == true);
  MINUNIT_CHECK(RingBufferDutU16.PopBack(output) == true);
  MINUNIT_CHECK(output == 42);
  MINUNIT_CHECK(RingBufferDutU16.PopBack(output) == false);
  for (uint16_t i = 0; i < 10; i++) {
    RingBufferDutU16.PushFront(i);
  }
  for (uint16_t i = 0; i < 10; i++) {
    MINUNIT_CHECK(RingBufferDutU16.PopBack(output) == true);
    MINUNIT_CHECK(output == i);
  }
  MINUNIT_CHECK(RingBufferDutU16.PopBack(output) == false);
  for (uint16_t i = 0; i < 10; i++) {
    RingBufferDutU16.PushBack(i);
  }
  for (uint16_t i = 0; i < 10; i++) {
    MINUNIT_CHECK(RingBufferDutU16.PopBack(output) == true);
    MINUNIT_CHECK(output == 9 - i);
  }
  MINUNIT_CHECK(RingBufferDutU16.PopBack(output) == false);
}

MINUNIT_ADD(RingBufferCppPopFront, NULL, NULL) {
  uint16_t number = 42;
  uint16_t output = 0;
  MINUNIT_CHECK(RingBufferDutU16.PopFront(output) == false);
  MINUNIT_CHECK(RingBufferDutU16.PushFront(number) == true);
  MINUNIT_CHECK(RingBufferDutU16.PopFront(output) == true);
  MINUNIT_CHECK(output == 42);
  MINUNIT_CHECK(RingBufferDutU16.PopFront(output) == false);
  for (uint16_t i = 0; i < 10; i++) {
    RingBufferDutU16.PushFront(i);
  }
  for (int i = 0; i < 10; i++) {
    MINUNIT_CHECK(RingBufferDutU16.PopFront(output) == true);
    MINUNIT_CHECK(output == 9 - i);
  }
  MINUNIT_CHECK(RingBufferDutU16.PopFront(output) == false);
  for (uint16_t i = 0; i < 10; i++) {
    RingBufferDutU16.PushBack(i);
  }
  for (uint16_t i = 0; i < 10; i++) {
    MINUNIT_CHECK(RingBufferDutU16.PopFront(output) == true);
    MINUNIT_CHECK(output == i);
  }
  MINUNIT_CHECK(RingBufferDutU16.PopFront(output) == false);
}