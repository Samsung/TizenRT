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
// UNSUPPORTED: c++03, c++11, c++14

// <memory>

// template <class InputIt, class ForwardIt>
// ForwardIt uninitialized_move(InputIt, InputIt, ForwardIt);

#include <memory>
#include <cstdlib>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct Counted {
  static int count;
  static int constructed;
  static void reset() { count = constructed =  0; }
  explicit Counted(int&& x) : value(x) { x = 0; ++count; ++constructed; }
  Counted(Counted const&) { TC_ASSERT_EXPR(false); }
  ~Counted() { TC_ASSERT_EXPR(count > 0); --count; }
  friend void operator&(Counted) = delete;
  int value;
};
int Counted::count = 0;
int Counted::constructed = 0;

struct ThrowsCounted {
  static int count;
  static int constructed;
  static int throw_after;
  static void reset() { throw_after = count = constructed =  0; }
  explicit ThrowsCounted(int&& x) {
      ++constructed;
      if (throw_after > 0 && --throw_after == 0) {
          TEST_THROW(1);
      }
      ++count;
      x = 0;
  }
  ThrowsCounted(ThrowsCounted const&) { TC_ASSERT_EXPR(false); }
  ~ThrowsCounted() { TC_ASSERT_EXPR(count > 0); --count; }
  friend void operator&(ThrowsCounted) = delete;
};
int ThrowsCounted::count = 0;
int ThrowsCounted::constructed = 0;
int ThrowsCounted::throw_after = 0;

void test_ctor_throws()
{
#ifndef TEST_HAS_NO_EXCEPTIONS
    using It = forward_iterator<ThrowsCounted*>;
    const int N = 5;
    int values[N] = {1, 2, 3, 4, 5};
    alignas(ThrowsCounted) char pool[sizeof(ThrowsCounted)*N] = {};
    ThrowsCounted* p = (ThrowsCounted*)pool;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
        ThrowsCounted::throw_after = 4;
        std::uninitialized_move(values, values + N, It(p));
        TC_ASSERT_EXPR(false);
    } catch (...) {}
    TC_ASSERT_EXPR(ThrowsCounted::count == 0);
    TC_ASSERT_EXPR(ThrowsCounted::constructed == 4); // forth construction throws
    TC_ASSERT_EXPR(values[0] == 0);
    TC_ASSERT_EXPR(values[1] == 0);
    TC_ASSERT_EXPR(values[2] == 0);
    TC_ASSERT_EXPR(values[3] == 4);
    TC_ASSERT_EXPR(values[4] == 5);
#endif
}
#endif // _LIBCPP_NO_EXCEPTIONS

void test_counted()
{
    using It = cpp17_input_iterator<int*>;
    using FIt = forward_iterator<Counted*>;
    const int N = 5;
    int values[N] = {1, 2, 3, 4, 5};
    alignas(Counted) char pool[sizeof(Counted)*N] = {};
    Counted* p = (Counted*)pool;
    auto ret = std::uninitialized_move(It(values), It(values + 1), FIt(p));
    TC_ASSERT_EXPR(ret == FIt(p +1));
    TC_ASSERT_EXPR(Counted::constructed == 1);
    TC_ASSERT_EXPR(Counted::count == 1);
    TC_ASSERT_EXPR(p[0].value == 1);
    TC_ASSERT_EXPR(values[0] == 0);
    ret = std::uninitialized_move(It(values+1), It(values+N), FIt(p+1));
    TC_ASSERT_EXPR(ret == FIt(p + N));
    TC_ASSERT_EXPR(Counted::count == 5);
    TC_ASSERT_EXPR(Counted::constructed == 5);
    TC_ASSERT_EXPR(p[1].value == 2);
    TC_ASSERT_EXPR(p[2].value == 3);
    TC_ASSERT_EXPR(p[3].value == 4);
    TC_ASSERT_EXPR(p[4].value == 5);
    TC_ASSERT_EXPR(values[1] == 0);
    TC_ASSERT_EXPR(values[2] == 0);
    TC_ASSERT_EXPR(values[3] == 0);
    TC_ASSERT_EXPR(values[4] == 0);
    std::destroy(p, p+N);
    TC_ASSERT_EXPR(Counted::count == 0);
}

int tc_utilities_memory_specialized_algorithms_uninitialized_move_uninitialized_move(void) {
    test_counted();
    test_ctor_throws();

  return 0;
}
