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

// <memory>

// unique_ptr

// Test unique_ptr converting move assignment

#include <memory>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"
#include "libcxx_tc_common.h"

template <class APtr, class BPtr>
void testAssign(APtr& aptr, BPtr& bptr) {
  A* p = bptr.get();
  TC_ASSERT_EXPR(A::count == 2);
  aptr = std::move(bptr);
  TC_ASSERT_EXPR(aptr.get() == p);
  TC_ASSERT_EXPR(bptr.get() == 0);
  TC_ASSERT_EXPR(A::count == 1);
  TC_ASSERT_EXPR(B::count == 1);
}

template <class LHS, class RHS>
void checkDeleter(LHS& lhs, RHS& rhs, int LHSState, int RHSState) {
  TC_ASSERT_EXPR(lhs.get_deleter().state() == LHSState);
  TC_ASSERT_EXPR(rhs.get_deleter().state() == RHSState);
}

template <class T>
struct NCConvertingDeleter {
  NCConvertingDeleter() = default;
  NCConvertingDeleter(NCConvertingDeleter const&) = delete;
  NCConvertingDeleter(NCConvertingDeleter&&) = default;

  template <class U>
  NCConvertingDeleter(NCConvertingDeleter<U>&&) {}

  void operator()(T*) const {}
};

template <class T>
struct NCConvertingDeleter<T[]> {
  NCConvertingDeleter() = default;
  NCConvertingDeleter(NCConvertingDeleter const&) = delete;
  NCConvertingDeleter(NCConvertingDeleter&&) = default;

  template <class U>
  NCConvertingDeleter(NCConvertingDeleter<U>&&) {}

  void operator()(T*) const {}
};

struct GenericDeleter {
  void operator()(void*) const;
};

struct NCGenericDeleter {
  NCGenericDeleter() = default;
  NCGenericDeleter(NCGenericDeleter const&) = delete;
  NCGenericDeleter(NCGenericDeleter&&) = default;

  void operator()(void*) const {}
};

void test_sfinae() {
  using DA = NCConvertingDeleter<A[]>;        // non-copyable deleters
  using DAC = NCConvertingDeleter<const A[]>; // non-copyable deleters

  using UA = std::unique_ptr<A[]>;
  using UAC = std::unique_ptr<const A[]>;
  using UAD = std::unique_ptr<A[], DA>;
  using UACD = std::unique_ptr<const A[], DAC>;

  { // cannot move from an lvalue
    static_assert(std::is_assignable<UAC, UA&&>::value, "");
    static_assert(!std::is_assignable<UAC, UA&>::value, "");
    static_assert(!std::is_assignable<UAC, const UA&>::value, "");
  }
  { // cannot move if the deleter-types cannot convert
    static_assert(std::is_assignable<UACD, UAD&&>::value, "");
    static_assert(!std::is_assignable<UACD, UAC&&>::value, "");
    static_assert(!std::is_assignable<UAC, UACD&&>::value, "");
  }
  { // cannot move-convert with reference deleters of different types
    using UA1 = std::unique_ptr<A[], DA&>;
    using UA2 = std::unique_ptr<A[], DAC&>;
    static_assert(!std::is_assignable<UA1, UA2&&>::value, "");
  }
  { // cannot move-convert with reference deleters of different types
    using UA1 = std::unique_ptr<A[], const DA&>;
    using UA2 = std::unique_ptr<A[], const DAC&>;
    static_assert(!std::is_assignable<UA1, UA2&&>::value, "");
  }
  { // cannot move-convert from unique_ptr<Single>
    using UA1 = std::unique_ptr<A[]>;
    using UA2 = std::unique_ptr<A>;
    static_assert(!std::is_assignable<UA1, UA2&&>::value, "");
  }
  { // cannot move-convert from unique_ptr<Array[]>
    using UA1 = std::unique_ptr<A[], NCGenericDeleter>;
    using UA2 = std::unique_ptr<A, NCGenericDeleter>;
    static_assert(!std::is_assignable<UA1, UA2&&>::value, "");
  }
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_asgn_move_convert_runtime(void) {
  test_sfinae();
  // FIXME: add tests

  return 0;
}
