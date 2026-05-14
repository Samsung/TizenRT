//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14

// <memory>

// template <class ForwardIt>
// void uninitialized_value_construct(ForwardIt, ForwardIt);

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
  explicit Counted() { ++count; ++constructed; }
  Counted(Counted const&) { TC_ASSERT_EXPR(false); }
  ~Counted() { TC_ASSERT_EXPR(count > 0); --count; }
  friend void operator&(Counted) = delete;
};
int Counted::count = 0;
int Counted::constructed = 0;


struct ThrowsCounted {
  static int count;
  static int constructed;
  static int throw_after;
  static void reset() { throw_after = count = constructed =  0; }
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
        std::uninitialized_value_construct(It(p), It(p+N));
        TC_ASSERT_EXPR(false);
    } catch (...) {}
    TC_ASSERT_EXPR(ThrowsCounted::count == 0);
    TC_ASSERT_EXPR(ThrowsCounted::constructed == 4); // forth construction throws
#endif
}
#endif // _LIBCPP_NO_EXCEPTIONS

void test_counted()
{
    using It = forward_iterator<Counted*>;
    const int N = 5;
    alignas(Counted) char pool[sizeof(Counted)*N] = {};
    Counted* p = (Counted*)pool;
    std::uninitialized_value_construct(It(p), It(p+1));
    TC_ASSERT_EXPR(Counted::count == 1);
    TC_ASSERT_EXPR(Counted::constructed == 1);
    std::uninitialized_value_construct(It(p+1), It(p+N));
    TC_ASSERT_EXPR(Counted::count == 5);
    TC_ASSERT_EXPR(Counted::constructed == 5);
    std::destroy(p, p+N);
    TC_ASSERT_EXPR(Counted::count == 0);
}

void test_value_initialized()
{
    using It = forward_iterator<int*>;
    const int N = 5;
    int pool[N] = {-1, -1, -1, -1, -1};
    int* p = pool;
    std::uninitialized_value_construct(It(p), It(p+1));
    TC_ASSERT_EXPR(pool[0] == 0);
    TC_ASSERT_EXPR(pool[1] == -1);
    std::uninitialized_value_construct(It(p+1), It(p+N));
    TC_ASSERT_EXPR(pool[1] == 0);
    TC_ASSERT_EXPR(pool[2] == 0);
    TC_ASSERT_EXPR(pool[3] == 0);
    TC_ASSERT_EXPR(pool[4] == 0);
}

int tc_utilities_memory_specialized_algorithms_uninitialized_construct_value_uninitialized_value_construct(void) {
    test_counted();
    test_value_initialized();
    test_ctor_throws();

  return 0;
}
