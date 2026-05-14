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

// template <class ForwardIt>
// void uninitialized_default_construct(ForwardIt, ForwardIt);

#include <memory>
#include <cstdlib>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct Counted {
  static int count;
  static int constructed;
  explicit Counted() { ++count; ++constructed; }
  Counted(Counted const&) { TC_ASSERT_EXPR(false); }
  ~Counted() { --count; }
  friend void operator&(Counted) = delete;
};
int Counted::count = 0;
int Counted::constructed = 0;


struct ThrowsCounted {
  static int count;
  static int constructed;
  static int throw_after;
  explicit ThrowsCounted() {
      ++constructed;
      if (throw_after > 0 && --throw_after == 0) {
          TEST_THROW(1);
      }
      ++count;
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
    alignas(ThrowsCounted) char pool[sizeof(ThrowsCounted)*N] = {};
    ThrowsCounted* p = (ThrowsCounted*)pool;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
        ThrowsCounted::throw_after = 4;
        std::uninitialized_default_construct(It(p), It(p+N));
        TC_ASSERT_EXPR(false);
    } catch (...) {}
    TC_ASSERT_EXPR(ThrowsCounted::count == 0);
    TC_ASSERT_EXPR(ThrowsCounted::constructed == 4); // Fourth construction throws
#endif
}
#endif // _LIBCPP_NO_EXCEPTIONS

void test_counted()
{
    using It = forward_iterator<Counted*>;
    const int N = 5;
    alignas(Counted) char pool[sizeof(Counted)*N] = {};
    Counted* p = (Counted*)pool;
    std::uninitialized_default_construct(It(p), It(p+1));
    TC_ASSERT_EXPR(Counted::count == 1);
    TC_ASSERT_EXPR(Counted::constructed == 1);
    std::uninitialized_default_construct(It(p+1), It(p+N));
    TC_ASSERT_EXPR(Counted::count == 5);
    TC_ASSERT_EXPR(Counted::constructed == 5);
    std::destroy(p, p+N);
    TC_ASSERT_EXPR(Counted::count == 0);
}

int tc_utilities_memory_specialized_algorithms_uninitialized_construct_default_uninitialized_default_construct(void) {
    test_counted();
    test_ctor_throws();

  return 0;
}
