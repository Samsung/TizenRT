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
// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "demangle.h"
#include <typeinfo>
#include <cassert>

struct MyType {};

template <class T, class U> struct ArgumentListID {};

int main() {
  struct {
    const char* raw;
    const char* expect;
  } TestCases[] = {
      {typeid(int).name(), "int"},
      {typeid(MyType).name(), "MyType"},
      {typeid(ArgumentListID<int, MyType>).name(), "ArgumentListID<int, MyType>"}
  };
  const size_t size = sizeof(TestCases) / sizeof(TestCases[0]);
  for (size_t i=0; i < size; ++i) {
    const char* raw = TestCases[i].raw;
    const char* expect = TestCases[i].expect;
#ifdef TEST_HAS_NO_DEMANGLE
    assert(demangle(raw) == raw);
    ((void)expect);
#else
    assert(demangle(raw) == expect);
#endif
  }
}
