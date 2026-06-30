//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>
// UNSUPPORTED: c++03, c++11, c++14

// template<class T> class shared_ptr

// shared_ptr(weak_ptr<T>) -> shared_ptr<T>
// shared_ptr(unique_ptr<T>) -> shared_ptr<T>

#include <memory>
#include <cassert>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A {};

struct D {
  void operator()(A const* ptr) const
  {
    delete ptr;
  }
};

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_deduction(void) {
  {
    std::shared_ptr<A> s0(new A);
    std::weak_ptr<A> w = s0;
    auto s = std::shared_ptr(w);
    ASSERT_SAME_TYPE(decltype(s), std::shared_ptr<A>);
    TC_ASSERT_EXPR(s0.use_count() == 2);
    TC_ASSERT_EXPR(s.use_count() == 2);
    TC_ASSERT_EXPR(s0.get() == s.get());
  }
  {
    std::shared_ptr<A const> s0(new A);
    std::weak_ptr<A const> w = s0;
    auto s = std::shared_ptr(w);
    ASSERT_SAME_TYPE(decltype(s), std::shared_ptr<A const>);
    TC_ASSERT_EXPR(s0.use_count() == 2);
    TC_ASSERT_EXPR(s.use_count() == 2);
    TC_ASSERT_EXPR(s0.get() == s.get());
  }

  {
    std::unique_ptr<A> u(new A);
    A* uPointee = u.get();
    std::shared_ptr s = std::move(u);
    ASSERT_SAME_TYPE(decltype(s), std::shared_ptr<A>);
    TC_ASSERT_EXPR(u == nullptr);
    TC_ASSERT_EXPR(s.get() == uPointee);
  }
  {
    std::unique_ptr<A const> u(new A);
    A const* uPointee = u.get();
    std::shared_ptr s = std::move(u);
    ASSERT_SAME_TYPE(decltype(s), std::shared_ptr<A const>);
    TC_ASSERT_EXPR(u == nullptr);
    TC_ASSERT_EXPR(s.get() == uPointee);
  }

  {
    std::unique_ptr<A, D> u(new A, D{});
    A* uPointee = u.get();
    std::shared_ptr s(std::move(u));
    ASSERT_SAME_TYPE(decltype(s), std::shared_ptr<A>);
    TC_ASSERT_EXPR(u == nullptr);
    TC_ASSERT_EXPR(s.get() == uPointee);
  }
  {
    std::unique_ptr<A const, D> u(new A, D{});
    A const* uPointee = u.get();
    std::shared_ptr s(std::move(u));
    ASSERT_SAME_TYPE(decltype(s), std::shared_ptr<A const>);
    TC_ASSERT_EXPR(u == nullptr);
    TC_ASSERT_EXPR(s.get() == uPointee);
  }

  return 0;
}
