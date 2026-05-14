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
