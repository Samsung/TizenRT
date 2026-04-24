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
// libc++ cannot safely provide the auto_ptr constructor without rvalue
// references.
// REQUIRES: c++11 || c++14
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

// <memory>

// unique_ptr

// template <class U> unique_ptr(auto_ptr<U>&&) noexcept

#include <memory>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A {
  static int count;
  A() { ++count; }
  A(const A&) { ++count; }
  virtual ~A() { --count; }
};

int A::count = 0;

struct B : public A {
  static int count;
  B() { ++count; }
  B(const B& b) : A(b) { ++count; }
  virtual ~B() { --count; }
};

int B::count = 0;

struct C {};

struct Deleter {
  void operator()(void*) {}
};

void test_sfinae() {
  {
    // the auto_ptr constructor should be disable with a non-default deleter.
    using AP = std::auto_ptr<int>;
    using U = std::unique_ptr<int, Deleter>;
    static_assert(!std::is_constructible<U, AP&&>::value, "");
  }
  {
    // the auto_ptr constructor should be disabled when the pointer types are incompatible.
    using AP = std::auto_ptr<A>;
    using U = std::unique_ptr<C>;
    static_assert(!std::is_constructible<U, AP&&>::value, "");
  }
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_ctor_auto_pointer(void) {
  {
    B* p = new B;
    std::auto_ptr<B> ap(p);
    std::unique_ptr<A> up(std::move(ap));
    TC_ASSERT_EXPR(up.get() == p);
    TC_ASSERT_EXPR(ap.get() == 0);
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(B::count == 1);
  }
  TC_ASSERT_EXPR(A::count == 0);
  TC_ASSERT_EXPR(B::count == 0);
  {
    B* p = new B;
    std::auto_ptr<B> ap(p);
    std::unique_ptr<A> up;
    up = std::move(ap);
    TC_ASSERT_EXPR(up.get() == p);
    TC_ASSERT_EXPR(ap.get() == 0);
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(B::count == 1);
  }
  TC_ASSERT_EXPR(A::count == 0);
  TC_ASSERT_EXPR(B::count == 0);
#if TEST_STD_VER >= 11
  {
    static_assert(std::is_nothrow_constructible<std::unique_ptr<A>,
                                                std::auto_ptr<B>&&>::value,
                  "");
  }
#endif
  test_sfinae();

  return 0;
}
