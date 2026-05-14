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

// <tuple>

// template <class F, class T> constexpr decltype(auto) apply(F &&, T &&)

// Testing extended function types. The extended function types are those
// named by INVOKE but that are not actual callable objects. These include
// bullets 1-4 of invoke.

#include <tuple>
#include <array>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int count = 0;

struct A_int_0
{
    A_int_0() : obj1(0){}
    A_int_0(int x) : obj1(x) {}
    int mem1() { return ++count; }
    int mem2() const { return ++count; }
    int const obj1;
};

struct A_int_1
{
    A_int_1() {}
    A_int_1(int) {}
    int mem1(int x) { return count += x; }
    int mem2(int x) const { return count += x; }
};

struct A_int_2
{
    A_int_2() {}
    A_int_2(int) {}
    int mem1(int x, int y) { return count += (x + y); }
    int mem2(int x, int y) const { return count += (x + y); }
};

template <class A>
struct A_wrap
{
    A_wrap() {}
    A_wrap(int x) : m_a(x) {}
    A & operator*() { return m_a; }
    A const & operator*() const { return m_a; }
    A m_a;
};

typedef A_wrap<A_int_0> A_wrap_0;
typedef A_wrap<A_int_1> A_wrap_1;
typedef A_wrap<A_int_2> A_wrap_2;


template <class A>
struct A_base : public A
{
    A_base() : A() {}
    A_base(int x) : A(x) {}
};

typedef A_base<A_int_0> A_base_0;
typedef A_base<A_int_1> A_base_1;
typedef A_base<A_int_2> A_base_2;


template <
    class Tuple, class ConstTuple
  , class TuplePtr, class ConstTuplePtr
  , class TupleWrap, class ConstTupleWrap
  , class TupleBase, class ConstTupleBase
  >
void test_ext_int_0()
{
    count = 0;
    typedef A_int_0 T;
    typedef A_wrap_0 Wrap;
    typedef A_base_0 Base;

    typedef int(T::*mem1_t)();
    mem1_t mem1 = &T::mem1;

    typedef int(T::*mem2_t)() const;
    mem2_t mem2 = &T::mem2;

    typedef int const T::*obj1_t;
    obj1_t obj1 = &T::obj1;

    // member function w/ref
    {
        T a;
        Tuple t{a};
        TC_ASSERT_EXPR(1 == std::apply(mem1, t));
        TC_ASSERT_EXPR(count == 1);
    }
    count = 0;
    // member function w/pointer
    {
        T a;
        TuplePtr t{&a};
        TC_ASSERT_EXPR(1 == std::apply(mem1, t));
        TC_ASSERT_EXPR(count == 1);
    }
    count = 0;
    // member function w/base
    {
        Base a;
        TupleBase t{a};
        TC_ASSERT_EXPR(1 == std::apply(mem1, t));
        TC_ASSERT_EXPR(count == 1);
    }
    count = 0;
    // member function w/wrap
    {
        Wrap a;
        TupleWrap t{a};
        TC_ASSERT_EXPR(1 == std::apply(mem1, t));
        TC_ASSERT_EXPR(count == 1);
    }
    count = 0;
    // const member function w/ref
    {
        T const a;
        ConstTuple t{a};
        TC_ASSERT_EXPR(1 == std::apply(mem2, t));
        TC_ASSERT_EXPR(count == 1);
    }
    count = 0;
    // const member function w/pointer
    {
        T const a;
        ConstTuplePtr t{&a};
        TC_ASSERT_EXPR(1 == std::apply(mem2, t));
        TC_ASSERT_EXPR(count == 1);
    }
    count = 0;
    // const member function w/base
    {
        Base const a;
        ConstTupleBase t{a};
        TC_ASSERT_EXPR(1 == std::apply(mem2, t));
        TC_ASSERT_EXPR(count == 1);
    }
    count = 0;
    // const member function w/wrapper
    {
        Wrap const a;
        ConstTupleWrap t{a};
        TC_ASSERT_EXPR(1 == std::apply(mem2, t));
        TC_ASSERT_EXPR(1 == count);
    }
    // member object w/ref
    {
        T a{42};
        Tuple t{a};
        TC_ASSERT_EXPR(42 == std::apply(obj1, t));
    }
    // member object w/pointer
    {
        T a{42};
        TuplePtr t{&a};
        TC_ASSERT_EXPR(42 == std::apply(obj1, t));
    }
    // member object w/base
    {
        Base a{42};
        TupleBase t{a};
        TC_ASSERT_EXPR(42 == std::apply(obj1, t));
    }
    // member object w/wrapper
    {
        Wrap a{42};
        TupleWrap t{a};
        TC_ASSERT_EXPR(42 == std::apply(obj1, t));
    }
}


template <
    class Tuple, class ConstTuple
  , class TuplePtr, class ConstTuplePtr
  , class TupleWrap, class ConstTupleWrap
  , class TupleBase, class ConstTupleBase
  >
void test_ext_int_1()
{
    count = 0;
    typedef A_int_1 T;
    typedef A_wrap_1 Wrap;
    typedef A_base_1 Base;

    typedef int(T::*mem1_t)(int);
    mem1_t mem1 = &T::mem1;

    typedef int(T::*mem2_t)(int) const;
    mem2_t mem2 = &T::mem2;

    // member function w/ref
    {
        T a;
        Tuple t{a, 2};
        TC_ASSERT_EXPR(2 == std::apply(mem1, t));
        TC_ASSERT_EXPR(count == 2);
    }
    count = 0;
    // member function w/pointer
    {
        T a;
        TuplePtr t{&a, 3};
        TC_ASSERT_EXPR(3 == std::apply(mem1, t));
        TC_ASSERT_EXPR(count == 3);
    }
    count = 0;
    // member function w/base
    {
        Base a;
        TupleBase t{a, 4};
        TC_ASSERT_EXPR(4 == std::apply(mem1, t));
        TC_ASSERT_EXPR(count == 4);
    }
    count = 0;
    // member function w/wrap
    {
        Wrap a;
        TupleWrap t{a, 5};
        TC_ASSERT_EXPR(5 == std::apply(mem1, t));
        TC_ASSERT_EXPR(count == 5);
    }
    count = 0;
    // const member function w/ref
    {
        T const a;
        ConstTuple t{a, 6};
        TC_ASSERT_EXPR(6 == std::apply(mem2, t));
        TC_ASSERT_EXPR(count == 6);
    }
    count = 0;
    // const member function w/pointer
    {
        T const a;
        ConstTuplePtr t{&a, 7};
        TC_ASSERT_EXPR(7 == std::apply(mem2, t));
        TC_ASSERT_EXPR(count == 7);
    }
    count = 0;
    // const member function w/base
    {
        Base const a;
        ConstTupleBase t{a, 8};
        TC_ASSERT_EXPR(8 == std::apply(mem2, t));
        TC_ASSERT_EXPR(count == 8);
    }
    count = 0;
    // const member function w/wrapper
    {
        Wrap const a;
        ConstTupleWrap t{a, 9};
        TC_ASSERT_EXPR(9 == std::apply(mem2, t));
        TC_ASSERT_EXPR(9 == count);
    }
}


template <
    class Tuple, class ConstTuple
  , class TuplePtr, class ConstTuplePtr
  , class TupleWrap, class ConstTupleWrap
  , class TupleBase, class ConstTupleBase
  >
void test_ext_int_2()
{
    count = 0;
    typedef A_int_2 T;
    typedef A_wrap_2 Wrap;
    typedef A_base_2 Base;

    typedef int(T::*mem1_t)(int, int);
    mem1_t mem1 = &T::mem1;

    typedef int(T::*mem2_t)(int, int) const;
    mem2_t mem2 = &T::mem2;

    // member function w/ref
    {
        T a;
        Tuple t{a, 1, 1};
        TC_ASSERT_EXPR(2 == std::apply(mem1, t));
        TC_ASSERT_EXPR(count == 2);
    }
    count = 0;
    // member function w/pointer
    {
        T a;
        TuplePtr t{&a, 1, 2};
        TC_ASSERT_EXPR(3 == std::apply(mem1, t));
        TC_ASSERT_EXPR(count == 3);
    }
    count = 0;
    // member function w/base
    {
        Base a;
        TupleBase t{a, 2, 2};
        TC_ASSERT_EXPR(4 == std::apply(mem1, t));
        TC_ASSERT_EXPR(count == 4);
    }
    count = 0;
    // member function w/wrap
    {
        Wrap a;
        TupleWrap t{a, 2, 3};
        TC_ASSERT_EXPR(5 == std::apply(mem1, t));
        TC_ASSERT_EXPR(count == 5);
    }
    count = 0;
    // const member function w/ref
    {
        T const a;
        ConstTuple t{a, 3, 3};
        TC_ASSERT_EXPR(6 == std::apply(mem2, t));
        TC_ASSERT_EXPR(count == 6);
    }
    count = 0;
    // const member function w/pointer
    {
        T const a;
        ConstTuplePtr t{&a, 3, 4};
        TC_ASSERT_EXPR(7 == std::apply(mem2, t));
        TC_ASSERT_EXPR(count == 7);
    }
    count = 0;
    // const member function w/base
    {
        Base const a;
        ConstTupleBase t{a, 4, 4};
        TC_ASSERT_EXPR(8 == std::apply(mem2, t));
        TC_ASSERT_EXPR(count == 8);
    }
    count = 0;
    // const member function w/wrapper
    {
        Wrap const a;
        ConstTupleWrap t{a, 4, 5};
        TC_ASSERT_EXPR(9 == std::apply(mem2, t));
        TC_ASSERT_EXPR(9 == count);
    }
}

int tc_utilities_tuple_tuple_tuple_tuple_apply_apply_extended_types(void) {
    {
        test_ext_int_0<
            std::tuple<A_int_0 &>, std::tuple<A_int_0 const &>
          , std::tuple<A_int_0 *>, std::tuple<A_int_0 const *>
          , std::tuple<A_wrap_0 &>, std::tuple<A_wrap_0 const &>
          , std::tuple<A_base_0 &>, std::tuple<A_base_0 const &>
          >();
        test_ext_int_0<
            std::tuple<A_int_0>, std::tuple<A_int_0 const>
          , std::tuple<A_int_0 *>, std::tuple<A_int_0 const *>
          , std::tuple<A_wrap_0>, std::tuple<A_wrap_0 const>
          , std::tuple<A_base_0>, std::tuple<A_base_0 const>
          >();
        test_ext_int_0<
            std::array<A_int_0, 1>, std::array<A_int_0 const, 1>
          , std::array<A_int_0*, 1>, std::array<A_int_0 const*, 1>
          , std::array<A_wrap_0, 1>, std::array<A_wrap_0 const, 1>
          , std::array<A_base_0, 1>, std::array<A_base_0 const, 1>
          >();
    }
    {
        test_ext_int_1<
            std::tuple<A_int_1 &, int>, std::tuple<A_int_1 const &, int>
          , std::tuple<A_int_1 *, int>, std::tuple<A_int_1 const *, int>
          , std::tuple<A_wrap_1 &, int>, std::tuple<A_wrap_1 const &, int>
          , std::tuple<A_base_1 &, int>, std::tuple<A_base_1 const &, int>
          >();
        test_ext_int_1<
            std::tuple<A_int_1, int>, std::tuple<A_int_1 const, int>
          , std::tuple<A_int_1 *, int>, std::tuple<A_int_1 const *, int>
          , std::tuple<A_wrap_1, int>, std::tuple<A_wrap_1 const, int>
          , std::tuple<A_base_1, int>, std::tuple<A_base_1 const, int>
          >();
        test_ext_int_1<
            std::pair<A_int_1 &, int>, std::pair<A_int_1 const &, int>
          , std::pair<A_int_1 *, int>, std::pair<A_int_1 const *, int>
          , std::pair<A_wrap_1 &, int>, std::pair<A_wrap_1 const &, int>
          , std::pair<A_base_1 &, int>, std::pair<A_base_1 const &, int>
          >();
        test_ext_int_1<
            std::pair<A_int_1, int>, std::pair<A_int_1 const, int>
          , std::pair<A_int_1 *, int>, std::pair<A_int_1 const *, int>
          , std::pair<A_wrap_1, int>, std::pair<A_wrap_1 const, int>
          , std::pair<A_base_1, int>, std::pair<A_base_1 const, int>
          >();
    }
    {
        test_ext_int_2<
            std::tuple<A_int_2 &, int, int>, std::tuple<A_int_2 const &, int, int>
          , std::tuple<A_int_2 *, int, int>, std::tuple<A_int_2 const *, int, int>
          , std::tuple<A_wrap_2 &, int, int>, std::tuple<A_wrap_2 const &, int, int>
          , std::tuple<A_base_2 &, int, int>, std::tuple<A_base_2 const &, int, int>
          >();
        test_ext_int_2<
            std::tuple<A_int_2, int, int>, std::tuple<A_int_2 const, int, int>
          , std::tuple<A_int_2 *, int, int>, std::tuple<A_int_2 const *, int, int>
          , std::tuple<A_wrap_2, int, int>, std::tuple<A_wrap_2 const, int, int>
          , std::tuple<A_base_2, int, int>, std::tuple<A_base_2 const, int, int>
          >();
    }

  return 0;
}
