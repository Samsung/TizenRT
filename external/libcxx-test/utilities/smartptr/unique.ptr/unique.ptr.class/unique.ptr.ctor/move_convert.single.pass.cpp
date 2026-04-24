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
// <memory>

// unique_ptr

// Test unique_ptr converting move ctor

// NOTE: unique_ptr does not provide converting constructors in C++03
// UNSUPPORTED: c++03

#include <memory>
#include <type_traits>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"
#include "libcxx_tc_common.h"

// test converting move ctor.  Should only require a MoveConstructible deleter, or if
//    deleter is a reference, not even that.
// Explicit version

template <class LHS, class RHS>
TEST_CONSTEXPR_CXX23 void checkReferenceDeleter(LHS& lhs, RHS& rhs) {
  typedef typename LHS::deleter_type NewDel;
  static_assert(std::is_reference<NewDel>::value, "");
  rhs.get_deleter().set_state(42);
  TC_ASSERT_EXPR(rhs.get_deleter().state() == 42);
  TC_ASSERT_EXPR(lhs.get_deleter().state() == 42);
  lhs.get_deleter().set_state(99);
  TC_ASSERT_EXPR(lhs.get_deleter().state() == 99);
  TC_ASSERT_EXPR(rhs.get_deleter().state() == 99);
}

template <class LHS, class RHS>
TEST_CONSTEXPR_CXX23 void checkDeleter(LHS& lhs, RHS& rhs, int LHSVal, int RHSVal) {
  TC_ASSERT_EXPR(lhs.get_deleter().state() == LHSVal);
  TC_ASSERT_EXPR(rhs.get_deleter().state() == RHSVal);
}

template <class LHS, class RHS>
TEST_CONSTEXPR_CXX23 void checkCtor(LHS& lhs, RHS& rhs, A* RHSVal) {
  TC_ASSERT_EXPR(lhs.get() == RHSVal);
  TC_ASSERT_EXPR(rhs.get() == nullptr);
  if (!TEST_IS_CONSTANT_EVALUATED) {
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(B::count == 1);
  }
}

TEST_CONSTEXPR_CXX23 void checkNoneAlive() {
  if (!TEST_IS_CONSTANT_EVALUATED) {
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(B::count == 0);
  }
}

template <class T>
struct NCConvertingDeleter {
  TEST_CONSTEXPR_CXX23 NCConvertingDeleter()                      = default;
  NCConvertingDeleter(NCConvertingDeleter const&) = delete;
  TEST_CONSTEXPR_CXX23 NCConvertingDeleter(NCConvertingDeleter&&) = default;

  template <class U>
  TEST_CONSTEXPR_CXX23 NCConvertingDeleter(NCConvertingDeleter<U>&&) {}

  TEST_CONSTEXPR_CXX23 void operator()(T*) const {}
};

template <class T>
struct NCConvertingDeleter<T[]> {
  TEST_CONSTEXPR_CXX23 NCConvertingDeleter()                      = default;
  NCConvertingDeleter(NCConvertingDeleter const&) = delete;
  TEST_CONSTEXPR_CXX23 NCConvertingDeleter(NCConvertingDeleter&&) = default;

  template <class U>
  TEST_CONSTEXPR_CXX23 NCConvertingDeleter(NCConvertingDeleter<U>&&) {}

  TEST_CONSTEXPR_CXX23 void operator()(T*) const {}
};

struct NCGenericDeleter {
  TEST_CONSTEXPR_CXX23 NCGenericDeleter()                   = default;
  NCGenericDeleter(NCGenericDeleter const&) = delete;
  TEST_CONSTEXPR_CXX23 NCGenericDeleter(NCGenericDeleter&&) = default;

  TEST_CONSTEXPR_CXX23 void operator()(void*) const {}
};

TEST_CONSTEXPR_CXX23 void test_sfinae() {
  using DA = NCConvertingDeleter<A>; // non-copyable deleters
  using DB = NCConvertingDeleter<B>;
  using UA = std::unique_ptr<A>;
  using UB = std::unique_ptr<B>;
  using UAD = std::unique_ptr<A, DA>;
  using UBD = std::unique_ptr<B, DB>;
  { // cannot move from an lvalue
    static_assert(std::is_constructible<UA, UB&&>::value, "");
    static_assert(!std::is_constructible<UA, UB&>::value, "");
    static_assert(!std::is_constructible<UA, const UB&>::value, "");
  }
  { // cannot move if the deleter-types cannot convert
    static_assert(std::is_constructible<UAD, UBD&&>::value, "");
    static_assert(!std::is_constructible<UAD, UB&&>::value, "");
    static_assert(!std::is_constructible<UA, UBD&&>::value, "");
  }
  { // cannot move-convert with reference deleters of different types
    using UA1 = std::unique_ptr<A, DA&>;
    using UB1 = std::unique_ptr<B, DB&>;
    static_assert(!std::is_constructible<UA1, UB1&&>::value, "");
  }
  { // cannot move-convert with reference deleters of different types
    using UA1 = std::unique_ptr<A, const DA&>;
    using UB1 = std::unique_ptr<B, const DB&>;
    static_assert(!std::is_constructible<UA1, UB1&&>::value, "");
  }
  { // cannot move-convert from unique_ptr<Array[]>
    using UA1 = std::unique_ptr<A>;
    using UA2 = std::unique_ptr<A[]>;
    using UB1 = std::unique_ptr<B[]>;
    static_assert(!std::is_constructible<UA1, UA2&&>::value, "");
    static_assert(!std::is_constructible<UA1, UB1&&>::value, "");
  }
  { // cannot move-convert from unique_ptr<Array[]>
    using UA1 = std::unique_ptr<A, NCGenericDeleter>;
    using UA2 = std::unique_ptr<A[], NCGenericDeleter>;
    using UB1 = std::unique_ptr<B[], NCGenericDeleter>;
    static_assert(!std::is_constructible<UA1, UA2&&>::value, "");
    static_assert(!std::is_constructible<UA1, UB1&&>::value, "");
  }
}

TEST_CONSTEXPR_CXX23 void test_noexcept() {
  {
    typedef std::unique_ptr<A> APtr;
    typedef std::unique_ptr<B> BPtr;
    static_assert(std::is_nothrow_constructible<APtr, BPtr>::value, "");
  }
  {
    typedef std::unique_ptr<A, Deleter<A> > APtr;
    typedef std::unique_ptr<B, Deleter<B> > BPtr;
    static_assert(std::is_nothrow_constructible<APtr, BPtr>::value, "");
  }
  {
    typedef std::unique_ptr<A, NCDeleter<A>&> APtr;
    typedef std::unique_ptr<B, NCDeleter<A>&> BPtr;
    static_assert(std::is_nothrow_constructible<APtr, BPtr>::value, "");
  }
  {
    typedef std::unique_ptr<A, const NCConstDeleter<A>&> APtr;
    typedef std::unique_ptr<B, const NCConstDeleter<A>&> BPtr;
    static_assert(std::is_nothrow_constructible<APtr, BPtr>::value, "");
  }
}

TEST_CONSTEXPR_CXX23 bool test() {
  {
    test_sfinae();
    test_noexcept();
  }
  {
    typedef std::unique_ptr<A> APtr;
    typedef std::unique_ptr<B> BPtr;
    { // explicit
      BPtr b(new B);
      A* p = b.get();
      APtr a(std::move(b));
      checkCtor(a, b, p);
    }
    checkNoneAlive();
    { // implicit
      BPtr b(new B);
      A* p = b.get();
      APtr a = std::move(b);
      checkCtor(a, b, p);
    }
    checkNoneAlive();
  }
  { // test with moveable deleters
    typedef std::unique_ptr<A, Deleter<A> > APtr;
    typedef std::unique_ptr<B, Deleter<B> > BPtr;
    {
      Deleter<B> del(5);
      BPtr b(new B, std::move(del));
      A* p = b.get();
      APtr a(std::move(b));
      checkCtor(a, b, p);
      checkDeleter(a, b, 5, 0);
    }
    checkNoneAlive();
    {
      Deleter<B> del(5);
      BPtr b(new B, std::move(del));
      A* p = b.get();
      APtr a = std::move(b);
      checkCtor(a, b, p);
      checkDeleter(a, b, 5, 0);
    }
    checkNoneAlive();
  }
  { // test with reference deleters
    typedef std::unique_ptr<A, NCDeleter<A>&> APtr;
    typedef std::unique_ptr<B, NCDeleter<A>&> BPtr;
    NCDeleter<A> del(5);
    {
      BPtr b(new B, del);
      A* p = b.get();
      APtr a(std::move(b));
      checkCtor(a, b, p);
      checkReferenceDeleter(a, b);
    }
    checkNoneAlive();
    {
      BPtr b(new B, del);
      A* p = b.get();
      APtr a = std::move(b);
      checkCtor(a, b, p);
      checkReferenceDeleter(a, b);
    }
    checkNoneAlive();
  }
  {
    typedef std::unique_ptr<A, CDeleter<A> > APtr;
    typedef std::unique_ptr<B, CDeleter<B>&> BPtr;
    CDeleter<B> del(5);
    {
      BPtr b(new B, del);
      A* p = b.get();
      APtr a(std::move(b));
      checkCtor(a, b, p);
      checkDeleter(a, b, 5, 5);
    }
    checkNoneAlive();
    {
      BPtr b(new B, del);
      A* p = b.get();
      APtr a = std::move(b);
      checkCtor(a, b, p);
      checkDeleter(a, b, 5, 5);
    }
    checkNoneAlive();
  }

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_ctor_move_convert_single(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
