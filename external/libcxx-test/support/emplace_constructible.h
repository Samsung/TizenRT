/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef TEST_SUPPORT_EMPLACE_CONSTRUCTIBLE_H
#define TEST_SUPPORT_EMPLACE_CONSTRUCTIBLE_H

#include "test_macros.h"

#if TEST_STD_VER >= 11
template <class T>
struct EmplaceConstructible {
  T value;
  explicit EmplaceConstructible(T xvalue) : value(xvalue) {}
  EmplaceConstructible(EmplaceConstructible const&) = delete;
};

template <class T>
struct EmplaceConstructibleAndMoveInsertable {
  int copied = 0;
  T value;
  explicit EmplaceConstructibleAndMoveInsertable(T xvalue) : value(xvalue) {}

  EmplaceConstructibleAndMoveInsertable(
      EmplaceConstructibleAndMoveInsertable&& Other)
      : copied(Other.copied + 1), value(std::move(Other.value)) {}
};

template <class T>
struct EmplaceConstructibleAndMoveable {
  int copied = 0;
  int assigned = 0;
  T value;
  explicit EmplaceConstructibleAndMoveable(T xvalue) noexcept : value(xvalue) {}

  EmplaceConstructibleAndMoveable(EmplaceConstructibleAndMoveable&& Other)
      noexcept : copied(Other.copied + 1),
                 value(std::move(Other.value)) {}

  EmplaceConstructibleAndMoveable&
  operator=(EmplaceConstructibleAndMoveable&& Other) noexcept {
    copied = Other.copied;
    assigned = Other.assigned + 1;
    value = std::move(Other.value);
    return *this;
  }
};

template <class T>
struct EmplaceConstructibleMoveableAndAssignable {
  int copied = 0;
  int assigned = 0;
  T value;
  explicit EmplaceConstructibleMoveableAndAssignable(T xvalue) noexcept
      : value(xvalue) {}

  EmplaceConstructibleMoveableAndAssignable(
      EmplaceConstructibleMoveableAndAssignable&& Other) noexcept
      : copied(Other.copied + 1),
        value(std::move(Other.value)) {}

  EmplaceConstructibleMoveableAndAssignable&
  operator=(EmplaceConstructibleMoveableAndAssignable&& Other) noexcept {
    copied = Other.copied;
    assigned = Other.assigned + 1;
    value = std::move(Other.value);
    return *this;
  }

  EmplaceConstructibleMoveableAndAssignable& operator=(T xvalue) {
    value = std::move(xvalue);
    ++assigned;
    return *this;
  }
};
#endif

#endif // TEST_SUPPORT_EMPLACE_CONSTRUCTIBLE_H
