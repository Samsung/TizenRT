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
// <algorithm>

// template<class Iter, IntegralLike Size, class T>
//   requires OutputIterator<Iter, const T&>
//   constexpr OutputIterator      // constexpr after C++17
//   fill_n(Iter first, Size n, const T& value);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "user_defined_integral.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    const std::size_t N = 5;
    int ib[] = {0, 0, 0, 0, 0, 0}; // one bigger than N

    auto it = std::fill_n(std::begin(ib), N, 5);
    return it == (std::begin(ib) + N)
        && std::all_of(std::begin(ib), it, [](int a) {return a == 5; })
        && *it == 0 // don't overwrite the last value in the output array
        ;
    }
#endif

typedef UserDefinedIntegral<unsigned> UDI;

template <class Iter>
void
test_char()
{
    char a[4] = {};
    Iter it = std::fill_n(Iter(a), UDI(4), char(1));
    TC_ASSERT_EXPR(base(it) == a + 4);
    TC_ASSERT_EXPR(a[0] == 1);
    TC_ASSERT_EXPR(a[1] == 1);
    TC_ASSERT_EXPR(a[2] == 1);
    TC_ASSERT_EXPR(a[3] == 1);
}

template <class Iter>
void
test_int()
{
    int a[4] = {};
    Iter it = std::fill_n(Iter(a), UDI(4), 1);
    TC_ASSERT_EXPR(base(it) == a + 4);
    TC_ASSERT_EXPR(a[0] == 1);
    TC_ASSERT_EXPR(a[1] == 1);
    TC_ASSERT_EXPR(a[2] == 1);
    TC_ASSERT_EXPR(a[3] == 1);
}

void
test_int_array()
{
    int a[4] = {};
    TC_ASSERT_EXPR(std::fill_n(a, UDI(4), static_cast<char>(1)) == a + 4);
    TC_ASSERT_EXPR(a[0] == 1);
    TC_ASSERT_EXPR(a[1] == 1);
    TC_ASSERT_EXPR(a[2] == 1);
    TC_ASSERT_EXPR(a[3] == 1);
}

struct source {
    source() : i(0) { }

    operator int() const { return i++; }
    mutable int i;
};

void
test_int_array_struct_source()
{
    int a[4] = {};
    TC_ASSERT_EXPR(std::fill_n(a, UDI(4), source()) == a + 4);
    TC_ASSERT_EXPR(a[0] == 0);
    TC_ASSERT_EXPR(a[1] == 1);
    TC_ASSERT_EXPR(a[2] == 2);
    TC_ASSERT_EXPR(a[3] == 3);
}

struct test1 {
    test1() : c(0) { }
    test1(char xc) : c(xc + 1) { }
    char c;
};

void
test_struct_array()
{
    test1 test1a[4] = {};
    TC_ASSERT_EXPR(std::fill_n(test1a, UDI(4), static_cast<char>(10)) == test1a + 4);
    TC_ASSERT_EXPR(test1a[0].c == 11);
    TC_ASSERT_EXPR(test1a[1].c == 11);
    TC_ASSERT_EXPR(test1a[2].c == 11);
    TC_ASSERT_EXPR(test1a[3].c == 11);
}

class A
{
    char a_;
public:
    A() {}
    explicit A(char a) : a_(a) {}
    operator unsigned char() const {return 'b';}

    friend bool operator==(const A& x, const A& y)
        {return x.a_ == y.a_;}
};

void
test5()
{
    A a[3];
    TC_ASSERT_EXPR(std::fill_n(&a[0], UDI(3), A('a')) == a+3);
    TC_ASSERT_EXPR(a[0] == A('a'));
    TC_ASSERT_EXPR(a[1] == A('a'));
    TC_ASSERT_EXPR(a[2] == A('a'));
}

struct Storage
{
  union
  {
    unsigned char a;
    unsigned char b;
  };
};

void test6()
{
  Storage foo[5];
  std::fill_n(&foo[0], UDI(5), Storage());
}


int tc_algorithms_alg_modifying_operations_alg_fill_fill_n(void) {
    test_char<cpp17_output_iterator<char*> >();
    test_char<forward_iterator<char*> >();
    test_char<bidirectional_iterator<char*> >();
    test_char<random_access_iterator<char*> >();
    test_char<char*>();

    test_int<cpp17_output_iterator<int*> >();
    test_int<forward_iterator<int*> >();
    test_int<bidirectional_iterator<int*> >();
    test_int<random_access_iterator<int*> >();
    test_int<int*>();

    test_int_array();
    test_int_array_struct_source();
    test_struct_array();

    test5();
    test6();

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
