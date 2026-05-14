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

//=============================================================================
// TESTING unique_ptr(pointer, deleter)
//
// Concerns:
//   1 unique_ptr(pointer, deleter&&) only requires a MoveConstructible deleter.
//   2 unique_ptr(pointer, deleter&) requires a CopyConstructible deleter.
//   3 unique_ptr<T, D&>(pointer, deleter) does not require a CopyConstructible deleter.
//   4 unique_ptr<T, D const&>(pointer, deleter) does not require a CopyConstructible deleter.
//   5 unique_ptr(pointer, deleter) should work for derived pointers.
//   6 unique_ptr(pointer, deleter) should work with function pointers.
//   7 unique_ptr<void> should work.

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"
#include "libcxx_tc_common.h"

bool my_free_called = false;

void my_free(void*) { my_free_called = true; }

#if TEST_STD_VER >= 11
struct DeleterBase {
  TEST_CONSTEXPR_CXX23 void operator()(void*) const {}
};
struct CopyOnlyDeleter : DeleterBase {
  TEST_CONSTEXPR_CXX23 CopyOnlyDeleter()                       = default;
  TEST_CONSTEXPR_CXX23 CopyOnlyDeleter(CopyOnlyDeleter const&) = default;
  CopyOnlyDeleter(CopyOnlyDeleter&&) = delete;
};
struct MoveOnlyDeleter : DeleterBase {
  TEST_CONSTEXPR_CXX23 MoveOnlyDeleter()                  = default;
  TEST_CONSTEXPR_CXX23 MoveOnlyDeleter(MoveOnlyDeleter&&) = default;
};
struct NoCopyMoveDeleter : DeleterBase {
  TEST_CONSTEXPR_CXX23 NoCopyMoveDeleter()    = default;
  NoCopyMoveDeleter(NoCopyMoveDeleter const&) = delete;
};
#endif

template <bool IsArray>
TEST_CONSTEXPR_CXX23 void test_sfinae() {
#if TEST_STD_VER >= 11
  typedef typename std::conditional<!IsArray, int, int[]>::type VT;
  {
    using D = CopyOnlyDeleter;
    using U = std::unique_ptr<VT, D>;
    static_assert(std::is_constructible<U, int*, D const&>::value, "");
    static_assert(std::is_constructible<U, int*, D&>::value, "");
    static_assert(std::is_constructible<U, int*, D&&>::value, "");
    // FIXME: __libcpp_compressed_pair attempts to perform a move even though
    // it should only copy.
    //D d;
    //U u(nullptr, std::move(d));
  }
  {
    using D = MoveOnlyDeleter;
    using U = std::unique_ptr<VT, D>;
    static_assert(!std::is_constructible<U, int*, D const&>::value, "");
    static_assert(!std::is_constructible<U, int*, D&>::value, "");
    static_assert(std::is_constructible<U, int*, D&&>::value, "");
    D d;
    U u(nullptr, std::move(d));
  }
  {
    using D = NoCopyMoveDeleter;
    using U = std::unique_ptr<VT, D>;
    static_assert(!std::is_constructible<U, int*, D const&>::value, "");
    static_assert(!std::is_constructible<U, int*, D&>::value, "");
    static_assert(!std::is_constructible<U, int*, D&&>::value, "");
  }
  {
    using D = NoCopyMoveDeleter;
    using U = std::unique_ptr<VT, D&>;
    static_assert(!std::is_constructible<U, int*, D const&>::value, "");
    static_assert(std::is_constructible<U, int*, D&>::value, "");
    static_assert(!std::is_constructible<U, int*, D&&>::value, "");
    static_assert(!std::is_constructible<U, int*, const D&&>::value, "");
  }
  {
    using D = NoCopyMoveDeleter;
    using U = std::unique_ptr<VT, const D&>;
    static_assert(std::is_constructible<U, int*, D const&>::value, "");
    static_assert(std::is_constructible<U, int*, D&>::value, "");
    static_assert(!std::is_constructible<U, int*, D&&>::value, "");
    static_assert(!std::is_constructible<U, int*, const D&&>::value, "");
  }
#endif
}

template <bool IsArray>
TEST_CONSTEXPR_CXX23 void test_noexcept() {
#if TEST_STD_VER >= 11
  typedef typename std::conditional<!IsArray, int, int[]>::type VT;
  {
    using D = CopyOnlyDeleter;
    using U = std::unique_ptr<VT, D>;
    static_assert(std::is_nothrow_constructible<U, int*, D const&>::value, "");
    static_assert(std::is_nothrow_constructible<U, int*, D&>::value, "");
    static_assert(std::is_nothrow_constructible<U, int*, D&&>::value, "");
  }
  {
    using D = MoveOnlyDeleter;
    using U = std::unique_ptr<VT, D>;
    static_assert(std::is_nothrow_constructible<U, int*, D&&>::value, "");
    D d;
    U u(nullptr, std::move(d));
  }
  {
    using D = NoCopyMoveDeleter;
    using U = std::unique_ptr<VT, D&>;
    static_assert(std::is_nothrow_constructible<U, int*, D&>::value, "");
  }
  {
    using D = NoCopyMoveDeleter;
    using U = std::unique_ptr<VT, const D&>;
    static_assert(std::is_nothrow_constructible<U, int*, D const&>::value, "");
    static_assert(std::is_nothrow_constructible<U, int*, D&>::value, "");
  }
#endif
}

TEST_CONSTEXPR_CXX23 void test_sfinae_runtime() {
#if TEST_STD_VER >= 11
  {
    using D = CopyOnlyDeleter;
    using U = std::unique_ptr<A[], D>;
    static_assert(std::is_nothrow_constructible<U, A*, D const&>::value, "");
    static_assert(std::is_nothrow_constructible<U, A*, D&>::value, "");
    static_assert(std::is_nothrow_constructible<U, A*, D&&>::value, "");

    static_assert(!std::is_constructible<U, B*, D const&>::value, "");
    static_assert(!std::is_constructible<U, B*, D&>::value, "");
    static_assert(!std::is_constructible<U, B*, D&&>::value, "");
    // FIXME: __libcpp_compressed_pair attempts to perform a move even though
    // it should only copy.
    //D d;
    //U u(nullptr, std::move(d));
  }
  {
    using D = MoveOnlyDeleter;
    using U = std::unique_ptr<A[], D>;
    static_assert(!std::is_constructible<U, A*, D const&>::value, "");
    static_assert(!std::is_constructible<U, A*, D&>::value, "");
    static_assert(std::is_nothrow_constructible<U, A*, D&&>::value, "");

    static_assert(!std::is_constructible<U, B*, D const&>::value, "");
    static_assert(!std::is_constructible<U, B*, D&>::value, "");
    static_assert(!std::is_constructible<U, B*, D&&>::value, "");
    D d;
    U u(nullptr, std::move(d));
  }
  {
    using D = NoCopyMoveDeleter;
    using U = std::unique_ptr<A[], D>;
    static_assert(!std::is_constructible<U, A*, D const&>::value, "");
    static_assert(!std::is_constructible<U, A*, D&>::value, "");
    static_assert(!std::is_constructible<U, A*, D&&>::value, "");

    static_assert(!std::is_constructible<U, B*, D const&>::value, "");
    static_assert(!std::is_constructible<U, B*, D&>::value, "");
    static_assert(!std::is_constructible<U, B*, D&&>::value, "");
  }
  {
    using D = NoCopyMoveDeleter;
    using U = std::unique_ptr<A[], D&>;
    static_assert(!std::is_constructible<U, A*, D const&>::value, "");
    static_assert(std::is_nothrow_constructible<U, A*, D&>::value, "");
    static_assert(!std::is_constructible<U, A*, D&&>::value, "");
    static_assert(!std::is_constructible<U, A*, const D&&>::value, "");

    static_assert(!std::is_constructible<U, B*, D const&>::value, "");
    static_assert(!std::is_constructible<U, B*, D&>::value, "");
    static_assert(!std::is_constructible<U, B*, D&&>::value, "");
    static_assert(!std::is_constructible<U, B*, const D&&>::value, "");
  }
  {
    using D = NoCopyMoveDeleter;
    using U = std::unique_ptr<A[], const D&>;
    static_assert(std::is_nothrow_constructible<U, A*, D const&>::value, "");
    static_assert(std::is_nothrow_constructible<U, A*, D&>::value, "");
    static_assert(!std::is_constructible<U, A*, D&&>::value, "");
    static_assert(!std::is_constructible<U, A*, const D&&>::value, "");

    static_assert(!std::is_constructible<U, B*, D const&>::value, "");
    static_assert(!std::is_constructible<U, B*, D&>::value, "");
    static_assert(!std::is_constructible<U, B*, D&&>::value, "");
    static_assert(!std::is_constructible<U, B*, const D&&>::value, "");
  }
#endif
}

template <bool IsArray>
TEST_CONSTEXPR_CXX23 void test_basic() {
  typedef typename std::conditional<!IsArray, A, A[]>::type VT;
  const int expect_alive = IsArray ? 5 : 1;
  { // MoveConstructible deleter (C-1)
    A* p = newValue<VT>(expect_alive);
    if (!TEST_IS_CONSTANT_EVALUATED)
      TC_ASSERT_EXPR(A::count == expect_alive);
    std::unique_ptr<VT, Deleter<VT> > s(p, Deleter<VT>(5));
    TC_ASSERT_EXPR(s.get() == p);
    TC_ASSERT_EXPR(s.get_deleter().state() == 5);
  }
  if (!TEST_IS_CONSTANT_EVALUATED)
    TC_ASSERT_EXPR(A::count == 0);
  { // CopyConstructible deleter (C-2)
    A* p = newValue<VT>(expect_alive);
    if (!TEST_IS_CONSTANT_EVALUATED)
      TC_ASSERT_EXPR(A::count == expect_alive);
    CopyDeleter<VT> d(5);
    std::unique_ptr<VT, CopyDeleter<VT> > s(p, d);
    TC_ASSERT_EXPR(s.get() == p);
    TC_ASSERT_EXPR(s.get_deleter().state() == 5);
    d.set_state(6);
    TC_ASSERT_EXPR(s.get_deleter().state() == 5);
  }
  if (!TEST_IS_CONSTANT_EVALUATED)
    TC_ASSERT_EXPR(A::count == 0);
  { // Reference deleter (C-3)
    A* p = newValue<VT>(expect_alive);
    if (!TEST_IS_CONSTANT_EVALUATED)
      TC_ASSERT_EXPR(A::count == expect_alive);
    NCDeleter<VT> d(5);
    std::unique_ptr<VT, NCDeleter<VT>&> s(p, d);
    TC_ASSERT_EXPR(s.get() == p);
    TC_ASSERT_EXPR(&s.get_deleter() == &d);
    TC_ASSERT_EXPR(s.get_deleter().state() == 5);
    d.set_state(6);
    TC_ASSERT_EXPR(s.get_deleter().state() == 6);
  }
  if (!TEST_IS_CONSTANT_EVALUATED)
    TC_ASSERT_EXPR(A::count == 0);
  { // Const Reference deleter (C-4)
    A* p = newValue<VT>(expect_alive);
    if (!TEST_IS_CONSTANT_EVALUATED)
      TC_ASSERT_EXPR(A::count == expect_alive);
    NCConstDeleter<VT> d(5);
    std::unique_ptr<VT, NCConstDeleter<VT> const&> s(p, d);
    TC_ASSERT_EXPR(s.get() == p);
    TC_ASSERT_EXPR(s.get_deleter().state() == 5);
    TC_ASSERT_EXPR(&s.get_deleter() == &d);
  }
  if (!TEST_IS_CONSTANT_EVALUATED) {
    TC_ASSERT_EXPR(A::count == 0);
    { // Void and function pointers (C-6,7)
      typedef typename std::conditional<IsArray, int[], int>::type VT2;
      my_free_called = false;
      {
        int i = 0;
        std::unique_ptr<VT2, void (*)(void*)> s(&i, my_free);
        TC_ASSERT_EXPR(s.get() == &i);
        TC_ASSERT_EXPR(s.get_deleter() == my_free);
        TC_ASSERT_EXPR(!my_free_called);
      }
      TC_ASSERT_EXPR(my_free_called);
    }
  }
}

TEST_CONSTEXPR_CXX23 void test_basic_single() {
  if (!TEST_IS_CONSTANT_EVALUATED) {
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(B::count == 0);
  }
  { // Derived pointers (C-5)
    B* p = new B;
    if (!TEST_IS_CONSTANT_EVALUATED) {
      TC_ASSERT_EXPR(A::count == 1);
      TC_ASSERT_EXPR(B::count == 1);
    }
    std::unique_ptr<A, Deleter<A> > s(p, Deleter<A>(5));
    TC_ASSERT_EXPR(s.get() == p);
    TC_ASSERT_EXPR(s.get_deleter().state() == 5);
  }
  if (!TEST_IS_CONSTANT_EVALUATED) {
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(B::count == 0);

    { // Void and function pointers (C-6,7)
      my_free_called = false;
      {
        int i = 0;
        std::unique_ptr<void, void (*)(void*)> s(&i, my_free);
        TC_ASSERT_EXPR(s.get() == &i);
        TC_ASSERT_EXPR(s.get_deleter() == my_free);
        TC_ASSERT_EXPR(!my_free_called);
      }
      TC_ASSERT_EXPR(my_free_called);
    }
  }
}

template <bool IsArray>
TEST_CONSTEXPR_CXX23 void test_nullptr() {
#if TEST_STD_VER >= 11
  typedef typename std::conditional<!IsArray, A, A[]>::type VT;
  {
    std::unique_ptr<VT, Deleter<VT> > u(nullptr, Deleter<VT>{});
    TC_ASSERT_EXPR(u.get() == nullptr);
  }
  {
    NCDeleter<VT> d;
    std::unique_ptr<VT, NCDeleter<VT>& > u(nullptr, d);
    TC_ASSERT_EXPR(u.get() == nullptr);
  }
  {
    NCConstDeleter<VT> d;
    std::unique_ptr<VT, NCConstDeleter<VT> const& > u(nullptr, d);
    TC_ASSERT_EXPR(u.get() == nullptr);
  }
#endif
}

TEST_CONSTEXPR_CXX23 bool test() {
  {
    test_basic</*IsArray*/ false>();
    test_nullptr<false>();
    test_basic_single();
    test_sfinae<false>();
    test_noexcept<false>();
  }
  {
    test_basic</*IsArray*/ true>();
    test_nullptr<true>();
    test_sfinae<true>();
    test_sfinae_runtime();
    test_noexcept<true>();
  }

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_ctor_pointer_deleter(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
