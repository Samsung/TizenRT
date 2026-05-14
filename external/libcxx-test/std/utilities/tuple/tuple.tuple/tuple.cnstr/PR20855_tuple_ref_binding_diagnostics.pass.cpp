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

// <tuple>

// See https://llvm.org/PR20855.

#include <functional>
#include <tuple>
#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#if TEST_HAS_BUILTIN_IDENTIFIER(__reference_binds_to_temporary)
# define ASSERT_REFERENCE_BINDS_TEMPORARY(...) static_assert(__reference_binds_to_temporary(__VA_ARGS__), "")
# define ASSERT_NOT_REFERENCE_BINDS_TEMPORARY(...) static_assert(!__reference_binds_to_temporary(__VA_ARGS__), "")
#else
# define ASSERT_REFERENCE_BINDS_TEMPORARY(...) static_assert(true, "")
# define ASSERT_NOT_REFERENCE_BINDS_TEMPORARY(...) static_assert(true, "")
#endif

template <class Tp>
struct ConvertsTo {
  using RawTp = typename std::remove_cv< typename std::remove_reference<Tp>::type>::type;

  operator Tp() const {
    return static_cast<Tp>(value);
  }

  mutable RawTp value;
};

struct Base {};
struct Derived : Base {};


static_assert(std::is_same<decltype("abc"), decltype(("abc"))>::value, "");
ASSERT_REFERENCE_BINDS_TEMPORARY(std::string const&, decltype("abc"));
ASSERT_REFERENCE_BINDS_TEMPORARY(std::string const&, decltype(("abc")));
ASSERT_REFERENCE_BINDS_TEMPORARY(std::string const&, const char*&&);

ASSERT_NOT_REFERENCE_BINDS_TEMPORARY(int&, const ConvertsTo<int&>&);
ASSERT_NOT_REFERENCE_BINDS_TEMPORARY(const int&, ConvertsTo<int&>&);
ASSERT_NOT_REFERENCE_BINDS_TEMPORARY(Base&, Derived&);


static_assert(std::is_constructible<int&, std::reference_wrapper<int>>::value, "");
static_assert(std::is_constructible<int const&, std::reference_wrapper<int>>::value, "");

template <class T> struct CannotDeduce {
 using type = T;
};

template <class ...Args>
void F(typename CannotDeduce<std::tuple<Args...>>::type const&) {}

void compile_tests() {
  {
    F<int, int const&>(std::make_tuple(42, 42));
  }
  {
    F<int, int const&>(std::make_tuple<const int&, const int&>(42, 42));
    std::tuple<int, int const&> t(std::make_tuple<const int&, const int&>(42, 42));
  }
  {
    auto fn = &F<int, std::string const&>;
    fn(std::tuple<int, std::string const&>(42, std::string("a")));
    fn(std::make_tuple(42, std::string("a")));
  }
  {
    Derived d;
    std::tuple<Base&, Base const&> t(d, d);
  }
  {
    ConvertsTo<int&> ct;
    std::tuple<int, int&> t(42, ct);
  }
}

void allocator_tests() {
    std::allocator<int> alloc;
    int x = 42;
    {
        std::tuple<int&> t(std::ref(x));
        TC_ASSERT_EXPR(&std::get<0>(t) == &x);
        std::tuple<int&> t1(std::allocator_arg, alloc, std::ref(x));
        TC_ASSERT_EXPR(&std::get<0>(t1) == &x);
    }
    {
        auto r = std::ref(x);
        auto const& cr = r;
        std::tuple<int&> t(r);
        TC_ASSERT_EXPR(&std::get<0>(t) == &x);
        std::tuple<int&> t1(cr);
        TC_ASSERT_EXPR(&std::get<0>(t1) == &x);
        std::tuple<int&> t2(std::allocator_arg, alloc, r);
        TC_ASSERT_EXPR(&std::get<0>(t2) == &x);
        std::tuple<int&> t3(std::allocator_arg, alloc, cr);
        TC_ASSERT_EXPR(&std::get<0>(t3) == &x);
    }
    {
        std::tuple<int const&> t(std::ref(x));
        TC_ASSERT_EXPR(&std::get<0>(t) == &x);
        std::tuple<int const&> t2(std::cref(x));
        TC_ASSERT_EXPR(&std::get<0>(t2) == &x);
        std::tuple<int const&> t3(std::allocator_arg, alloc, std::ref(x));
        TC_ASSERT_EXPR(&std::get<0>(t3) == &x);
        std::tuple<int const&> t4(std::allocator_arg, alloc, std::cref(x));
        TC_ASSERT_EXPR(&std::get<0>(t4) == &x);
    }
    {
        auto r = std::ref(x);
        auto cr = std::cref(x);
        std::tuple<int const&> t(r);
        TC_ASSERT_EXPR(&std::get<0>(t) == &x);
        std::tuple<int const&> t2(cr);
        TC_ASSERT_EXPR(&std::get<0>(t2) == &x);
        std::tuple<int const&> t3(std::allocator_arg, alloc, r);
        TC_ASSERT_EXPR(&std::get<0>(t3) == &x);
        std::tuple<int const&> t4(std::allocator_arg, alloc, cr);
        TC_ASSERT_EXPR(&std::get<0>(t4) == &x);
    }
}


int tc_utilities_tuple_tuple_tuple_tuple_cnstr_PR20855_tuple_ref_binding_diagnostics(void) {
  compile_tests();
  allocator_tests();

  return 0;
}
