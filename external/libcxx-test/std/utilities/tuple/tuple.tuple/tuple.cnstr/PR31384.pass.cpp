//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// See https://llvm.org/PR31384.

#include <tuple>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int count = 0;

struct Explicit {
  Explicit() = default;
  explicit Explicit(int) {}
};

struct Implicit {
  Implicit() = default;
  Implicit(int) {}
};

template<class T>
struct Derived : std::tuple<T> {
  using std::tuple<T>::tuple;
  template<class U>
  operator std::tuple<U>() && { ++count; return {}; }
};


template<class T>
struct ExplicitDerived : std::tuple<T> {
  using std::tuple<T>::tuple;
  template<class U>
  explicit operator std::tuple<U>() && { ++count; return {}; }
};

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_PR31384(void) {
  {
    std::tuple<Explicit> foo = Derived<int>{42}; ((void)foo);
    TC_ASSERT_EXPR(count == 1);
    Derived<int> d{42};
    std::tuple<Explicit> bar(std::move(d)); ((void)bar);
#if TEST_STD_VER < 17
    TC_ASSERT_EXPR(count == 1);
#else
    TC_ASSERT_EXPR(count == 2);
#endif
  }
  count = 0;
  {
    std::tuple<Implicit> foo = Derived<int>{42}; ((void)foo);
    TC_ASSERT_EXPR(count == 1);
    Derived<int> d{42};
    std::tuple<Implicit> bar(std::move(d)); ((void)bar);
#if TEST_STD_VER < 17
    TC_ASSERT_EXPR(count == 1);
#else
    TC_ASSERT_EXPR(count == 2);
#endif
  }
  count = 0;
  {
    static_assert(!std::is_convertible<ExplicitDerived<int>, std::tuple<Explicit>>::value, "");
    ExplicitDerived<int> d{42};
    std::tuple<Explicit> bar(std::move(d)); ((void)bar);
#if TEST_STD_VER < 17
    TC_ASSERT_EXPR(count == 0);
#else
    TC_ASSERT_EXPR(count == 1);
#endif
  }
  count = 0;
  {
    std::tuple<Implicit> foo = ExplicitDerived<int>{42}; ((void)foo);
    static_assert(std::is_convertible<ExplicitDerived<int>, std::tuple<Implicit>>::value, "");
    TC_ASSERT_EXPR(count == 0);
    ExplicitDerived<int> d{42};
    std::tuple<Implicit> bar(std::move(d)); ((void)bar);
#if TEST_STD_VER < 17
    TC_ASSERT_EXPR(count == 0);
#else
    TC_ASSERT_EXPR(count == 1);
#endif
  }
  count = 0;

  return 0;
}
