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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// "support/test_convertible.hpp"

#include "test_convertible.hpp"

struct ImplicitDefault {
  ImplicitDefault() {}
};
static_assert(test_convertible<ImplicitDefault>(), "Must be convertible");

struct ExplicitDefault {
  explicit ExplicitDefault() {}
};
static_assert(!test_convertible<ExplicitDefault>(), "Must not be convertible");

struct ImplicitInt {
  ImplicitInt(int) {}
};
static_assert(test_convertible<ImplicitInt, int>(), "Must be convertible");

struct ExplicitInt {
  explicit ExplicitInt(int) {}
};
static_assert(!test_convertible<ExplicitInt, int>(), "Must not be convertible");

struct ImplicitCopy {
  ImplicitCopy(ImplicitCopy const&) {}
};
static_assert(test_convertible<ImplicitCopy, ImplicitCopy>(), "Must be convertible");

struct ExplicitCopy {
  explicit ExplicitCopy(ExplicitCopy const&) {}
};
static_assert(!test_convertible<ExplicitCopy, ExplicitCopy>(), "Must not be convertible");

struct ImplicitMove {
  ImplicitMove(ImplicitMove&&) {}
};
static_assert(test_convertible<ImplicitMove, ImplicitMove>(), "Must be convertible");

struct ExplicitMove {
  explicit ExplicitMove(ExplicitMove&&) {}
};
static_assert(!test_convertible<ExplicitMove, ExplicitMove>(), "Must not be convertible");

struct ImplicitArgs {
  ImplicitArgs(int, int, int) {}
};
static_assert(test_convertible<ImplicitArgs, int, int, int>(), "Must be convertible");

struct ExplicitArgs {
  explicit ExplicitArgs(int, int, int) {}
};
static_assert(!test_convertible<ExplicitArgs, int, int, int>(), "Must not be convertible");

int main() {
    // Nothing to do
}
